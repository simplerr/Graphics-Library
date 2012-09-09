#pragma once

#include <d3dx11.h>
#include <dxerr.h>
#include <assert.h>
#include <Windows.h>
#include <vector>
#include <xnamath.h>
#include <D3DX10math.h>
#include "xnacollision.h"

class Light;
using namespace XNA;

// Typedef for convenience
typedef std::vector<Light*> LightList;

// Overloaded operators to make things smoother.
XMFLOAT3 operator+(const XMFLOAT3 a, const XMFLOAT3 b);
XMFLOAT3 operator-(const XMFLOAT3 a, const XMFLOAT3 b);

XMFLOAT3 operator+=(const XMFLOAT3 a, const XMVECTOR b);
XMFLOAT3 operator-=(const XMFLOAT3 a, const XMVECTOR b);

XMFLOAT2 operator+(const XMFLOAT2 a, const XMFLOAT2 b);
XMFLOAT2 operator-(const XMFLOAT2 a, const XMFLOAT2 b);

XMFLOAT4 operator*(const XMFLOAT4 a, const float b);

// Returns the inverse transpose.
XMMATRIX InverseTranspose(CXMMATRIX M);

// Transform an axis aligned box by an angle preserving transform.
// Custom version of the XNA::TransformAxisAlignedBox.
VOID TransformAxisAlignedBoxCustom(AxisAlignedBox* pOut, const AxisAlignedBox* pIn, FXMVECTOR Scale, FXMVECTOR Rotation, FXMVECTOR Translation);

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
	GLOBALCONST XMFLOAT4 White(1.0f, 1.0f, 1.0f, 1.0f);
	GLOBALCONST XMFLOAT4 Black(0.0f, 0.0f, 0.0f, 1.0f);
	GLOBALCONST XMFLOAT4 Red(1.0f, 0.0f, 0.0f, 1.0f);
	GLOBALCONST XMFLOAT4 Green(0.0f, 1.0f, 0.0f, 1.0f);
	GLOBALCONST XMFLOAT4 Blue(0.0f, 0.0f, 1.0f, 1.0f);
	GLOBALCONST XMFLOAT4 Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	GLOBALCONST XMFLOAT4 Cyan(0.0f, 1.0f, 1.0f, 1.0f);
	GLOBALCONST XMFLOAT4 Magenta(1.0f, 0.0f, 1.0f, 1.0f);
	GLOBALCONST XMFLOAT4 Silver(0.75f, 0.75f, 0.75f, 1.0f);
	GLOBALCONST XMFLOAT4 LightSteelBlue(0.69f, 0.77f, 0.87f, 1.0f);
}