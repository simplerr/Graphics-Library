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
#include "RenderTarget.h"
#include "PrimitiveFactory.h"
#include "BlurFilter.h"
#include "RenderStates.h"
#include "Sky.h"
#include "ShadowMap.h"
#include "Object3D.h"
#include "Terrain.h"
#include "Mesh.h"
#include "Model.h"

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
	delete mBlurFilter;
	delete mShadowMap;

	// Release and delete the textures.
	for(auto iter = mTextureMap.begin(); iter != mTextureMap.end(); iter++)
	{
		ReleaseCOM((*iter).second->shaderResourceView);
		delete (*iter).second;
	}

	// Cleanup the billboards
	for(auto iter = mBillboardManagerMap.begin(); iter!= mBillboardManagerMap.end(); iter++)
		delete (*iter).second;

	Effects::DestroyAll();
	RenderStates::DestroyAll();
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
	RenderStates::InitAll(GetDevice());

	// Create the camera.
	mCamera = new Camera();

	// Create the blur filter.
	mBlurFilter = new BlurFilter();

	mMaterial = Material(Colors::LightSteelBlue);

	// Create the primtive used when drawing 2D screen quads.
	mScreenQuad = gPrimitiveFactory->CreateQuad();

	mAABB = gPrimitiveFactory->CreateBox();

	mShadowMap = new ShadowMap(GetDevice(), 2048, 2048);
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
	else // Not already loaded.
	{
		Texture2D* texture2d = new Texture2D();
		HR(D3DX11CreateShaderResourceViewFromFile(GetD3D()->GetDevice(), filename.c_str(), 0, 0, &texture2d->shaderResourceView, 0));
		texture2d->scale = scale;
		mTextureMap[textureId] = texture2d;
		return mTextureMap[textureId];
	}
}

//! Updates the camera.
void Graphics::Update(float dt)
{
	mCamera->Update(dt);
	mShadowMap->BuildShadowTransform(mLightList->operator[](0));
}

//! Draws a primitive.
/**
@param primitive the primitive containing the buffers to draw
@param worldMatrix the primitives world transform matrix
@param effect the effect to use when rendering the primitive
*/
void Graphics::DrawPrimitive(Primitive* primitive, CXMMATRIX worldMatrix, Texture2D* texture, Texture2D* normalMap, Material material, BasicEffect* effect)
{
	ID3D11DeviceContext* context = GetD3D()->GetContext();

	// Set the input layout and the primitive topology.
	context->IASetInputLayout(effect->GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the effect parameters.
	SetEffectParameters(effect, worldMatrix, texture, normalMap, material);

	// Draw the primitive.
	primitive->Draw(context);
}

//! Draws all the billboards.
void Graphics::DrawBillboards()
{
	// Set topology and input layout.
	ID3D11DeviceContext* context = GetD3D()->GetContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetInputLayout(Effects::BillboardFX->GetInputLayout());

	// Loop through all billboard managers.
	for(auto iter = mBillboardManagerMap.begin(); iter != mBillboardManagerMap.end(); iter++)
	{
		// Rebuild the vertex buffer if a billboard has moved or changed size.
		BillboardManager* manager = (*iter).second;
		if(manager->GetRebuild())
			manager->BuildVertexBuffer(GetD3D()->GetDevice());

		manager->SetRebuild(false);

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

		context->Draw(manager->GetNumBillboards(), 0);
	}
}

void Graphics::DrawScreenQuad(Texture2D* texture, float x, float y, float width, float height)
{
	ID3D11DeviceContext* context = GetD3D()->GetContext();

	// Set the input layout and the primitive topology.
	context->IASetInputLayout(Effects::BasicFX->GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the effect parameters.
	float transformedX = x - GetClientWidth()/2;
	float transformedY = -(y - GetClientHeight()/2);
	XMMATRIX T = XMMatrixTranslation(transformedX / GetClientWidth() * 2, transformedY / GetClientHeight() * 2, 0);
	XMMATRIX S = XMMatrixScaling(width / GetClientWidth(), height / GetClientHeight(), 1);
	SetEffectParameters(Effects::BasicFX, S*T, texture, 0, Material(XMFLOAT4(1, 1, 1, 1)));

	// Make sure to not use the view and projection matrices when dealing with NDC coordinates.
	Effects::BasicFX->SetWorldViewProj(S*T);
	Effects::BasicFX->Apply();

	// Draw the primitive.
	mScreenQuad->Draw(context);
}

void Graphics::DrawBoundingBox(AxisAlignedBox* aabb, CXMMATRIX worldMatrix, Material material, float transparency)
{
	// Calculate the scaling matrix.
	XMMATRIX scaleMatrix = XMMatrixScaling(aabb->Extents.x, aabb->Extents.y, aabb->Extents.z);

	// Ignore the rotation.
	XMMATRIX world = scaleMatrix * XMMatrixTranslation(aabb->Center.x, aabb->Center.y, aabb->Center.z);

	// Draw the primitive.
	material.diffuse.w = transparency;
	DrawPrimitive(mAABB, world, 0, 0, material, Effects::BasicFX);
}

//! Sets the effect parameters.
void Graphics::SetEffectParameters(BasicEffect* effect, CXMMATRIX worldMatrix, Texture2D* texture, Texture2D* normalMap, Material material)
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
	effect->SetNormalMap(normalMap);
	effect->SetShadowMap(mShadowMap->GetSRV());
	effect->SetShadowTransform(worldMatrix * XMLoadFloat4x4(&mShadowMap->GetShadowTransform()));
	
	if(gInput->KeyPressed('C')) {
		XMFLOAT3 pos = mCamera->GetPosition();
		char buffer[256];
		sprintf(buffer, "x: %f\n, y: %f\n, z: %f\n", pos.x, pos.y, pos.z);
		OutputDebugString(buffer);
	}

	// Fog
	effect->SetFogColor(mFogColor);
	effect->SetFogStart(1000.0f);
	effect->SetFogRange(50.0f);

	effect->Apply();
}

//! Renders depth values from the light space to the shadow map texture.
void Graphics::FillShadowMap(ObjectList* objects)
{
	// Sets render target to NULL and the DSV to the shadow maps. Enables depth writes to the DSV basicly.
	GetShadowMap()->BindDepthStencil(GetContext());

	XMMATRIX view = XMLoadFloat4x4(&mShadowMap->GetLightView());
	XMMATRIX proj = XMLoadFloat4x4(&mShadowMap->GetLightProj());
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);

	Effects::BuildShadowMapFX->SetViewProj(viewProj);

	// Set the input layout and the primitive topology.
	GetContext()->IASetInputLayout(Effects::BuildShadowMapFX->GetInputLayout());
	GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Loop through all objects.
	for(int i = 0; i < objects->size(); i++)
	{
		Object3D* object = objects->operator[](i);	
		XMMATRIX world = object->GetWorldMatrix();
		XMMATRIX worldInvTranspose = InverseTranspose(world);

		Effects::BuildShadowMapFX->SetWorld(world);
		Effects::BuildShadowMapFX->SetWorldViewProj(world * view * proj);
		Effects::BuildShadowMapFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BuildShadowMapFX->Apply();

		// Draw all the meshes.
		vector<Mesh*>* meshList = object->GetModel()->GetMeshList();
		for(int j = 0; j < meshList->size(); j++)
		{
			Mesh* mesh = meshList->operator[](i);	
			ID3D11Buffer* vb = mesh->GetVertexBuffer();
			ID3D11Buffer* ib = mesh->GetIndexBuffer();

			// Set the vertex and index buffers to the Input Assembler stage.
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			GetContext()->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
			GetContext()->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);	// [NOTE] Important with right format!! 16 or 32!

			// Draw the mesh.
			GetContext()->DrawIndexed(mesh->GetNumIndices(), 0, 0);
		}
	}

	// Restore the rasterizer state.
	GetContext()->RSSetState(0);

	// Restore the render targets and viewport.
	RestoreRenderTarget();
	GetContext()->RSSetViewports(1, &GetD3D()->GetViewport());
}

void Graphics::ApplyBlur(Texture2D* texture, int blurCount)
{
	mBlurFilter->ApplyBlur(GetContext(), texture->shaderResourceView, blurCount);
}

void Graphics::DrawText(string text, int x, int y, D3DXCOLOR textColor, int size)
{

}

//! Adds a billboard to the scene.
BillboardVertex* Graphics::AddBillboard(XMFLOAT3 position, XMFLOAT2 size, string texture)
{
	// A billboard manager for the texture already exists.
	if(mBillboardManagerMap.find(texture) != mBillboardManagerMap.end()) {
		BillboardVertex* billboard = new BillboardVertex(position, size);
		billboard->Manager = mBillboardManagerMap[texture];
		mBillboardManagerMap[texture]->AddBillboard(billboard);
		return billboard;
	}
	else {
		mBillboardManagerMap[texture] = new BillboardManager(texture);
		BillboardVertex* billboard = new BillboardVertex(position, size);
		billboard->Manager = mBillboardManagerMap[texture];
		mBillboardManagerMap[texture]->AddBillboard(billboard);
		return billboard;
	}

	return nullptr;
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

//! Returns the Direct3D device context.
ID3D11DeviceContext* Graphics::GetContext()
{
	return mD3DCore->GetContext();
}
	
//! Returns the 3D camera.
Camera*	Graphics::GetCamera()
{
	return mCamera;
}

ShadowMap* Graphics::GetShadowMap()
{
	return mShadowMap;
}

LightList* Graphics::GetLightList()
{
	return mLightList;
}

XMFLOAT4 Graphics::GetFogColor()
{
	return mFogColor;
}

//! Sets which render target to use.
void Graphics::SetRenderTarget(RenderTarget* renderTarget)
{
	ID3D11RenderTargetView* renderTargetView = renderTarget->GetRenderTargetView();
	ID3D11DepthStencilView* depthStencilView = renderTarget->GetDepthStencilView();
	GetContext()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	GetContext()->ClearRenderTargetView(renderTarget->GetRenderTargetView(), reinterpret_cast<const float*>(&mFogColor));
	GetContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//! Sets which render target to use.
void Graphics::RestoreRenderTarget()
{
	ID3D11RenderTargetView* renderTargetView = GetD3D()->GetRenderTargetView();
	ID3D11DepthStencilView* depthStencilView = GetD3D()->GetDepthStencilView();
	GetContext()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	GetContext()->ClearRenderTargetView(GetD3D()->GetRenderTargetView(), reinterpret_cast<const float*>(&mFogColor));
	GetContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//! Returns the Direct3D device.
ID3D11Device* Graphics::GetDevice()
{
	return mD3DCore->GetDevice();
}

//! Sets the light list.
void Graphics::SetLightList(LightList* lightList)
{
	mLightList = lightList;
}

//! Sets the fog color.
void Graphics::SetFogColor(XMFLOAT4 color)
{
	mFogColor = color;
}

float Graphics::GetClientWidth()
{
	return mD3DCore->GetClientWidth();
}
	
float Graphics::GetClientHeight()
{
	return mD3DCore->GetClientHeight();
}