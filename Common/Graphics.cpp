#include "Graphics.h"
#include "d3dUtil.h"
#include "D3DCore.h"
#include "Runnable.h"
#include <d3dx10math.h>
#include <tchar.h>
#include "d3dx11effect.h"
#include "Effects.h"
#include "Primitive.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"
#include "Vertex.h"
#include "BillboardManager.h"

//! Constructor. The Init() function handles the initialization.
Graphics::Graphics()
{
	// Set default values.
	mD3DCore = nullptr;
	mCamera = nullptr;

	// Used for clear scene and fog.
	SetFogColor(Colors::Silver);
}
	
//! Cleans up and frees all pointers.
Graphics::~Graphics()
{
	// Cleanup.
	delete mD3DCore;
	delete mCamera;

	// Release and delete the textures.
	for(auto iter = mTextureMap.begin(); iter != mTextureMap.end(); iter++)
	{
		ReleaseCOM((*iter).second->texture);
		delete (*iter).second;
	}

	// Cleanup the billboards
	for(auto iter = mBillboardManagerMap.begin(); iter!= mBillboardManagerMap.end(); iter++)
		delete (*iter).second;

	Effects::DestroyAll();
}

//! Initializes Direct3D by calling D3DCore::Init(...).
/**
@param clientWidth the width of the window
@param clientHeight the height of the window
@param hwnd the HWND of the main window
@param fullscreen true if fullscreen
*/
bool Graphics::Init(int clientWidth, int clientHeight, HWND hwnd, bool fullscreen)
{
	// Create the D3DBase object.
	mD3DCore = new D3DCore();

	// Initialize Direct3D.
	if(!mD3DCore->Init(clientWidth, clientHeight, hwnd, fullscreen)) {
		MessageBox(0, "Failed initializing Direct3D", "Error", MB_ICONEXCLAMATION);
		return false;
	}

	//! Init all effects.
	Effects::InitAll();

	// Create the camera.
	mCamera = new Camera();

	mMaterial = Material(Colors::LightSteelBlue);
}

//! Returns the created texture. The Graphics class handles cleanup.
Texture2D* Graphics::LoadTexture(string filename, float scale)
{
	// A bit of a hax... [NOTE][TODO][HAX]
	char buffer[256];
	sprintf(buffer, "%s - %f", filename.c_str(), scale);
	string textureId = buffer;

	// Is the texture already loaded?
	if(mTextureMap.find(textureId) != mTextureMap.end()) {
		return mTextureMap[textureId];
	}
	else
	{
		Texture2D* texture2d = new Texture2D();
		HR(D3DX11CreateShaderResourceViewFromFile(GetD3D()->GetDevice(), filename.c_str(), 0, 0, &texture2d->texture, 0));
		texture2d->scale = scale;
		mTextureMap[textureId] = texture2d;
		return mTextureMap[textureId];
	}
}

void Graphics::Update(float dt)
{
	mCamera->Update(dt);
}

//! Draws a primitive.
/**
@param primitive the primitive containing the buffers to draw
@param worldMatrix the primitives world transform matrix
@param effect the effect to use when rendering the primitive
*/
void Graphics::DrawPrimitive(Primitive* primitive, CXMMATRIX worldMatrix, Texture2D* texture, Material material, BasicEffect* effect)
{
	ID3D11DeviceContext* context = GetD3D()->GetContext();

	// Set the input layout and the primitive topology.
	context->IASetInputLayout(effect->GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the effect parameters.
	SetEffectParameters(effect, worldMatrix, texture, material);

	// Draw the primitive.
	primitive->Draw(context);
}

void Graphics::DrawBillboards()
{
	// Set topology and input layout.
	ID3D11DeviceContext* context = GetD3D()->GetContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetInputLayout(Effects::BillboardFX->GetInputLayout());

	// Loop through all billboard managers.
	for(auto iter = mBillboardManagerMap.begin(); iter != mBillboardManagerMap.end(); iter++)
	{
		BillboardManager* manager = (*iter).second;
		ID3D11Buffer* vb = manager->GetVertexBuffer();
		// Set the vertex buffer.
		UINT stride = sizeof(BillboardVertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

		// Set effect variables.
		XMMATRIX view = XMLoadFloat4x4(&mCamera->GetViewMatrix());
		XMMATRIX proj = XMLoadFloat4x4(&mCamera->GetProjectionMatrix());
		Effects::BillboardFX->SetViewProj(view * proj);
		Effects::BillboardFX->SetLights(mLightList);
		Effects::BillboardFX->SetEyePosition(mCamera->GetPosition());
		Effects::BillboardFX->SetFogColor(mFogColor);
		Effects::BillboardFX->SetFogStart(15.0f);
		Effects::BillboardFX->SetFogRange(175.0f);
		Effects::BillboardFX->SetMaterial(Colors::White);
		Effects::BillboardFX->SetTexture(manager->GetTexture());
		Effects::BillboardFX->Apply();

		context->Draw(manager->GetNumVertices(), 0);
	}
}

void Graphics::SetEffectParameters(BasicEffect* effect, CXMMATRIX worldMatrix, Texture2D* texture, Material material)
{
	// Set the world * view * proj matrix.
	XMMATRIX view = XMLoadFloat4x4(&mCamera->GetViewMatrix());
	XMMATRIX proj = XMLoadFloat4x4(&mCamera->GetProjectionMatrix());
	effect->SetWorldViewProj(worldMatrix * view * proj);
	effect->SetWorld(worldMatrix);
	effect->SetWorldInvTranspose(InverseTranspose(worldMatrix));
	effect->SetEyePosition(mCamera->GetPosition());
	effect->SetMaterial(material);
	effect->SetLights(mLightList);
	effect->SetTexture(texture);
	
	if(gInput->KeyPressed('C')) {
		XMFLOAT3 pos = mCamera->GetPosition();
		char buffer[256];
		sprintf(buffer, "x: %f\n, y: %f\n, z: %f\n", pos.x, pos.y, pos.z);
		OutputDebugString(buffer);
	}

	// Fog
	effect->SetFogColor(mFogColor);
	effect->SetFogStart(100.0f);
	effect->SetFogRange(50.0f);

	effect->Apply();
}

void Graphics::DrawText(string text, int x, int y, D3DXCOLOR textColor, int size)
{

}

//! Adds a billboard to the scene.
void Graphics::AddBillboard(XMFLOAT3 position, XMFLOAT2 size, string texture)
{
	// A billboard manager for the texture already exists.
	if(mBillboardManagerMap.find(texture) != mBillboardManagerMap.end()) {
		BillboardVertex* billboard = new BillboardVertex(position, size);
		mBillboardManagerMap[texture]->AddBillboard(billboard);
	}
	else {
		mBillboardManagerMap[texture] = new BillboardManager(texture);
		BillboardVertex* billboard = new BillboardVertex(position, size);
		mBillboardManagerMap[texture]->AddBillboard(billboard);
	}
}

//! Clears the backbuffer with the color "color".
void Graphics::ClearScene()
{
	GetD3D()->GetContext()->ClearRenderTargetView(GetD3D()->GetRenderTargetView(), reinterpret_cast<const float*>(&mFogColor));
	GetD3D()->GetContext()->ClearDepthStencilView(GetD3D()->GetDepthStencilView(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//! Calls the D3DCores Present() function.
void Graphics::Present()
{
	mD3DCore->Present(0, 0);
}

//! Returns the D3DCore instance.
D3DCore* Graphics::GetD3D()
{
	return mD3DCore; 
}

void Graphics::SetLightList(LightList* lightList)
{
	mLightList = lightList;
}

void Graphics::SetFogColor(XMFLOAT4 color)
{
	mFogColor = color;
}