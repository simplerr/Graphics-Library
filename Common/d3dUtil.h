#pragma once

#include <d3dx11.h>
#include <D3D11.h>
#include <dxerr.h>
#include <assert.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <xnamath.h>
#include <D3DX10math.h>
#include "xnacollision.h"
#include <assimp\Importer.hpp>

using namespace XNA;
using namespace std;

class Object3D;

// Overloaded operators to make things smoother.
XMFLOAT3 operator+(const XMFLOAT3 a, const XMFLOAT3 b);
XMFLOAT3 operator-(const XMFLOAT3 a, const XMFLOAT3 b);

XMFLOAT3 operator+=(const XMFLOAT3 a, const XMVECTOR b);
XMFLOAT3 operator-=(const XMFLOAT3 a, const XMVECTOR b);

XMFLOAT2 operator+(const XMFLOAT2 a, const XMFLOAT2 b);
XMFLOAT2 operator-(const XMFLOAT2 a, const XMFLOAT2 b);

XMFLOAT4 operator*(const XMFLOAT4 a, const float b);
XMFLOAT3 operator*(const XMFLOAT3 a, const float b);

//! Graphics Library namespace.
namespace GLib
{
	// Forward declarations.
	class Light;
	class Graphics;
	class Camera;

	ID3D11Device*			GetD3DDevice();
	ID3D11DeviceContext*	GetD3DContext();
	Graphics*				GetGraphics();
	Camera*					GetCamera();
	HINSTANCE				GetWindowInstance();
	HWND					GetWindowHandler();
	int						GetClientWidth();
	int						GetClientHeight();

	struct Ray
	{
		XMFLOAT3 origin;
		XMFLOAT3 direction;
	};

	struct Dimension
	{
		Dimension(){};
		Dimension(float w, float h) {
			width = w;
			height = h;
		}

		float width;
		float height;
	};

	struct BoundingSphere
	{
		BoundingSphere() : center(0.0f, 0.0f, 0.0f), radius(0.0f) {}
		XMFLOAT3 center;
		float radius;
	};

	class d3dHelper
	{
	public:
		//! Loads and creates a SRV to a 2d texture array.
		static ID3D11ShaderResourceView* CreateTexture2DArraySRV(ID3D11Device* device, ID3D11DeviceContext* context, vector<string>& filenames);
	};

	// Typedef for convenience
	typedef std::vector<Light*> LightList;

	XMFLOAT4X4 ToXMFloat4X4(aiMatrix4x4 ai);

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
}