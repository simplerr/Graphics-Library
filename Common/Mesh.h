#pragma once

#include "d3dUtil.h"
#include "Vertex.h"
#include "Light.h"

class Graphics;
class Texture2D;

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Draw(Graphics* pGraphics);

	void SetVertices(ID3D11Device* device, vector<Vertex> vertices);
	void SetIndices(ID3D11Device* device, vector<UINT> indices);
	void SetMaterial(Material material);
	void SetTexture(Texture2D* texture);
	void LoadTexture(string filename);

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetNumIndices();
private:
	ID3D11Buffer*	mVertexBuffer;
	ID3D11Buffer*	mIndexBuffer;
	Texture2D*		mTexture;
	Material		mMaterial;
	AxisAlignedBox	mBoundingBox;
	UINT			mNumVertices;
	UINT			mNumIndices;
	vector<Vertex>	mVertices;
};
