#include "Graphics.h"
#include "d3dUtil.h"
#include "D3DCore.h"
#include "Runnable.h"
#include <d3dx10math.h>
#include <tchar.h>
#include "d3dx11effect.h"
#include "Effect.h"
#include "EffectManager.h"
#include "../Primitive.h"
#include "Camera.h"
#include "LightHelper.h"

//! Constructor. The Init() function handles the initialization.
Graphics::Graphics()
{
	// Set default values.
	mD3DCore = nullptr;
	mEffectManager = nullptr;
	mCamera = nullptr;
}
	
//! Cleans up and frees all pointers.
Graphics::~Graphics()
{
	// Cleanup.
	delete mD3DCore;
	delete mEffectManager;
	delete mCamera;
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

	// Create the effect manager
	mEffectManager = new EffectManager();

	// Create the camera.
	mCamera = new Camera();

	// Initialize Direct3D.
	if(!mD3DCore->Init(clientWidth, clientHeight, hwnd, fullscreen)) {
		MessageBox(0, "Failed initializing Direct3D", "Error", MB_ICONEXCLAMATION);
		return false;
	}

	// Move to camera...
	XMVECTOR pos    = XMVectorSet(6, 6, 6, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f*3.14f, (float)clientWidth/(float)clientHeight, 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, proj);

	// Directional light.
	mLight.Ambient  = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mLight.Diffuse  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mMaterial.Ambient  = XMFLOAT4(1.0f, 0.42f, 0.556f, 1.0f);
	mMaterial.Diffuse  = XMFLOAT4(1.0f, 0.42f, 0.556f, 1.0f);
	mMaterial.Specular = XMFLOAT4(1.0f, 0.8f, 0.8f, 96.0f);
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
void Graphics::DrawPrimitive(Primitive* primitive, CXMMATRIX worldMatrix, Material material, Effect* effect)
{
	ID3D11DeviceContext* context = GetD3D()->GetContext();

	// Set the input layout and the primitive topology.
	context->IASetInputLayout(effect->GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the effect parameters.
	SetEffectParameters(effect, worldMatrix, mMaterial);

	// Draw the primitive.
	primitive->Draw(context);
}

void Graphics::SetEffectParameters(Effect* effect, CXMMATRIX worldMatrix, Material material)
{
	// Set the world * view * proj matrix.
	XMMATRIX view = XMLoadFloat4x4(&mCamera->GetViewMatrix());
	XMMATRIX proj = XMLoadFloat4x4(&mCamera->GetProjectionMatrix());
	effect->SetWorldViewProj(worldMatrix * view * proj);
	effect->SetWorld(worldMatrix);
	effect->SetWorldInvTranspose(InverseTranspose(worldMatrix));
	effect->SetEyePosition(XMLoadFloat3(&mCamera->GetPosition()));
	effect->SetMaterial(material);
	effect->SetDirectionalLight(mLight);

	effect->Apply();
}

void Graphics::DrawText(string text, int x, int y, D3DXCOLOR textColor, int size)
{
	RECT pos = {x, y, 0, 0};
	mFont->DrawText(0, text.c_str(), -1, &pos, DT_NOCLIP, textColor);
}

//! Clears the backbuffer with the color "color".
void Graphics::ClearScene()
{
	GetD3D()->GetContext()->ClearRenderTargetView(GetD3D()->GetRenderTargetView(), reinterpret_cast<const float*>(&Colors::White));
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

Effect* Graphics::LoadEffect(string filename, string technique)
{
	Effect* effect = mEffectManager->LoadEffect(filename, technique);
	effect->Init();
	return effect;
}
