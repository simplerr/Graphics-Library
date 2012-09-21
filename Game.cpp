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
#include "Common\RenderStates.h"
#include "Common\ShadowMap.h"
#include "Common\ModelImporter.h"
#include "Common\Model.h"
#include "vld.h"

// Set globals to nullptrs
Runnable*			gGame				= nullptr;
PrimitiveFactory*	gPrimitiveFactory	= nullptr;
Input*				gInput				= nullptr;

BillboardVertex* billboard;

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	/*#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif*/

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
	delete mModelImporter;
}

void Game::Init()
{
	// Important to run Systems Init() function.
	Runnable::Init();

	// Create the world.
	mWorld = new World();
	mWorld->Init();

	// Create the model importer.
	mModelImporter = new ModelImporter(gPrimitiveFactory);

	// Connect the graphics light list to the one in World.
	GetGraphics()->SetLightList(mWorld->GetLights());

	// Add some objects.
	mObject = new Object3D(mModelImporter, "models/assassin/Crimson Assassin Athea.obj");
	mObject->SetPosition(XMFLOAT3(0, 15, 0));
	mObject->SetScale(XMFLOAT3(0.1, 0.1, 0.1));
	mWorld->AddObject(mObject);

	// Add some lights.
	mLight = new Light();
	mLight->SetMaterials(Colors::White*0.4f, Colors::White*1.0f, Colors::White*0.0f);
	mLight->SetDirection(0.5f, -0.5f, 0.5f);
	mLight->SetType(DIRECTIONAL_LIGHT);
	mLight->SetPosition(0, 5, 5);
	mWorld->AddLight(mLight);

	GetGraphics()->SetFogColor(XMFLOAT4(1.0f, 0.2f, 0.8, 1.0f));
	
	// Add test billboards.
	billboard = GetGraphics()->AddBillboard(XMFLOAT3(0, 10, 0), XMFLOAT2(5, 5), "textures\\crate.dds");

	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GetGraphics()->GetContext()->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
}
	
void Game::Update(float dt)
{
	gInput->Update(dt);
	GetGraphics()->Update(dt);
	mWorld->Update(dt);

	static float speed = 0.05;
	if(gInput->KeyDown('1'))
		mObject->SetPosition(mObject->GetPosition() + XMFLOAT3(speed, 0, 0));
	else if(gInput->KeyDown('2'))
		mObject->SetPosition(mObject->GetPosition() + XMFLOAT3(-speed, 0, 0));
	if(gInput->KeyDown('3'))
		mObject->SetPosition(mObject->GetPosition() + XMFLOAT3(0, 0, speed));
	else if(gInput->KeyDown('4'))
		mObject->SetPosition(mObject->GetPosition() + XMFLOAT3(0, 0, -speed));
	if(gInput->KeyDown('5'))
		mObject->SetPosition(mObject->GetPosition() + XMFLOAT3(0, speed, 0));
	else if(gInput->KeyDown('6'))
		mObject->SetPosition(mObject->GetPosition() + XMFLOAT3(0, -speed, 0));

	if(gInput->KeyDown('Z'))
		mLight->SetDirection(mLight->GetDirection() + XMFLOAT3(0, 0.0000, 0.0005));
	else if(gInput->KeyDown('X'))
		mLight->SetDirection(mLight->GetDirection() + XMFLOAT3(0, -0.0000, -0.0005));

	if(gInput->KeyDown(VK_LBUTTON)) {
		mObject->SetRotation(mObject->GetRotation() + XMFLOAT3(0.003, 0.003, 0.003));
	}
	else if(gInput->KeyDown(VK_RBUTTON)) {
		mObject->SetRotation(mObject->GetRotation() - XMFLOAT3(0.003, 0.003, 0.003));
	}

	if(gInput->KeyDown('V'))
		mObject->SetPosition(mObject->GetPosition() + XMFLOAT3(0, 0, 0.03));
	if(gInput->KeyDown('B'))
		mObject->SetPosition(mObject->GetPosition() - XMFLOAT3(0, 0, 0.03));
}
	
void Game::Draw(Graphics* pGraphics)
{
	// Clear the render target and depth/stencil.
	pGraphics->ClearScene();

	// Unbind the SRVs from the pipeline so they can be used as DSVs instead.
	ID3D11ShaderResourceView *const nullSRV[3] = {NULL, NULL, NULL};
	pGraphics->GetContext()->PSSetShaderResources(0, 3, nullSRV);

	// Draw depth values to the shadow map.
	pGraphics->FillShadowMap(mWorld->GetObjects());

	// Draw all objects.
	mWorld->Draw(pGraphics);
	pGraphics->DrawBillboards();

	// Present the backbuffer.
	pGraphics->Present();
}

LRESULT Game::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	gInput->MsgProc(msg, wParam, lParam);

	return Runnable::MsgProc(hwnd, msg, wParam, lParam);
}