#pragma once

#include "d3dUtil.h"

class Graphics;
class StaticMesh;
struct Material;

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
	bool RayIntersect(XMVECTOR origin, XMVECTOR direction, float& pDist);

	void SetMaterial(Material material);
	MeshList* GetMeshList();
private:
	MeshList mMeshList;
};