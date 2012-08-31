#pragma once

#include <vector>
using namespace std;

class Graphics;
class Object3D;
class Light;

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
	vector<Light*>* GetLights();
private:
	vector<Object3D*> mObjectList;
	vector<Light*> mLightList;
};