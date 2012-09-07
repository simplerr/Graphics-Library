#include <crtdbg.h>
#include <assert.h>
#include <time.h>
#include "Game.h"
#include "Common\D3DCore.h"
#include "Common\d3dUtil.h"
#include "Common\Graphics.h"
#include "Common\PrimitiveFactory.h"
#include "Common\Object3D.h"
#include "Common\Input.h"
#include "Common\World.h"
#include "Common\Light.h"
#include "Common\BillboardManager.h"
#include "Common\Vertex.h"
#include "Common\RenderTarget.h"
#include "Common\Camera.h"
#include "Common\Effects.h"

// Set globals to nullptrs
Runnable*			gGame				= nullptr;
PrimitiveFactory*	gPrimitiveFactory	= nullptr;
Input*				gInput				= nullptr;

BillboardVertex* billboard;

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Game game(hInstance, "Graphics Library", 800, 600);
	gGame = &game;

	// Create the primitive factory.
	gPrimitiveFactory = new PrimitiveFactory();

	// Init the app.
	game.Init();

	// Create the input class.
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

	// Create the world.
	mWorld = new World();
	mWorld->Init();

	// Connect the graphics light list to the one in World.
	GetGraphics()->SetLightList(mWorld->GetLights());

	// Add some objects.
	Object3D* object = new Object3D();

	// Load the effect and the primitive.
	object->SetPrimitive(gPrimitiveFactory->CreateBox());
	object->SetTexture("textures/crate.dds");
	object->SetPosition(XMFLOAT3(0, 3, 0));

	mWorld->AddObject(object);

	object = new Object3D();

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
	mLight2->SetMaterials(Colors::White*0.3f, Colors::White*0.8f, Colors::White*0.0f);
	mLight2->SetDirection(0.5f, -0.5f, 0.5f);
	mLight2->SetType(DIRECTIONAL_LIGHT);
	mWorld->AddLight(mLight2);

	GetGraphics()->SetFogColor(XMFLOAT4(1.0f, 0.2f, 0.8, 1.0f));
	
	// Add test billboards.
	billboard = GetGraphics()->AddBillboard(XMFLOAT3(0, 10, 0), XMFLOAT2(5, 5), "textures\\crate.dds");
	srand(time(0));
	/*for(int i = 0; i < 500; i++) {
		XMFLOAT3 pos(rand() % 50 - 20, rand() % 30, rand() % 50 - 25);
		XMFLOAT2 size(rand() % 5 + 0.5f, rand() % 5 + 0.5f);
		GetGraphics()->AddBillboard(pos, size, "textures\\grass.png");
	}

	for(int i = 0; i < 500; i++) {
		XMFLOAT3 pos(rand() % 50 - 20 - 50, rand() % 30, rand() % 50 - 25);
		XMFLOAT2 size(rand() % 5 + 0.5f, rand() % 5 + 0.5f);
		GetGraphics()->AddBillboard(pos, size, "textures\\crate.dds");
	}*/

	mTexture2D = new Texture2D();
	mRenderTarget = new RenderTarget(GetGraphics(), 256, 256);
	mTexture2D->texture = mRenderTarget->GetShaderResourceView();
	mPrimitive = gPrimitiveFactory->CreateQuad();
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

	if(gInput->KeyDown('R')) {
		billboard->SetSize(billboard->Size + XMFLOAT2(0.01, 0.01)),
		billboard->SetPos(billboard->Pos + XMFLOAT3(0.05, 0, 0));
	}
	else if(gInput->KeyDown('T')) {
		billboard->SetPos(billboard->Pos - XMFLOAT3(0.05, 0, 0));
		billboard->SetSize(billboard->Size - XMFLOAT2(0.01, 0));
	}
}
	
void Game::Draw(Graphics* pGraphics)
{
	pGraphics->ClearScene();

	/**
		Draw everything to a texture.
	*/
	pGraphics->SetRenderTarget(mRenderTarget);
	XMFLOAT3 oldPos = GetGraphics()->GetCamera()->GetPosition();
	GetGraphics()->GetCamera()->SetPosition(XMFLOAT3(0, 20, 0));

	// Draw all objects.
	mWorld->Draw(pGraphics);
	pGraphics->DrawBillboards();

	// Restore the render target.
	pGraphics->SetRenderTarget(GetD3D()->GetRenderTargetView(), GetD3D()->GetDepthStencilView());

	GetGraphics()->GetCamera()->SetPosition(oldPos);

	// Draw all objects.
	mWorld->Draw(pGraphics);
	pGraphics->DrawBillboards();

	// Draw the texture.
	pGraphics->DrawScreenQuad(mTexture2D, 210, 210, 400, 400);

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