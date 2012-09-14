#include "World.h"
#include "Object3D.h"
#include "Light.h"
#include "PrimitiveFactory.h"
#include "Runnable.h"
#include "Effects.h"
#include "Graphics.h"
#include "xnacollision.h"
#include "Camera.h"
#include "Sky.h"

World::World()
{
	mNumVisibleObjects = 0;
}

void World::Init()
{
	mDebugObject = new Object3D();
	mDebugObject->SetPrimitive(gPrimitiveFactory->CreateBox());
	mDebugObject->LoadTexture("textures/crate.dds");

	// Create the sky box.
	mSkyBox = new Sky("textures/sky.dds", 5000.0f);
}

//! Cleanup the object and light lists.
World::~World()
{
	delete mSkyBox;

	// Delete all objects.
	for(int i = 0; i < mObjectList.size(); i++)
		delete mObjectList[i];

	// Delete all lights.
	for(int i = 0; i < mLightList.size(); i++)
		delete mLightList[i];
}

//! Updates all objects.
void World::Update(float dt)
{
	for(int i = 0; i < mObjectList.size(); i++)
	{
		mObjectList[i]->Update(dt);
	}
}
	
//! Draws all objects.
void World::Draw(Graphics* pGraphics)
{
	mNumVisibleObjects = 0;
	for(int i = 0; i < mObjectList.size(); i++)
	{
		// Don't draw the object if it's outside the cameras frustum.
		Frustum frustum = pGraphics->GetCamera()->GetFrustum();
		if(XNA::IntersectAxisAlignedBoxFrustum(&mObjectList[i]->GetBoundingBox(), &frustum)) {
			mObjectList[i]->Draw(pGraphics);
			mNumVisibleObjects++;
		}
	}

	for(int i = 0; i < mLightList.size(); i++)
	{
		mDebugObject->SetPosition(mLightList[i]->GetPosition());
		mDebugObject->SetRotation(mLightList[i]->GetDirection());
		//mDebugObject->Draw(pGraphics);
	}

	mSkyBox->Draw();
}

//! Adds a object to the object list.
void World::AddObject(Object3D* object)
{
	// [TODO] Set Id.
	mObjectList.push_back(object);
}
	
//! Adds a light to the light list.
void World::AddLight(Light* light)
{
	mLightList.push_back(light);
}

//! Returns the address to the light list.
LightList* World::GetLights()
{
	return &mLightList;
}

//! Returns the address to the object list.
ObjectList* World::GetObjects()
{
	return &mObjectList;
}

//! Returns the number of visible objects (inside the camera frustum).
int World::GetVisibleObjects()
{
	return mNumVisibleObjects;
}