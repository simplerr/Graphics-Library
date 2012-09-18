#pragma once

#include "Common\Runnable.h"

class Object3D;
class World;
class Light;
class RenderTarget;
class Primitive;
class Texture2D;
class ShadowMap;
class ModelImporter;

class Game : public Runnable
{
public:
	Game(HINSTANCE hInstance, string caption, int width, int height);
	~Game();

	void Init();
	void Update(float dt);
	void Draw(Graphics* pGraphics);

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	World* mWorld;
	Light* mLight;
	Light* mLight2;
	RenderTarget* mRenderTarget;
	Primitive* mPrimitive;
	Object3D*  mObject;
	Object3D*  mObject2;
	Object3D*  mTerrain;
	ModelImporter* mModelImporter;
};