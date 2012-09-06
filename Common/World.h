#pragma once

#include <vector>
#include <map>
#include "d3dUtil.h"
using namespace std;

class Graphics;
class Object3D;
class Light;
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
private:
	vector<Object3D*> mObjectList;
	LightList mLightList;//LightList mLightList;	
	Object3D* mDebugObject;
};