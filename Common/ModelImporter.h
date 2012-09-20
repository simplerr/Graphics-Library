#pragma once
#include <string>
#include <map>
#include <assimp\Importer.hpp>
#include "d3dUtil.h"
using namespace std;

class Primitive;
class PrimitiveFactory;
class Model;

class ModelImporter
{
public:
	ModelImporter(PrimitiveFactory* primitiveFactory);
	~ModelImporter();

	Model* LoadModel(string filename);
private:
	int FindValidPath(aiString* p_szString);
	bool TryLongerPath(char* szTemp,aiString* p_szString);
private:
	map<string, Model*> mModelMap;
	PrimitiveFactory*	mPrimtiveFactory;
	string				mFilename;
};