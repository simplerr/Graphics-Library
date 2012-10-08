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
	void SetMaterial(Material material);
	void SetTexture(Texture2D* texture);
	void LoadTexture(string filename);

	void SetVertices(ID3D11Device* device, const SkinnedVertex* vertices, int size);
	void SetIndices(ID3D11Device* device, vector<UINT> indices);

	Primitive*		GetPrimitive();
	Material		GetMaterial();
private:
	ID3D11Buffer*	mVertexBuffer;
	ID3D11Buffer*	mIndexBuffer;
	UINT			mNumIndices;
	Primitive*		mPrimitive;
	Texture2D*		mTexture;
	Material		mMaterial;
};
