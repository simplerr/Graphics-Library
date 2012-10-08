#pragma once

#include <vector>
#include "d3dUtil.h"

class Graphics;
class SkinnedMesh;
class SceneAnimator;

typedef vector<SkinnedMesh*> SkinnedMeshList;

class SkinnedModel
{
public:
	SkinnedModel();
	~SkinnedModel();

	void Update(float dt);
	void Draw(Graphics* pGraphics, CXMMATRIX world);
	void Cleanup();
	void AddMesh(SkinnedMesh* mesh);
	void SetAnimator(SceneAnimator* animator);

	void SetAnimation(string animation);
	void SetAnimation(int index);

	SkinnedMeshList* GetMeshList();
private:
	SkinnedMeshList	mMeshList;
	SceneAnimator*	mAnimator;
	float			mElapsedTime;
};