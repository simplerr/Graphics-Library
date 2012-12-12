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
#include "..\Object3D.h"
#include "..\StaticObject.h"
#include "Terrain.h"
#include "StaticMesh.h"
#include "StaticModel.h"
#include "PrimitiveFactory.h"
#include "ModelImporter.h"
#include "RenderStates.h"

//! Graphics Library namespace.
namespace GLib
{

//! Constructor. The Init() function handles the initialization.
Graphics::Graphics()
{
	// Set default values.
	mD3DCore = nullptr;
	mCamera = nullptr;
	mRenderingShadows = false;
	mLightList = nullptr;

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
	delete mPrimitiveFactory;
	delete mModelImporter;

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

	// Cleanup fonts.
	mFontFactory->Release();
	mFontWrapper->Release();
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
	Effects::InitAll(GetDevice());
	RenderStates::InitAll(GetDevice());

	// Create the blur filter.
	mBlurFilter = new BlurFilter();

	// Create the primitive factory and the model importer.
	mPrimitiveFactory = new PrimitiveFactory();	
	mModelImporter = new ModelImporter(mPrimitiveFactory);

	// Create the primtive used when drawing 2D screen quads.
	mScreenQuad = mPrimitiveFactory->CreateQuad();

	mAABB = mPrimitiveFactory->CreateBox();

	mShadowMap = new ShadowMap(GetDevice(), 2048, 2048);

	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GetContext()->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);

	Effects::BasicFX->SetUseLighting(true);

	// Create the font wrapper.
	FW1CreateFactory(FW1_VERSION, &mFontFactory);
	mFontFactory->CreateFontWrapper(GetD3DDevice(), L"Arial", &mFontWrapper);
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
		if(FAILED(D3DX11CreateShaderResourceViewFromFile(GetD3D()->GetDevice(), filename.c_str(), 0, 0, &texture2d->shaderResourceView, 0))) {
			char buffer[246];
			sprintf(buffer, "Error loading texture: %s", filename.c_str());
		}

		texture2d->scale = scale;
		texture2d->name = filename;
		mTextureMap[textureId] = texture2d;
		return mTextureMap[textureId];
	}
}

//! Updates the camera and builds the shadow map.
void Graphics::Update(Input* pInput, float dt)
{
	mCamera->Update(pInput, dt);

	if(mLightList->size() != 0)
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
	primitive->Draw<Vertex>(context);
}

//! Draws all the billboards.
void Graphics::DrawBillboards()
{
	// Set topology and input layout.
	ID3D11DeviceContext* context = GetD3D()->GetContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetInputLayout(Effects::BillboardFX->GetInputLayout());

	// Remove billboard managers with no billboards.
	for(auto iter = mBillboardManagerMap.begin(); iter != mBillboardManagerMap.end(); iter++)
	{
		if((*iter).second->GetNumBillboards() == 0) {
			mBillboardManagerMap.erase(iter);
			break;
		}
	}

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
		Effects::BillboardFX->SetFogStart(2015.0f);
		Effects::BillboardFX->SetFogRange(175.0f);
		Effects::BillboardFX->SetMaterial(Colors::White);
		Effects::BillboardFX->SetTexture(manager->GetTexture());
		Effects::BillboardFX->Apply(GetD3DContext());

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
	//SetEffectParameters(Effects::BasicFX, S*T, texture, 0, Material(Colors::White));

	// Make sure to not use the view and projection matrices when dealing with NDC coordinates.
	// Set the world * view * proj matrix.
	XMMATRIX view = XMLoadFloat4x4(&mCamera->GetViewMatrix());
	XMMATRIX proj = XMLoadFloat4x4(&mCamera->GetProjectionMatrix());
	Effects::ScreenFX->SetWorld(S*T);
	Effects::ScreenFX->SetWorldInvTranspose(InverseTranspose(S*T));
	Effects::ScreenFX->SetEyePosition(mCamera->GetPosition());
	Effects::ScreenFX->SetTexture(texture);
	Effects::ScreenFX->SetWorldViewProj(S*T);
	Effects::ScreenFX->Apply(GetD3DContext());

	// Draw the primitive.
	mScreenQuad->Draw<Vertex>(context);
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

	// Fog
	effect->SetFogColor(mFogColor);
	effect->SetFogStart(1000.0f);
	effect->SetFogRange(50.0f);

	effect->Apply(GetD3DContext());
}

void Graphics::ApplyBlur(Texture2D* texture, int blurCount)
{
	mBlurFilter->ApplyBlur(GetDevice(), GetContext(), texture->shaderResourceView, blurCount);
}

//! Draws text to the screen.
void Graphics::DrawText(string text, int x, int y, int size,  UINT32 textColor)
{
	std::wstring wsTmp(text.begin(), text.end());
	mFontWrapper->DrawString(GetD3DContext(), wsTmp.c_str(), size, x, y, textColor, 0);
}

//! Adds a billboard to the scene.
BillboardVertex* Graphics::AddBillboard(XMFLOAT3 position, XMFLOAT2 size, string texture)
{
	// A billboard manager for the texture already exists.
	if(mBillboardManagerMap.find(texture) != mBillboardManagerMap.end()) {
		BillboardVertex* billboard = new BillboardVertex(position, size);
		billboard->Manager = mBillboardManagerMap[texture];
		mBillboardManagerMap[texture]->AddBillboard(this, billboard);
		return billboard;
	}
	else {
		mBillboardManagerMap[texture] = new BillboardManager(this, texture);
		BillboardVertex* billboard = new BillboardVertex(position, size);
		billboard->Manager = mBillboardManagerMap[texture];
		mBillboardManagerMap[texture]->AddBillboard(this, billboard);
		return billboard;
	}

	return nullptr;
}

void Graphics::RemoveBillboard(string texture, BillboardVertex* pBillboard)
{
	if(mBillboardManagerMap.find(texture) != mBillboardManagerMap.end()) 
		mBillboardManagerMap[texture]->RemoveBillbaord(pBillboard);
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

void Graphics::ActiveShadowMap()
{
	// Unbind the SRVs from the pipeline so they can be used as DSVs instead.
	ID3D11ShaderResourceView *const nullSRV[1] = {NULL};//, NULL, NULL, NULL};
	GetContext()->PSSetShaderResources(1, 1, nullSRV);
	Effects::BuildShadowMapFX->Apply(GetD3DContext());

	// Sets render target to NULL and the DSV to the shadow maps. Enables depth writes to the DSV basicly.
	GetShadowMap()->BindDepthStencil(GetContext());
	Effects::BasicFX->SetRenderingToShadowMap(true);
	Effects::BasicFX->Apply(GetD3DContext());
	mRenderingShadows = true;
}

void Graphics::DeactiveShadowMap()
{
	// Restore the rasterizer state.
	GetContext()->RSSetState(0);

	// Restore the render targets and viewport.
	RestoreRenderTarget();
	GetContext()->RSSetViewports(1, &GetD3D()->GetViewport());

	Effects::BasicFX->SetRenderingToShadowMap(false);
	mRenderingShadows = false;
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

//! Sets the camera to use.
void Graphics::SetCamera(Camera* pCamera)
{
	mCamera = pCamera;
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

bool Graphics::IsRenderingShadows()
{
	return mRenderingShadows;
}

PrimitiveFactory* Graphics::GetPrimitiveFactory()
{
	return mPrimitiveFactory;
}

ModelImporter* Graphics::GetModelImporter()
{
	return mModelImporter;
}

}	// End of Graphics Library namespace.