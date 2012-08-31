#include <crtdbg.h>
#include <assert.h>
#include "Game.h"
#include "Common\D3DCore.h"
#include "Common\d3dUtil.h"
#include "Common\Graphics.h"
#include "Common\PrimitiveFactory.h"
#include "Common\Object3D.h"
#include "Common\Input.h"
#include "Common\World.h"
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
	mWorld->Init();

	// Connect the graphics light list to the one in World.
	GetGraphics()->SetLightList(mWorld->GetLights());

	// Add some objects.
	Object3D* object = new Object3D();

	// Load the effect and the primitive.
	object->SetEffect(gGame->GetGraphics()->LoadEffect("Base.fx", "BaseTech"));
	object->SetPrimitive(gPrimitiveFactory->CreateBox());
	object->SetTexture("textures/crate.dds");
	object->SetPosition(XMFLOAT3(0, 3, 0));

	mWorld->AddObject(object);

	object = new Object3D();

	object->SetEffect(gGame->GetGraphics()->LoadEffect("Base.fx", "BaseTech"));
	object->SetPrimitive(gPrimitiveFactory->CreateGrid(160.0f, 160.0f, 50, 50));
	object->SetTexture("textures/grass.png", 15.0f);

	mWorld->AddObject(object);

	// Add some lights.
	mLight = new Light();
	mLight->SetMaterials(Colors::Red*0.2f, Colors::Red*0.8f, Colors::Red*0.8f);
	mLight->SetDirection(0.7f, -1.0f, 0.7);
	mLight->SetType(SPOT_LIGHT);
	mLight->SetPosition(0, 10, 0);
	mLight->SetAtt(0.0f, 1.0f/20, 0.0f);
	mLight->SetRange(250.0f);
	mLight->SetSpot(5.0f);
	mWorld->AddLight(mLight);

	mLight2 = new Light;
	mLight2->SetMaterials(Colors::White*0.1f, Colors::White*0.7f, Colors::White*0.0f);
	mLight2->SetDirection(0.5f, -0.5f, 0.5f);
	mLight2->SetType(DIRECTIONAL_LIGHT);
	mWorld->AddLight(mLight2);
}
	
void Game::Update(float dt)
{
	gInput->Update(dt);
	GetGraphics()->Update(dt);

	static float speed = 0.005;
	if(gInput->KeyDown('1'))
		mLight->SetPosition(mLight->GetPosition() + XMFLOAT3(speed, 0, 0));
	else if(gInput->KeyDown('2'))
		mLight->SetPosition(mLight->GetPosition() + XMFLOAT3(-speed, 0, 0));
	if(gInput->KeyDown('3'))
		mLight->SetPosition(mLight->GetPosition() + XMFLOAT3(0, 0, speed));
	else if(gInput->KeyDown('4'))
		mLight->SetPosition(mLight->GetPosition() + XMFLOAT3(0, 0, -speed));
	if(gInput->KeyDown('5'))
		mLight->SetPosition(mLight->GetPosition() + XMFLOAT3(0, speed, 0));
	else if(gInput->KeyDown('6'))
		mLight->SetPosition(mLight->GetPosition() + XMFLOAT3(0, -speed, 0));

	if(gInput->KeyDown('Z'))
		mLight2->SetDirection(mLight2->GetDirection() + XMFLOAT3(0, 0, 0.001));
	else if(gInput->KeyDown('X'))
		mLight2->SetDirection(mLight2->GetDirection() + XMFLOAT3(0, -0, -0.001));
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