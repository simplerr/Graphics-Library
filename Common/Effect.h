#pragma once

#include "d3dx11Effect.h"
#include <Windows.h>
#include <xnamath.h>
#include <vector>
#include "d3dUtil.h"

using namespace std;

struct Material;
class Light;

/**
	Wrapper for the effect interface.
	Extend this class to use custom effects.
*/
class Effect
{
public:
	Effect();
	~Effect();

	void Init();
	ID3D11InputLayout* CreateInputLayout();
	void Apply();

	ID3DX11Effect*			GetFx();
	ID3DX11EffectTechnique* GetTech();
	ID3D11InputLayout*		GetInputLayout();

	// Used by the effect manager.
	void SetEffect(ID3DX11Effect* effect);
	void SetTech(ID3DX11EffectTechnique* tech);
	void SetInputLayout(ID3D11InputLayout* inputLayout);

	// Setters to effect variables.
	void SetWorldViewProj(CXMMATRIX matrix);
	void SetWorld(CXMMATRIX matrix);
	void SetWorldInvTranspose(CXMMATRIX matrix);
	void SetEyePosition(FXMVECTOR eyePos);
	void SetMaterial(Material material);
	void SetLights(LightList* lights);
	
private:
	// These 3 members gets loaded by EffectManager::LoadEffect().
	ID3DX11Effect*			mEffect;
	ID3DX11EffectTechnique* mTech;
	ID3D11InputLayout*		mInputLayout; 

	// Handles to effect variables.
	ID3DX11EffectMatrixVariable* mfxWVP;
	ID3DX11EffectMatrixVariable* mfxWorld;
	ID3DX11EffectMatrixVariable* mfxWorldInvTranspose;
	ID3DX11EffectVectorVariable* mfxEyePosW;
	ID3DX11EffectVariable* mfxLights;
	ID3DX11EffectVariable* mfxMaterial;
	ID3DX11EffectVariable* mfxNumLights;
};