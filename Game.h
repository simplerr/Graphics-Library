#pragma once

#include "Common\Runnable.h"

class StaticObject;
class World;
class Light;
class RenderTarget;
class Primitive;
class Texture2D;
class ShadowMap;
class ModelImporter;
class Model;
class SkinnedModel;
class AnimatedObject;

/** GWEN **/
#include <objbase.h>
#include <GdiPlus.h>

#include "Gwen/Gwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"
#include "Gwen/UnitTest/UnitTest.h"
#include "Gwen/Input/Windows.h"
#include "Gwen/Renderers/GDIPlus.h"

#pragma comment( lib, "gdiplus.lib" )
#pragma comment( lib, "gwend_static.lib" )
#pragma comment( lib, "GWEN-Renderer-GDI.lib" )

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
	StaticObject*	mObject;
	StaticObject*	mObject2;
	StaticObject*	mTerrain;
	ModelImporter*	mModelImporter;
	AnimatedObject* mAnimatedObject;
};