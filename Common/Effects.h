#pragma once

#include "d3dx11Effect.h"
#include <Windows.h>
#include <xnamath.h>
#include <vector>
#include "d3dUtil.h"
#include "Light.h"

using namespace std;

struct Material;
struct Texture2D;
class Light;
class BasicEffect;

/**
	Contains all the effects that can be used.
	Inits all effects.
	Destroys all effects.
*/
#pragma region Effects class

class Effects
{
public:
	static void InitAll();
	static void DestroyAll();

	static BasicEffect* BasicFX;
};

#pragma endregion

/**
	Base class for all effects.
	Extend this class to use custom effects.
*/
#pragma region Effect class

class Effect
{
public:
	Effect(string filename, string technique);
	virtual ~Effect();

	// Abstract functions.
	virtual void Init() {};
	virtual void CreateInputLayout() {};

	void Apply();

	// Used by the effect manager.
	void SetEffect(ID3DX11Effect* effect);
	void SetTech(string technique);

	ID3DX11Effect*			GetFx();
	ID3DX11EffectTechnique* GetTech();
	ID3D11InputLayout*		GetInputLayout();
protected:
	// These 3 members gets loaded by EffectManager::LoadEffect().
	ID3DX11Effect*			mEffect;
	ID3DX11EffectTechnique* mTech;
	ID3D11InputLayout*		mInputLayout; 
};

#pragma endregion

/**
	The basic effect class, used as the standard render effect.
*/
#pragma region BasicEffect class

class BasicEffect : public Effect
{
public:
	BasicEffect();
	~BasicEffect();

	void Init();
	void CreateInputLayout();

	// Setters to effect variables.
	void SetWorldViewProj(CXMMATRIX matrix)			{mfxWVP->SetMatrix(reinterpret_cast<const float*>(&matrix));}
	void SetWorld(CXMMATRIX matrix)					{mfxWorld->SetMatrix(reinterpret_cast<const float*>(&matrix));}
	void SetWorldInvTranspose(CXMMATRIX matrix)		{mfxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&matrix));}
	void SetEyePosition(XMFLOAT3 eyePos)			{mfxEyePosW->SetRawValue(&eyePos, 0, sizeof(XMFLOAT3));}
	void SetMaterial(Material material)				{mfxMaterial->SetRawValue(&material, 0, sizeof(material));}
	void SetUseTexture(bool use)					{mfxUseTexture->SetRawValue(&use, 0, sizeof(bool));}
	void SetFogRange(float range)					{mfxFogRange->SetFloat(range);}
	void SetFogStart(float start)					{mfxFogStart->SetFloat(start);}
	void SetFogColor(XMFLOAT4 color)				{mfxFogColor->SetFloatVector((const float*)&color);}
	void SetTexture(Texture2D* texture);				
	void SetLights(LightList* lights);

private:
	// Handles to effect variables.
	ID3DX11EffectMatrixVariable* mfxWVP;
	ID3DX11EffectMatrixVariable* mfxWorld;
	ID3DX11EffectMatrixVariable* mfxWorldInvTranspose;
	ID3DX11EffectMatrixVariable* mfxTexTransform;
	ID3DX11EffectVectorVariable* mfxEyePosW;
	ID3DX11EffectVariable* mfxLights;
	ID3DX11EffectVariable* mfxMaterial;
	ID3DX11EffectVariable* mfxNumLights;
	ID3DX11EffectVariable* mfxUseTexture;
	ID3DX11EffectShaderResourceVariable* mfxTexture;

	// Fog.
	ID3DX11EffectScalarVariable* mfxFogStart;
	ID3DX11EffectScalarVariable* mfxFogRange;
	ID3DX11EffectVectorVariable* mfxFogColor;
};

#pragma endregion