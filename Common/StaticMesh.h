#pragma once

#include "d3dUtil.h"
#include "Vertex.h"
#include "Light.h"

class Graphics;
class Texture2D;
class Primitive;

/**
	A part of a static model that's rendered with the same texture and material.
	Contains a Primitive with the vertices and indices.
*/
class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();

	void Draw(Graphics* pGraphics);

	bool RayIntersect(XMVECTOR origin, XMVECTOR direction, float& pDist);
	void LoadTexture(string filename);
	void SetPrimitive(Primitive* primitive);
	void SetMaterial(Material material);
	void SetTexture(Texture2D* texture);
	void SetVertices(vector<Vertex> vertices);
	void SetIndices(vector<UINT> indices);

	Primitive*		GetPrimitive();
	Material		GetMaterial();
private:
	Primitive*		mPrimitive;
	Texture2D*		mTexture;
	Material		mMaterial;
	vector<Vertex>	mVertices;
	vector<UINT>	mIndices;
};
