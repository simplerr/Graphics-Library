#pragma once
#include "Object3D.h"

namespace GLib {
	// Forward declarations.
	class SkinnedModel;
	class World;

	//!
	//	Represents an animated object. 
	//!
	class AnimatedObject : public Object3D
	{
	public:
		AnimatedObject(ModelImporter* importer, string filename);
		virtual ~AnimatedObject();

		virtual void Init();
		virtual void Update(float dt);
		virtual void Draw(Graphics* pGraphics);

		bool RayIntersect(XMVECTOR origin, XMVECTOR direction, float& pDist);
		void SetAnimation(int index);
		AxisAlignedBox GetBoundingBox();
		SkinnedModel* GetModel();
		string GetFilename();
	private:
		SkinnedModel* mSkinnedModel;
		int		mCurrentAnimIndex;
		float	mElapsedTime;
	};
}