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
	void Cleanup();
	void AddMesh(Mesh* mesh);

	vector<Mesh*>* GetMeshList();
private:
	vector<Mesh*> mMeshList;
};