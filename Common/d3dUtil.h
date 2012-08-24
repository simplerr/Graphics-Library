#pragma once

#include <d3dx11.h>
#include <dxerr.h>
#include <assert.h>
#include <xnamath.h>
#include <D3DX10math.h>

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
	GLOBALCONST D3DXCOLOR White(1.0f, 1.0f, 1.0f, 1.0f);
	GLOBALCONST D3DXCOLOR Black(0.0f, 0.0f, 0.0f, 1.0f);
	GLOBALCONST D3DXCOLOR Red(1.0f, 0.0f, 0.0f, 1.0f);
	GLOBALCONST D3DXCOLOR Green(0.0f, 1.0f, 0.0f, 1.0f);
	GLOBALCONST D3DXCOLOR Blue(0.0f, 0.0f, 1.0f, 1.0f);
	GLOBALCONST D3DXCOLOR Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	GLOBALCONST D3DXCOLOR Cyan(0.0f, 1.0f, 1.0f, 1.0f);
	GLOBALCONST D3DXCOLOR Magenta(1.0f, 0.0f, 1.0f, 1.0f);
	GLOBALCONST D3DXCOLOR Silver(0.75f, 0.75f, 0.75f, 1.0f);
	GLOBALCONST D3DXCOLOR LightSteelBlue(0.69f, 0.77f, 0.87f, 1.0f);
}