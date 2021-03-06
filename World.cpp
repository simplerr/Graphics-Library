#include "World.h"
#include "Object3D.h"
#include "Common\Light.h"
#include "Common\PrimitiveFactory.h"
#include "Common\Runnable.h"
#include "Effects.h"
#include "Common\Graphics.h"
#include "Common\xnacollision.h"
#include "Common\Camera.h"
#include "Common\Sky.h"
#include "Common\Terrain.h"
#include "Common\StaticModel.h"

World::World()
{
	mNumVisibleObjects = 0;
}

void World::Init()
{
	//mDebugObject = new Object3D(gPrimitiveFactory->CreateBox());
	//mDebugObject->SetTexture("textures/crate.dds");

	// Create the sky box.
	mSkyBox = new Sky("textures/sky.dds", 5000.0f);

	// Create the terrain.
	mTerrain = new Terrain();
	InitInfo initInfo;
	initInfo.HeightMapFilename = "textures/heightmap17_257.raw";
	initInfo.LayerMapFilename0 = "textures/grass.dds";
	initInfo.LayerMapFilename1 = "textures/darkdirt.dds";
	initInfo.LayerMapFilename2 = "textures/stone.dds";
	initInfo.LayerMapFilename3 = "textures/lightdirt.dds";
	initInfo.LayerMapFilename4 = "textures/snow.dds";
	initInfo.BlendMapFilename = "textures/blend.dds";
	initInfo.HeightScale = 15.0f;
	initInfo.HeightmapWidth = 257;
	initInfo.HeightmapHeight = 257;
	initInfo.CellSpacing = 0.5f;
	mTerrain->Init(gGame->GetGraphics()->GetDevice(), gGame->GetGraphics()->GetContext(), initInfo);
}

//! Cleanup the object and light lists.
World::~World()
{
	delete mSkyBox;
	delete mTerrain;
	//delete mDebugObject;

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
	// [TODO]!!
	//XMFLOAT3 pos = gGame->GetGraphics()->GetCamera()->GetPosition();
	//gGame->GetGraphics()->GetCamera()->SetPosition(XMFLOAT3(pos.x, mTerrain->GetHeight(pos.x, pos.z)+3, pos.z));
	//gGame->GetGraphics()->GetCamera()->SetTarget(gGame->GetGraphics()->GetCamera()->GetPosition() + gGame->GetGraphics()->GetCamera()->GetDirection());

	for(int i = 0; i < mObjectList.size(); i++)
	{
		mObjectList[i]->Update(dt);
	}
}
	
//! Draws all objects.
void World::Draw(Graphics* pGraphics)
{
	// Draw all objects to the shadow map.
	/*pGraphics->ActiveShadowMap();
	for(int i = 0; i < mObjectList.size(); i++)
		mObjectList[i]->Draw(pGraphics);
	pGraphics->DeactiveShadowMap();*/

	// Draw the terrain.
	mTerrain->Draw(pGraphics);

	mNumVisibleObjects = 0;
	for(int i = 0; i < mObjectList.size(); i++)
	{
		// Don't draw the object if it's outside the cameras frustum.
		//Frustum frustum = pGraphics->GetCamera()->GetFrustum();
		//if(XNA::IntersectAxisAlignedBoxFrustum(&mObjectList[i]->GetBoundingBox(), &frustum)) {
			mObjectList[i]->Draw(pGraphics);
			mNumVisibleObjects++;
		//}
	}

	for(int i = 0; i < mLightList.size(); i++)
	{
		//mDebugObject->SetPosition(mLightList[i]->GetPosition());
		//mDebugObject->SetRotation(mLightList[i]->GetDirection());
		//mDebugObject->Draw(pGraphics);
	}

	//mSkyBox->Draw();
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