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
	mTerrain = new Object3D();

	mTerrain->SetPrimitive(gPrimitiveFactory->CreateGrid(160.0f, 160.0f, 50, 50));
	mTerrain->LoadTexture("textures/grass.png", 15.0f);
	mTerrain->LoadNormalMap("textures/grass_nmap.png");

	mWorld->AddObject(mTerrain);
	
	mObject = new Object3D();

	// Load the effect and the primitive.
	mObject->SetPrimitive(gPrimitiveFactory->CreateBox());
	mObject->LoadTexture("textures/crate.dds");
	mObject->LoadNormalMap("textures/crate_nmap.dds");
	mObject->SetPosition(XMFLOAT3(0, 15, 0));
	mObject->SetScale(XMFLOAT3(1, 10, 10));
	mObject->SetRotation(XMFLOAT3(0.2f, 0.2f, 0.2f));

	mWorld->AddObject(mObject);

	mObject2 = new Object3D();

	// Load the effect and the primitive.
	mObject2->SetPrimitive(gPrimitiveFactory->CreateBox());
	mObject2->LoadTexture("textures/crate.dds");
	mObject2->SetPosition(XMFLOAT3(0, 15, 30));
	mObject2->SetScale(XMFLOAT3(3, 7, 5));
	mObject2->SetRotation(XMFLOAT3(0, 0, 0.2f));

	mWorld->AddObject(mObject2);

	// Add some lights.
	mLight = new Light();
	mLight->SetMaterials(Colors::White*0.3f, Colors::White*0.6f, Colors::White*0.0f);
	mLight->SetDirection(0.5f, -0.5f, 0.5f);
	mLight->SetType(DIRECTIONAL_LIGHT);
	mLight->SetPosition(0, 5, 5);
	mWorld->AddLight(mLight);

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

	// Testing...
	mRenderTarget	= new RenderTarget(GetGraphics(), 256, 256);
	mPrimitive		= gPrimitiveFactory->CreateQuad();
	//mObject->SetTexture(mRenderTarget->GetRenderTargetTexture());

	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GetGraphics()->GetContext()->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
}
	
void Game::Update(float dt)
{
	gInput->Update(dt);
	GetGraphics()->Update(dt);

	/*Camera* camera = GetGraphics()->GetCamera();
	mLight->SetPosition(camera->GetPosition());
	mLight->SetDirection(camera->GetDirection());*/

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

	/*static char buffer[16];
	sprintf(buffer, "Inside frustum: %i", mWorld->GetVisibleObjects());
	SetWindowText(GetHwnd(), buffer);*/

	// Collision?
	/*if(XNA::IntersectAxisAlignedBoxAxisAlignedBox(&mObject->GetBoundingBox(), &mObject2->GetBoundingBox()))
		SetWindowText(GetHwnd(), "Collision!");
	else
		SetWindowText(GetHwnd(), "No collision!");*/
	
	// Frustum test
	/*Frustum frustum = GetGraphics()->GetCamera()->GetFrustum();
	if(XNA::IntersectAxisAlignedBoxFrustum(&mObject->GetBoundingBox(), &frustum))
		SetWindowText(GetHwnd(), "Inside frustum!");
	else
		SetWindowText(GetHwnd(), "Outside frustum!");*/

	/*Ray ray = gInput->GetWorldPickingRay();
	float dist;
	if(IntersectRayAxisAlignedBox(XMLoadFloat3(&ray.origin), XMLoadFloat3(&ray.direction), &mObject->GetBoundingBox(), &dist)) {
		char buffer[256];
		sprintf(buffer, "Intersect! Dist = %f", dist);
		SetWindowText(GetHwnd(), buffer);
	}
	else
		SetWindowText(GetHwnd(), "No intersection!");*/
}
	
void Game::Draw(Graphics* pGraphics)
{
	pGraphics->ClearScene();

	// Unbind the SRVS from the pipeline so they can be used as DSV instead.
	ID3D11ShaderResourceView *const nullSRV[3] = {NULL, NULL, NULL};
	pGraphics->GetContext()->PSSetShaderResources(0, 3, nullSRV);

	// Draw depth values to the shadow map.
	pGraphics->FillShadowMap(mWorld->GetObjects());

	// Draw all objects.
	mWorld->Draw(pGraphics);
	pGraphics->DrawBillboards();
	pGraphics->DrawSkyBox();

	// Draw the blur texture.
	//pGraphics->ApplyBlur(mRenderTarget->GetRenderTargetTexture(), 4);
	/*Texture2D tex;
	tex.shaderResourceView = pGraphics->GetShadowMap()->GetSRV();
	pGraphics->DrawScreenQuad(&tex, 400, 300, 256, 256);*/

	// Present the backbuffer.
	pGraphics->Present();
}

LRESULT Game::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	gInput->MsgProc(msg, wParam, lParam);

	return Runnable::MsgProc(hwnd, msg, wParam, lParam);
}