#pragma once

#include <vector>
#include "d3dUtil.h"

//! Graphics Library namespace.
namespace GLib
{
	// Forward declarations.
	class Graphics;
	class SkinnedMesh;
	class SceneAnimator;

	typedef vector<SkinnedMesh*> SkinnedMeshList;

	//!
	//	Contains a list of skinned meshes representing an animated model,
	//	and a SceneAnimator that calculates the bone transforms.
	//!
	class SkinnedModel
	{
	public:
		SkinnedModel();
		~SkinnedModel();

		void Update(float dt);
		void Draw(Graphics* pGraphics, CXMMATRIX world);
		void Save(string filename);
		void Load(string filename);
		void AddMesh(SkinnedMesh* mesh);
		void SetAnimator(SceneAnimator* animator);

		void SetAnimation(string animation);
		void SetAnimation(int index);
		void SetElapsedTime(float elapsedTime);

		SkinnedMeshList* GetMeshList();
		vector<XMFLOAT4X4> GetFinalTransforms();
		XNA::AxisAlignedBox GetBoundingBox();
		void CalculateAABB();
	private:
		SkinnedMeshList	mMeshList;
		SceneAnimator*	mAnimator;
		float			mElapsedTime;
		XNA::AxisAlignedBox mAABB;
	};
}