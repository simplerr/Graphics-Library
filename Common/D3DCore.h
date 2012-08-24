#pragma once

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//////////////
// INCLUDES //
//////////////
#include <Windows.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

/** 
	Contains the core members of Direct3D.
	Handles initialization of Direct3D. 
*/
class D3DCore
{
public:
	D3DCore();
	~D3DCore();

	bool Init(int clientWidth, int clientHeight, HWND hwnd, bool fullscreen);
	void OnResize();

	HRESULT Present(UINT SyncInterval, UINT Flags);

	// Getters.
	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*	GetContext();
	IDXGISwapChain*			GetSwapChain();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();

private:
	// Core Direct3D members.
	ID3D11Device*			md3dDevice;
	ID3D11DeviceContext*	md3dImmediateContext;
	IDXGISwapChain*			mSwapChain;
	ID3D11Texture2D*		mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT			mScreenViewport;
	UINT					m4xMsaaQuality;
	bool					mEnable4xMsaa;
	int						mClientWidth, mClientHeight;

};	// Class