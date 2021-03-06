#include "AnimatedObject.h"
#include "Common\ModelImporter.h"
#include "Common\SkinnedModel.h"
#include "Common\Primitive.h"
#include "Common\Graphics.h"
#include "Common\SkinnedMesh.h"

AnimatedObject::AnimatedObject(ModelImporter* importer, string filename)
	: Object3D()
{
	// Load the model.
	mSkinnedModel = importer->LoadSkinnedModel(filename);
	mElapsedTime = 0.0f;
	mCurrentAnimIndex = 0;
}

//! Cleanup.
AnimatedObject::~AnimatedObject()
{
	
}

//! Updates the object.
void AnimatedObject::Update(float dt)
{
	// Increments the elapsed time counter used for animations. 
	mElapsedTime += dt;
	mSkinnedModel->Update(dt);
}

//! Draws the objects model.
void AnimatedObject::Draw(Graphics* pGraphics)
{
	mSkinnedModel->SetAnimation(mCurrentAnimIndex);
	mSkinnedModel->SetElapsedTime(mElapsedTime);
	mSkinnedModel->Draw(pGraphics, GetWorldMatrix());
	//pGraphics->DrawBoundingBox(&GetBoundingBox(), GetWorldMatrix(), Material(Colors::Green));
}

//! Sets which animation to use by index.
void AnimatedObject::SetAnimation(int index)
{
	mCurrentAnimIndex = index;
}

//! Returns the bounding box in world space. [NOTE] Does not work [TODO].
AxisAlignedBox AnimatedObject::GetBoundingBox()
{
	SkinnedMeshList* meshList = mSkinnedModel->GetMeshList();
	AxisAlignedBox aabb = (*meshList)[0]->GetPrimitive()->GetBoundingBox();
	XMFLOAT3 min = aabb.Center - aabb.Extents;
	XMFLOAT3 max = aabb.Center + aabb.Extents;
	for(int i = 1; i < meshList->size(); i++)
	{
		AxisAlignedBox meshAABB = (*meshList)[i]->GetPrimitive()->GetBoundingBox();
		XMFLOAT3 meshMin = meshAABB.Center - meshAABB.Extents;
		XMFLOAT3 meshMax = meshAABB.Center + meshAABB.Extents;

		XMStoreFloat3(&min, XMVectorMin(XMLoadFloat3(&meshMin), XMLoadFloat3(&min)));
		XMStoreFloat3(&max, XMVectorMax(XMLoadFloat3(&meshMax), XMLoadFloat3(&max)));
	}

	aabb.Center =  (min + max) * 0.5f;
	aabb.Extents = (max - min) * 0.5f;

	// Break up the world matrix into it's components.
	XMVECTOR scale, rotation, translation;
	XMMatrixDecompose(&scale, &rotation, &translation, GetWorldMatrix());

	// Transform the AABB with the components.
	TransformAxisAlignedBoxCustom(&aabb, &aabb, scale, rotation, translation);

	return aabb;
}

//! Returns the model.
SkinnedModel* AnimatedObject::GetModel()
{
	return mSkinnedModel;
}