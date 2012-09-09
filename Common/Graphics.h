#pragma once

#include <Windows.h>
#include <string>
#include <xnamath.h>
#include <vector>
#include <map>
#include <d3dx9.h>
#include "Light.h"
#include "d3dUtil.h"

using namespace std;

class D3DCore;
class BasicEffect;
class Primitive;
class EffectManager;
class BillboardManager;
class RenderTarget;
class Camera;
class BlurFilter;
struct Material;
struct Texture2D;
struct BillboardVertex;


//! Represents a 2D texture.
struct Texture2D
{
	Texture2D() {
		shaderResourceView = 0;
		scale = 1.0f;
	}

	ID3D11ShaderResourceView* shaderResourceView;
	float scale;
};

/**
	The user interface to the graphical components.
	Contains a D3DCore instance.
	This class and it's members handles everything that has to do with Direct3D.
*/
class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Init(int clientWidth, int clientHeight, HWND hwnd, bool fullscreen = false);
	void Update(float dt);

	void DrawPrimitive(Primitive* primitive, CXMMATRIX worldMatrix, Texture2D* texture, Material material, BasicEffect* effect);
	void DrawBillboards();
	void DrawScreenQuad(Texture2D* texture, float x, float y, float width, float height);
	void DrawText(string text, int x, int y, D3DXCOLOR textColor, int size);
	void ApplyBlur(Texture2D* texture, int blurCount);

	Texture2D* LoadTexture(string filename, float scale = 1.0f);
	BillboardVertex* AddBillboard(XMFLOAT3 position, XMFLOAT2 size, string texture);

	void ClearScene();
	void Present();

	void SetEffectParameters(BasicEffect* effect, CXMMATRIX worldMatrix, Texture2D* texture, Material material);
	void SetLightList(LightList* lightList);
	void SetFogColor(XMFLOAT4 color);
	void SetRenderTarget(RenderTarget* renderTarget);
	void RestoreRenderTarget();

	// Getters.
	ID3D11DeviceContext*	GetContext();
	ID3D11Device*			GetDevice();
	D3DCore*				GetD3D();
	Camera*					GetCamera();
	float					GetClientWidth();
	float					GetClientHeight();
private:
	map<string, Texture2D*>			mTextureMap;
	map<string, BillboardManager*>	mBillboardManagerMap;
	LightList*		mLightList;
	D3DCore*		mD3DCore;
	Camera*			mCamera;
	BlurFilter*		mBlurFilter;
	Primitive*		mScreenQuad;
	XMFLOAT4		mFogColor;

	// TMP
	Material mMaterial;
};	// Class