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
class Camera;
struct Material;
struct Texture2D;


//! Represents a 2D texture.
struct Texture2D
{
	ID3D11ShaderResourceView* texture;
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
	void DrawBillboards(ID3D11Buffer* vertexBuffer, Texture2D* texture, int num);
	void DrawText(string text, int x, int y, D3DXCOLOR textColor, int size);

	Texture2D* LoadTexture(string filename, float scale = 1.0f);

	void ClearScene();
	void Present();

	void SetEffectParameters(BasicEffect* effect, CXMMATRIX worldMatrix, Texture2D* texture, Material material);
	void SetLightList(LightList* lightList);
	void SetFogColor(XMFLOAT4 color);

	// Getters.
	D3DCore* GetD3D();
	BasicEffect* GetBasicEffect();
private:
	D3DCore*		mD3DCore;
	Camera*			mCamera;
	map<string, Texture2D*> mTextureMap;
	XMFLOAT4		mFogColor;

	// TMP
	LightList* mLightList;
	Material mMaterial;
};	// Class