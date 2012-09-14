#pragma once

#include <vector>
#include <map>
#include "d3dUtil.h"
using namespace std;

class Graphics;
class Object3D;
class Light;
class Sky;
class BillboardManager;

class World
{
public:
	World();
	~World();

	void Update(float dt);
	void Draw(Graphics* pGraphics);
	void Init();
	void AddObject(Object3D* object);
	void AddLight(Light* light);

	LightList* GetLights();
	ObjectList* GetObjects();
	int GetVisibleObjects();
private:
	ObjectList	mObjectList;
	LightList	mLightList;//LightList mLightList;	
	Sky*		mSkyBox;
	Object3D*	mDebugObject;
	int			mNumVisibleObjects;
};