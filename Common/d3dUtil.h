#pragma once

#include <d3dx11.h>
#include <dxerr.h>
#include <assert.h>
#include <Windows.h>
#include <xnamath.h>
#include <D3DX10math.h>

// Overloaded operators to make things smoother.
XMFLOAT3 operator+(const XMFLOAT3 a, const XMFLOAT3 b);
XMFLOAT3 operator-(const XMFLOAT3 a, const XMFLOAT3 b);

XMFLOAT3 operator+=(const XMFLOAT3 a, const XMVECTOR b);
XMFLOAT3 operator-=(const XMFLOAT3 a, const XMVECTOR b);

// Returns the inverse transpose.
XMMATRIX InverseTranspose(CXMMATRIX M);

// Debug macro that catches HRESULT errors.
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, #x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 

// Macro for releasing COM objects.
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

// A ninja define to avoid linking problems.
#define GLOBALCONST extern CONST __declspec(selectany)

// Convenient storage of often used colors.
namespace Colors
{
	GLOBALCONST XMVECTORF32 White     = {1.0f, 1.0f, 1.0f, 1.0f};
	GLOBALCONST XMVECTORF32 Black     = {0.0f, 0.0f, 0.0f, 1.0f};
	GLOBALCONST XMVECTORF32 Red       = {1.0f, 0.0f, 0.0f, 1.0f};
	GLOBALCONST XMVECTORF32 Green     = {0.0f, 1.0f, 0.0f, 1.0f};
	GLOBALCONST XMVECTORF32 Blue      = {0.0f, 0.0f, 1.0f, 1.0f};
	GLOBALCONST XMVECTORF32 Yellow    = {1.0f, 1.0f, 0.0f, 1.0f};
	GLOBALCONST XMVECTORF32 Cyan      = {0.0f, 1.0f, 1.0f, 1.0f};
	GLOBALCONST XMVECTORF32 Magenta   = {1.0f, 0.0f, 1.0f, 1.0f};
	GLOBALCONST XMVECTORF32 Silver    = {0.75f, 0.75f, 0.75f, 1.0f};
	GLOBALCONST XMVECTORF32 LightSteelBlue = {0.69f, 0.77f, 0.87f, 1.0f};
}