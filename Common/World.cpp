#include "World.h"
#include "Object3D.h"
#include "Light.h"
#include "PrimitiveFactory.h"
#include "Runnable.h"
#include "Effects.h"
#include "Graphics.h"

World::World()
{

}

void World::Init()
{
	mDebugObject = new Object3D();
	mDebugObject->SetPrimitive(gPrimitiveFactory->CreateBox());
	mDebugObject->LoadTexture("textures/crate.dds");
}

//! Cleanup the object and light lists.
World::~World()
{
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
	for(int i = 0; i < mObjectList.size(); i++)
	{
		mObjectList[i]->Draw(pGraphics);
	}

	for(int i = 0; i < mLightList.size(); i++)
	{
		mDebugObject->SetPosition(mLightList[i]->GetPosition());
		mDebugObject->SetRotation(mLightList[i]->GetDirection());
		mDebugObject->Draw(pGraphics);
	}
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