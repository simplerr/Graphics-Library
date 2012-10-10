#include "SkinnedMesh.h"
#include "Graphics.h"
#include "Effects.h"
#include "Primitive.h"
#include <assimp\scene.h>
#include <fstream>

SkinnedMesh::SkinnedMesh()
{
	mTexture = nullptr;
	mPrimitive = nullptr;
}

SkinnedMesh::~SkinnedMesh()
{

}

void SkinnedMesh::Draw(Graphics* pGraphics)
{
	// Set the material properties for this SkinnedMesh.
	Effects::BasicFX->SetMaterial(mMaterial);
	Effects::BasicFX->SetTexture(mTexture);
	Effects::BasicFX->GetSkinnedTech()->GetPassByIndex(0)->Apply(0, GetD3DContext());

	ID3D11DeviceContext* dc = pGraphics->GetContext();

	UINT stride = sizeof(SkinnedVertex);
	UINT offset = 0;
	// Set the input layout and the primitive topology.
	dc->IASetInputLayout(Effects::BasicFX->GetInputLayout());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the SkinnedMesh primitive.
	mPrimitive->Draw<SkinnedVertex>(pGraphics->GetContext());
}

void SkinnedMesh::Update(float dt)
{
	
}

void SkinnedMesh::Save(ofstream& fout)
{
	fout << "-Mesh-\r\n";
	fout << "#Texture " << mTexture->name << "\r\n";
	fout << "#NumVertices " << mVertices.size() << "\r\n";
	fout << "#NumIndices " << mIndices.size() << "\r\n";
	fout << "#Vertices" << "\r\n";

	for(int i = 0; i < mVertices.size(); i++) 
	{
		/*fout.write(reinterpret_cast<char*>(&mVertices[i].Pos.x), sizeof(mVertices[i].Pos.x));
		fout.write(reinterpret_cast<char*>(&mVertices[i].Pos.y), sizeof(mVertices[i].Pos.y));
		fout.write(reinterpret_cast<char*>(&mVertices[i].Pos.z), sizeof(mVertices[i].Pos.z));*/
		fout << "Pos: " << mVertices[i].Pos.x << " " << mVertices[i].Pos.y << " " << mVertices[i].Pos.z << "\r\n";
		fout << "Normal: " << mVertices[i].Normal.x << " " << mVertices[i].Normal.y << " " << mVertices[i].Normal.z << "\r\n";
		fout << "Tangent: " << mVertices[i].Tangent.x << " " << mVertices[i].Tangent.y << " " << mVertices[i].Tangent.z << "\r\n";
		fout << "UV: " << mVertices[i].Tex.x << " " << mVertices[i].Tex.y << "\r\n";
		fout << "BoneId " << (int)mVertices[i].BoneIndices[0] << " " << (int)mVertices[i].BoneIndices[1] << " " << (int)mVertices[i].BoneIndices[2] << " " << (int)mVertices[i].BoneIndices[3] << "\r\n";
		fout << "Weights: " << mVertices[i].Weights.x << " " << mVertices[i].Weights.y << " " << mVertices[i].Weights.z << "\r\n";
		fout << "-\r\n";
	}

	fout << "\r\n#Indices" << "\r\n";

	for(int i = 0; i < mIndices.size(); i++)
		fout << mIndices[i] << " ";

	fout << "\r\n";
}
	
void SkinnedMesh::Load(ifstream& fin)
{
	string ignore, texture;
	int numVertices, numIndices;
	fin >> ignore; // -Mesh-
	fin >> ignore >> texture; // #Texture
	fin >> ignore >> numVertices; // #NumVertices
	fin >> ignore >> numIndices; // #NumIndices
	fin >> ignore; // #Vertices

	// Read all vertices;
	vector<SkinnedVertex> vertices;
	for(int i = 0; i < numIndices; i++)
	{
		SkinnedVertex vertex;

		fin >> ignore >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
		fin >> ignore >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
		fin >> ignore >> vertex.Tangent.x >> vertex.Tangent.y >> vertex.Tangent.z;
		fin >> ignore >> vertex.Tex.x >> vertex.Tex.y;
		int boneIds[4];
		fin >> ignore >> boneIds[0] >> boneIds[1] >> boneIds[2] >> boneIds[3];
		fin >> ignore >> vertex.Weights.x >> vertex.Weights.y >> vertex.Weights.z;
		fin >> ignore; // -

		vertex.BoneIndices[0] = boneIds[0];
		vertex.BoneIndices[1] = boneIds[1];
		vertex.BoneIndices[2] = boneIds[2];
		vertex.BoneIndices[3] = boneIds[3];

		vertices.push_back(vertex);
	}

	// Read all indices.
	fin >> ignore; // #Indices
	vector<UINT> indices;
	for(int i = 0; i < numIndices; i++)
	{
		UINT index;
		fin >> index;
		indices.push_back(index);
	}

	// Create the primitive.
	mPrimitive = new Primitive(GetD3DDevice(), vertices, indices);
	SetVertices(vertices);
	SetIndices(indices);

	// Load the texture.
	LoadTexture(texture);
	
	SetMaterial(Material(Colors::White));
}


void SkinnedMesh::SetPrimitive(Primitive* primitive)
{
	mPrimitive = primitive;
}

void SkinnedMesh::SetVertices(vector<SkinnedVertex> vertices)
{
	mVertices = vertices;
}
	
void SkinnedMesh::SetIndices(vector<UINT> indices)
{
	mIndices = indices;
}

void SkinnedMesh::LoadTexture(string filename)
{
	mTexture = GetGraphics()->LoadTexture(filename);
}

void SkinnedMesh::SetMaterial(Material material)
{
	mMaterial = material;
}
	
void SkinnedMesh::SetTexture(Texture2D* texture)
{
	mTexture = texture;
}

Primitive* SkinnedMesh::GetPrimitive()
{
	return mPrimitive;
}

Material SkinnedMesh::GetMaterial()
{
	return mMaterial;
}