#pragma once
#include <string>
#include <map>
#include <assimp\Importer.hpp>
#include <assimp\mesh.h>
#include <assimp\scene.h>
#include "d3dUtil.h"
using namespace std;

class Primitive;
class PrimitiveFactory;
class Model;
class SkinnedModel;
struct BoneInfo;

struct Weights
{
	vector<int>		boneIndices;
	vector<float>	weights;
};

class ModelImporter
{
public:
	ModelImporter(PrimitiveFactory* primitiveFactory);
	~ModelImporter();

	Model* LoadModel(string filename);
	SkinnedModel* LoadSkinnedModel(string filename);

	vector<Weights> CalculateWeights(aiMesh* mesh);

	void CalculateBoneInfo(map<string, BoneInfo>& boneInfos, aiNode* node);
private:
	int FindValidPath(aiString* p_szString);
	bool TryLongerPath(char* szTemp,aiString* p_szString);
private:
	map<string, Model*> mModelMap;
	PrimitiveFactory*	mPrimtiveFactory;
	string				mFilename;
};