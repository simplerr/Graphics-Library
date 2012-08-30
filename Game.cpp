#include <crtdbg.h>
#include <assert.h>
#include "Game.h"
#include "Common\D3DCore.h"
#include "Common\d3dUtil.h"
#include "Common\Graphics.h"
#include "Common\PrimitiveFactory.h"
#include "Common\Object3D.h"
#include "Common\Input.h"

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
}

void Game::Init()
{
	// Important to run Systems Init() function.
	Runnable::Init();

	gPrimitiveFactory = new PrimitiveFactory();

	// Test object.
	mObject = new Object3D();
	mObject2 = new Object3D();

	// Load the effect and the primitive.
	mObject->SetEffect(gGame->GetGraphics()->LoadEffect("Lighting.fx", "LightTech"));
	mObject->SetPrimitive(gPrimitiveFactory->CreateBox());
	mObject->SetPosition(XMFLOAT3(0, 3, 0));

	mObject2->SetEffect(gGame->GetGraphics()->LoadEffect("Lighting.fx", "LightTech"));
	mObject2->SetPrimitive(gPrimitiveFactory->CreateGrid(160.0f, 160.0f, 50, 50));
}
	
void Game::Update(float dt)
{
	gInput->Update(dt);
	GetGraphics()->Update(dt);
}
	
void Game::Draw(Graphics* pGraphics)
{
	pGraphics->ClearScene();

	mObject->Draw(pGraphics);
	mObject2->Draw(pGraphics);

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
		XMFLOAT3 rotation = mObject->GetRotation();
		rotation.x += 0.1;
		mObject->SetRotation(rotation);
		break;
		}
	case WM_RBUTTONDOWN: 
		{
		XMFLOAT3 scale = mObject->GetScale();
		scale.y += 0.1;
		mObject->SetScale(scale);
		break;
		}
	};

	return Runnable::MsgProc(hwnd, msg, wParam, lParam);
}