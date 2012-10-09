#pragma once

#include <map>
#include "d3dUtil.h"
#include "Vertex.h"
#include "Light.h"

class Graphics;
class Texture2D;
class Primitive;

struct BoneInfo
{
	XMFLOAT4X4	offsetTransform;
	XMFLOAT4X4  toParentTransform;
	string		parent;
};

class SkinnedMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

	void Update(float dt);
	void Draw(Graphics* pGraphics);

	void SetPrimitive(Primitive* primitive);
	void SetVertices(vector<SkinnedVertex> vertices);
	void SetIndices(vector<UINT> indices);
	void SetMaterial(Material material);
	void SetTexture(Texture2D* texture);
	void LoadTexture(string filename);

	void Save(ofstream& fout);
	void Load(ifstream& fin);

	Primitive*		GetPrimitive();
	Material		GetMaterial();
private:
	Primitive*		mPrimitive;
	Texture2D*		mTexture;
	Material		mMaterial;
	vector<SkinnedVertex> mVertices;
	vector<UINT>		  mIndices;
};
