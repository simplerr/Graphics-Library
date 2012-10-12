#pragma once

#include "d3dUtil.h"

class Graphics;
class StaticMesh;

typedef vector<StaticMesh*> MeshList;

/**
	Contains a list of static meshes representing a model.
*/
class StaticModel
{
public:
	StaticModel();
	~StaticModel();

	void Draw(Graphics* pGraphics, CXMMATRIX world);
	void AddMesh(StaticMesh* mesh);

	MeshList* GetMeshList();
private:
	MeshList mMeshList;
};