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
class BillboardEffect;
class BlurEffect;
class SkyEffect;
class ShadowMapEffect;
class TerrainEffect;

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

	static BasicEffect*		BasicFX;
	static BillboardEffect* BillboardFX;
	static BlurEffect*		BlurFX;
	static SkyEffect*		SkyFX;
	static ShadowMapEffect* BuildShadowMapFX;
	static TerrainEffect*	TerrainFX;
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


//! The basic effect class, used as the standard render effect.
#pragma region BasicEffect class

class BasicEffect : public Effect
{
public:
	BasicEffect();
	~BasicEffect();

	void Init();
	void CreateInputLayout();

	// Setters to effect variables.
	void SetWorldViewProj(CXMMATRIX matrix)			{ mfxWVP->SetMatrix(reinterpret_cast<const float*>(&matrix)); }
	void SetWorld(CXMMATRIX matrix)					{ mfxWorld->SetMatrix(reinterpret_cast<const float*>(&matrix)); }
	void SetWorldInvTranspose(CXMMATRIX matrix)		{ mfxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&matrix)); }
	void SetEyePosition(XMFLOAT3 eyePos)			{ mfxEyePosW->SetRawValue(&eyePos, 0, sizeof(XMFLOAT3)); }
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt) { mfxBoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
	void SetMaterial(Material material)				{ mfxMaterial->SetRawValue(&material, 0, sizeof(material)); }
	void SetUseTexture(bool use)					{ mfxUseTexture->SetRawValue(&use, 0, sizeof(bool)); }
	void SetUseNormalMap(bool use)					{ mfxUseNormalMap->SetRawValue(&use, 0, sizeof(bool)) ;}
	void SetUseAnimation(bool use)					{ mfxUseAnimation->SetRawValue(&use, 0, sizeof(bool)) ;}
	void SetFogRange(float range)					{ mfxFogRange->SetFloat(range); }
	void SetFogStart(float start)					{ mfxFogStart->SetFloat(start); }
	void SetFogColor(XMFLOAT4 color)				{ mfxFogColor->SetFloatVector((const float*)&color); }
	void SetShadowMap(ID3D11ShaderResourceView* tex){ mfxShadowMap->SetResource(tex); }
	void SetShadowTransform(CXMMATRIX M)            { mfxShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexture(Texture2D* texture);				
	void SetNormalMap(Texture2D* normalMap);
	void SetLights(LightList* lights);

	ID3DX11EffectTechnique* GetSkinnedTech();
private:
	// Handles to effect variables.
	ID3DX11EffectMatrixVariable* mfxWVP;
	ID3DX11EffectMatrixVariable* mfxWorld;
	ID3DX11EffectMatrixVariable* mfxWorldInvTranspose;
	ID3DX11EffectMatrixVariable* mfxTexTransform;
	ID3DX11EffectMatrixVariable* mfxShadowTransform;
	ID3DX11EffectVectorVariable* mfxEyePosW;
	ID3DX11EffectVariable* mfxLights;
	ID3DX11EffectVariable* mfxMaterial;
	ID3DX11EffectVariable* mfxNumLights;
	ID3DX11EffectVariable* mfxUseTexture;
	ID3DX11EffectShaderResourceVariable* mfxTexture;
	ID3DX11EffectShaderResourceVariable* mfxNormalMap;
	ID3DX11EffectShaderResourceVariable* mfxShadowMap;
	ID3DX11EffectVariable* mfxUseNormalMap;

	// Fog.
	ID3DX11EffectScalarVariable* mfxFogStart;
	ID3DX11EffectScalarVariable* mfxFogRange;
	ID3DX11EffectVectorVariable* mfxFogColor;

	// Skinning.
	ID3DX11EffectTechnique*		 mfxSkinnedTech;
	ID3DX11EffectMatrixVariable* mfxBoneTransforms;
	ID3DX11EffectVariable*		 mfxUseAnimation;
};

#pragma endregion


//! The billboard effect.
class BillboardEffect : public Effect
{
public:
	BillboardEffect();
	~BillboardEffect();

	void Init();
	void CreateInputLayout();

	// Setters to the effect variables.
	void SetViewProj(CXMMATRIX matrix)              { mfxViewProj->SetMatrix(reinterpret_cast<const float*>(&matrix)); }
	void SetEyePosition(XMFLOAT3 eyePos)			{ mfxEyePosW->SetRawValue(&eyePos, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(XMFLOAT4 color)				{ mfxFogColor->SetFloatVector(reinterpret_cast<const float*>(&color)); }
	void SetFogStart(float start)                   { mfxFogStart->SetFloat(start); }
	void SetFogRange(float range)                   { mfxFogRange->SetFloat(range); }
	void SetUseTexture(bool use)					{ mfxUseTexture->SetRawValue(&use, 0, sizeof(bool)); }
	void SetTexture(ID3D11ShaderResourceView* tex)	{ mfxTexture->SetResource(tex); }
	void SetMaterial(Material material)				{ mfxMaterial->SetRawValue(&material, 0, sizeof(material)); }
	void SetTexture(Texture2D* texture);				
	void SetLights(LightList* lights);
private:
	ID3DX11EffectMatrixVariable* mfxViewProj;
	ID3DX11EffectVectorVariable* mfxEyePosW;
	ID3DX11EffectVectorVariable* mfxFogColor;
	ID3DX11EffectScalarVariable* mfxFogStart;
	ID3DX11EffectScalarVariable* mfxFogRange;
	ID3DX11EffectVariable*		 mfxLights;
	ID3DX11EffectVariable*		 mfxNumLights;
	ID3DX11EffectVariable*		 mfxMaterial;
	ID3DX11EffectVariable*		 mfxUseTexture;
	ID3DX11EffectShaderResourceVariable* mfxTexture;
};


//! The blur effect.
class BlurEffect : public Effect
{
public:
	BlurEffect();
	~BlurEffect();

	void Init();

	void SetWeights(const float weights[9])					{ Weights->SetFloatArray(weights, 0, 9); }
	void SetInputMap(ID3D11ShaderResourceView* tex)			{ InputMap->SetResource(tex); }
	void SetOutputMap(ID3D11UnorderedAccessView* tex)		{ OutputMap->SetUnorderedAccessView(tex); }

	ID3DX11EffectTechnique*	HorzTech;
	ID3DX11EffectTechnique* VertTech;

	ID3DX11EffectScalarVariable* Weights;
	ID3DX11EffectShaderResourceVariable* InputMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputMap;
};

//! The sky box effect.
class SkyEffect : public Effect
{
public:
	SkyEffect();
	~SkyEffect();

	void Init();
	void CreateInputLayout();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetCubeMap(ID3D11ShaderResourceView* cubemap)  { CubeMap->SetResource(cubemap); }

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectShaderResourceVariable* CubeMap;
};


//! The build shadow map effect.
class ShadowMapEffect : public Effect
{
public:
	ShadowMapEffect();
	~ShadowMapEffect();

	void Init();
	void CreateInputLayout();

	void SetViewProj(CXMMATRIX M)           { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M)      { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)              { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)  { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	//void SetTexture(Texture2D* texture); [TODO] Add support for alpha clipping.

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
};

//! The terrain effect.
#pragma region TerrainEffect
class TerrainEffect : public Effect
{
public:
	TerrainEffect();
	~TerrainEffect();

	void Init();
	void CreateInputLayout();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)	{ ShadowMap->SetResource(tex); }
	void SetShadowTransform(CXMMATRIX M)				{ ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexelCellSpaceU(float f)                    { TexelCellSpaceU->SetFloat(f); }
	void SetTexelCellSpaceV(float f)                    { TexelCellSpaceV->SetFloat(f); }
	void SetWorldCellSpace(float f)                     { WorldCellSpace->SetFloat(f); }
	void SetTextureScale(float scale)					{ TexScale->SetFloat(scale); }

	void SetLayerMapArray(ID3D11ShaderResourceView* tex)   { LayerMapArray->SetResource(tex); }
	void SetBlendMap(ID3D11ShaderResourceView* tex)        { BlendMap->SetResource(tex); }
	void SetHeightMap(ID3D11ShaderResourceView* tex)       { HeightMap->SetResource(tex); }

	void SetLights(LightList* lights);

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectScalarVariable* TexScale;
	ID3DX11EffectScalarVariable* TexelCellSpaceU;
	ID3DX11EffectScalarVariable* TexelCellSpaceV;
	ID3DX11EffectScalarVariable* WorldCellSpace;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectVariable* Lights;
	ID3DX11EffectVariable* NumLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* LayerMapArray;
	ID3DX11EffectShaderResourceVariable* BlendMap;
	ID3DX11EffectShaderResourceVariable* HeightMap;
};
#pragma endregion