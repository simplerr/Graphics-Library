#pragma once

#include "d3dUtil.h"
#include "Vertex.h"
#include "Light.h"

class Graphics;
class Texture2D;
class Primitive;

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Draw(Graphics* pGraphics);

	void SetPrimitive(Primitive* primitive);
	void SetMaterial(Material material);
	void SetTexture(Texture2D* texture);
	void LoadTexture(string filename);

	Primitive*		GetPrimitive();
	Material		GetMaterial();
private:
	Primitive*		mPrimitive;
	Texture2D*		mTexture;
	Material		mMaterial;
	vector<Vertex>	mVertices;
};
