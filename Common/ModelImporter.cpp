#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>  
#include <assimp\scene.h>
#include "ModelImporter.h"
#include "Vertex.h"
#include "Primitive.h"
#include "PrimitiveFactory.h"
#include "Runnable.h"
#include "Graphics.h"
#include <assimp\cimport.h>
#include <assimp\material.h>
#include <assimp\ai_assert.h>
#include "Model.h"
#include "Mesh.h"
#include "Light.h"

ModelImporter::ModelImporter(PrimitiveFactory* primitiveFactory)
{
	mPrimtiveFactory = primitiveFactory;	
}

ModelImporter::~ModelImporter()
{
	// Cleanup all the meshes.
	for(auto iter = mModelMap.begin(); iter != mModelMap.end(); iter++) {
		//(*iter).second->Cleanup();
		//delete (*iter).second;
	}
}

Model* ModelImporter::LoadModel(string filename)
{
	// Is the model already loaded?
	if(mModelMap.find(filename) != mModelMap.end())
		return mModelMap[filename];

	Assimp::Importer importer;
	mFilename =	filename;
	Model* model = NULL;

	// Important! Makes sure that if the angle between two face normals is > 80 they are not smoothed together.
	// Since the angle between a cubes face normals is 90 the lighting looks very bad if we don't specify this.
	importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);	
	importer.SetPropertyInteger(AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE);

	// Load scene from the file.
	const aiScene* scene = importer.ReadFile(filename, 
		aiProcess_CalcTangentSpace		| 
		aiProcess_Triangulate			| 
		aiProcess_GenSmoothNormals		|
		aiProcess_SplitLargeMeshes		|
		aiProcess_ConvertToLeftHanded	|
		aiProcess_SortByPType);

	string error = importer.GetErrorString();

	// Successfully loaded the scene.
	if(scene)
	{
		// Create the model that is getting filled out.
		model = new Model();

		// Loop through all meshes.
		for(int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* assimpMesh = scene->mMeshes[i];
			vector<Vertex>	vertices;
			vector<UINT>	indices;

			// Add vertices to the vertex list.
			for(int i = 0; i < assimpMesh->mNumVertices; i++) 
			{
				aiVector3D v = assimpMesh->mVertices[i];
				aiVector3D n = assimpMesh->mNormals[i];
				aiVector3D t = assimpMesh->mTextureCoords[0][i];

				n = n.Normalize();
				Vertex vertex(v.x, v.y, v.z, n.x, n.y, n.z, 0, 0, 0, t.x, t.y);
				vertices.push_back(vertex);
			}

			// Add indices to the index list.
			for(int i = 0; i < assimpMesh->mNumFaces; i++) 
				for(int j = 0; j < assimpMesh->mFaces[i].mNumIndices; j++) 
					indices.push_back(assimpMesh->mFaces[i].mIndices[j]);

			// Get the path to the texture in the directory.
			aiString path;
			aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];
			material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path);
			FindValidPath(&path);

			// Extract all the ambient, diffuse and specular colors.
			aiColor4D ambient, diffuse, specular;
			material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

			// Create the mesh and its primitive.
			Mesh* mesh = new Mesh();
			Primitive* primitive = new Primitive(GetD3DDevice(), vertices, indices);
			mesh->SetPrimitive(primitive);
			mPrimtiveFactory->AddPrimitive(path.C_Str(), primitive);

			if(_stricmp(path.C_Str(), "") != 0)
				mesh->LoadTexture(path.C_Str());

			// [NOTE] The material should probably be white instead!
			mesh->SetMaterial(Material(ambient, diffuse, specular));

			// Add the mesh to the model.
			model->AddMesh(mesh);
		}
	}

	// Add to the model map and return it.
	mModelMap[filename] = model;
	return mModelMap[filename];
}

int ModelImporter::FindValidPath(aiString* p_szString)
{
	ai_assert(NULL != p_szString);
	aiString pcpy = *p_szString;
	if ('*' ==  p_szString->data[0])	{
		// '*' as first character indicates an embedded file
		return 5;
	}

	// first check whether we can directly load the file
	FILE* pFile = fopen(p_szString->data,"rb");
	if (pFile)fclose(pFile);
	else
	{
		// check whether we can use the directory of  the asset as relative base
		char szTemp[MAX_PATH*2], tmp2[MAX_PATH*2];
		strcpy(szTemp, mFilename.c_str());
		strcpy(tmp2,szTemp);

		char* szData = p_szString->data;
		if (*szData == '\\' || *szData == '/')++szData;

		char* szEnd = strrchr(szTemp,'\\');
		if (!szEnd)
		{
			szEnd = strrchr(szTemp,'/');
			if (!szEnd)szEnd = szTemp;
		}
		szEnd++;
		*szEnd = 0;
		strcat(szEnd,szData);


		pFile = fopen(szTemp,"rb");
		if (!pFile)
		{
			// convert the string to lower case
			for (unsigned int i = 0;;++i)
			{
				if ('\0' == szTemp[i])break;
				szTemp[i] = (char)tolower(szTemp[i]);
			}

			if(TryLongerPath(szTemp,p_szString))return 1;
			*szEnd = 0;

			// search common sub directories
			strcat(szEnd,"tex\\");
			strcat(szEnd,szData);

			pFile = fopen(szTemp,"rb");
			if (!pFile)
			{
				if(TryLongerPath(szTemp,p_szString))return 1;

				*szEnd = 0;

				strcat(szEnd,"textures\\");
				strcat(szEnd,szData);

				pFile = fopen(szTemp,"rb");
				if (!pFile)
				{
					if(TryLongerPath(szTemp, p_szString))return 1;
				}

				// patch by mark sibly to look for textures files in the asset's base directory.
				const char *path=pcpy.data; 
				const char *p=strrchr( path,'/' ); 
				if( !p ) p=strrchr( path,'\\' ); 
				if( p ){ 
					char *q=strrchr( tmp2,'/' ); 
					if( !q ) q=strrchr( tmp2,'\\' ); 
					if( q ){ 
						strcpy( q+1,p+1 ); 
						if(pFile=fopen( tmp2,"r" ) ){ 
							fclose( pFile ); 
							strcpy(p_szString->data,tmp2);
							p_szString->length = strlen(tmp2);
							return 1;
						} 
					} 
				}
				return 0;
			}
		}
		fclose(pFile);

		// copy the result string back to the aiString
		const size_t iLen = strlen(szTemp);
		size_t iLen2 = iLen+1;
		iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
		memcpy(p_szString->data,szTemp,iLen2);
		p_szString->length = iLen;

	}
	return 1;
}

bool ModelImporter::TryLongerPath(char* szTemp,aiString* p_szString)
{
	char szTempB[MAX_PATH];
	strcpy(szTempB,szTemp);

	// go to the beginning of the file name
	char* szFile = strrchr(szTempB,'\\');
	if (!szFile)szFile = strrchr(szTempB,'/');

	char* szFile2 = szTemp + (szFile - szTempB)+1;
	szFile++;
	char* szExt = strrchr(szFile,'.');
	if (!szExt)return false;
	szExt++;
	*szFile = 0;

	strcat(szTempB,"*.*");
	const unsigned int iSize = (const unsigned int) ( szExt - 1 - szFile );

	HANDLE          h;
	WIN32_FIND_DATA info;

	// build a list of files
	h = FindFirstFile(szTempB, &info);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(strcmp(info.cFileName, ".") == 0 || strcmp(info.cFileName, "..") == 0))
			{
				char* szExtFound = strrchr(info.cFileName, '.');
				if (szExtFound)
				{
					++szExtFound;
					if (0 == _stricmp(szExtFound,szExt))
					{
						const unsigned int iSizeFound = (const unsigned int) ( 
							szExtFound - 1 - info.cFileName);

						for (unsigned int i = 0; i < iSizeFound;++i)
							info.cFileName[i] = (CHAR)tolower(info.cFileName[i]);

						if (0 == memcmp(info.cFileName,szFile2, min(iSizeFound,iSize)))
						{
							// we have it. Build the full path ...
							char* sz = strrchr(szTempB,'*');
							*(sz-2) = 0x0;

							strcat(szTempB,info.cFileName);

							// copy the result string back to the aiString
							const size_t iLen = strlen(szTempB);
							size_t iLen2 = iLen+1;
							iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
							memcpy(p_szString->data,szTempB,iLen2);
							p_szString->length = iLen;
							return true;
						}
					}
					// check whether the 8.3 DOS name is matching
					if (0 == _stricmp(info.cAlternateFileName,p_szString->data))
					{
						strcat(szTempB,info.cAlternateFileName);

						// copy the result string back to the aiString
						const size_t iLen = strlen(szTempB);
						size_t iLen2 = iLen+1;
						iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
						memcpy(p_szString->data,szTempB,iLen2);
						p_szString->length = iLen;
						return true;
					}
				}
			}
		} 
		while (FindNextFile(h, &info));

		FindClose(h);
	}
	return false;
}