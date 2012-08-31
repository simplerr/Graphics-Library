#include <crtdbg.h>
#include <assert.h>
#include "Game.h"
#include "Common\D3DCore.h"
#include "Common\d3dUtil.h"
#include "Common\Graphics.h"
#include "Common\PrimitiveFactory.h"
#include "Common\Object3D.h"
#include "Common\Input.h"
#include "World.h"
#include "Common\Light.h"

// Set globals to nullptrs
Runnable*			gGame				= nullptr;
PrimitiveFactory*	gPrimitiveFactory	= nullptr;
Input*				gInput				= nullptr;

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Game game(hInstance, "Graphics Library", 800, 600);
	gGame = &game;

	game.Init();

	gInput = new Input();

	return gGame->Run();
}

Game::Game(HINSTANCE hInstance, string caption, int width, int height)
	: Runnable(hInstance, caption, width, height)
{
	
}
	
Game::~Game()
{
	delete gPrimitiveFactory;
	delete gInput;
	delete mWorld;
}

void Game::Init()
{
	// Important to run Systems Init() function.
	Runnable::Init();

	// Create the primitive factory.
	gPrimitiveFactory = new PrimitiveFactory();

	// Create the world.
	mWorld = new World();

	// Connect the graphics light list to the one in World.
	GetGraphics()->SetLightList(mWorld->GetLights());

	// Add some objects.
	Object3D* object = new Object3D();

	// Load the effect and the primitive.
	object->SetEffect(gGame->GetGraphics()->LoadEffect("Lighting.fx", "LightTech"));
	object->SetPrimitive(gPrimitiveFactory->CreateBox());
	object->SetPosition(XMFLOAT3(0, 3, 0));

	mWorld->AddObject(object);

	object = new Object3D();

	object->SetEffect(gGame->GetGraphics()->LoadEffect("Lighting.fx", "LightTech"));
	object->SetPrimitive(gPrimitiveFactory->CreateGrid(160.0f, 160.0f, 50, 50));

	mWorld->AddObject(object);

	// Add some lights.
	mLight = new Light();
	mLight->SetMaterials(Colors::White*0.3f, Colors::White*0.5f, Colors::White*0.2f);
	mLight->SetDirection(0.7f, -1.0f, 0.7);
	mLight->SetType(POINT_LIGHT);
	mLight->SetPosition(0, 10, 0);
	mLight->SetAtt(1.0f/2.0f, 0.0f, 0.0f);
	mLight->SetRange(2500.0f);
	mLight->SetSpot(5.0f);
	mWorld->AddLight(mLight);
}
	
void Game::Update(float dt)
{
	gInput->Update(dt);
	GetGraphics()->Update(dt);
}
	
void Game::Draw(Graphics* pGraphics)
{
	pGraphics->ClearScene();

	// Draw all objects.
	mWorld->Draw(pGraphics);

	pGraphics->Present();
}

LRESULT Game::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		
		};
		break;
	case WM_LBUTTONDOWN:
		{
		
		break;
		}
	case WM_RBUTTONDOWN: 
		{
		
		break;
		}
	};

	return Runnable::MsgProc(hwnd, msg, wParam, lParam);
}