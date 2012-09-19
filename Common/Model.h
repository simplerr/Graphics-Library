#pragma once

#include "d3dUtil.h"

class Graphics;
class Mesh;

class Model
{
public:
	Model();
	~Model();

	void Draw(Graphics* pGraphics, CXMMATRIX world);
	void AddMesh(Mesh* mesh);
private:
	vector<Mesh*> mMeshList;
};