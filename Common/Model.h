#pragma once

#include "d3dUtil.h"

class Graphics;
class Mesh;

typedef vector<Mesh*> MeshList;

class Model
{
public:
	Model();
	~Model();

	void Draw(Graphics* pGraphics, CXMMATRIX world);
	void Cleanup();
	void AddMesh(Mesh* mesh);

	MeshList* GetMeshList();
private:
	MeshList mMeshList;
};