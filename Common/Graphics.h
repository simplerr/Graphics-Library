#pragma once

#include <Windows.h>
#include <string>
#include <xnamath.h>
#include <vector>
#include <d3dx9.h>
#include "LightHelper.h"

using namespace std;

class D3DCore;
class Effect;
class Primitive;
class EffectManager;
class Camera;
struct D3DXCOLOR;
struct Material;
struct DirectionalLight;

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
	Effect* LoadEffect(string filename, string technique);

	void DrawPrimitive(Primitive* primitive, CXMMATRIX worldMatrix, Material material, Effect* effect);
	void DrawText(string text, int x, int y, D3DXCOLOR textColor, int size);
	void SetEffectParameters(Effect* effect, CXMMATRIX worldMatrix, Material material);

	void ClearScene();
	void Present();

	void BuildVertexLayout();

	D3DCore* GetD3D();
private:
	D3DCore* mD3DCore;
	EffectManager* mEffectManager;
	Camera*	mCamera;
	ID3DXFont* mFont;

	// Move these to the camera class later
	// [NOTE] Never store XMMATRIX as a member because of alignment problems.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	// TMP
	vector<Light> mLightList;
	Material mMaterial;
};	// Class