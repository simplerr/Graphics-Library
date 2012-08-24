#include "Graphics.h"
#include "d3dUtil.h"
#include "D3DCore.h"
#include "Runnable.h"
#include <d3dx10math.h>

//! Constructor. The Init() function handles the initialization.
Graphics::Graphics()
{
	// Set default values.
	mD3DCore = nullptr;
}
	
//! Cleans up and frees all pointers.
Graphics::~Graphics()
{
	// Cleanup.
	delete mD3DCore;
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
}

//! Clears the backbuffer with the color "color".
void Graphics::ClearScene(D3DXCOLOR color)
{
	GetD3D()->GetContext()->ClearRenderTargetView(GetD3D()->GetRenderTargetView(), reinterpret_cast<const float*>(&color));
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