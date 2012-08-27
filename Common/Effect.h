#pragma once

#include "d3dx11Effect.h"
#include <Windows.h>
#include <xnamath.h>

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

	void SetWorldViewProj(XMMATRIX* matrix);
	void SetEffect(ID3DX11Effect* effect);
	void SetTech(ID3DX11EffectTechnique* tech);
	void SetInputLayout(ID3D11InputLayout* inputLayout);
private:
	// These 3 members gets loaded by EffectManager::LoadEffect().
	ID3DX11Effect*			mEffect;
	ID3DX11EffectTechnique* mTech;
	ID3D11InputLayout*		mInputLayout; 

	// Load these in the Init() function.
	ID3DX11EffectMatrixVariable* mfxWVP;
};