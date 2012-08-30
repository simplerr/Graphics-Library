#include "Effect.h"
#include "Runnable.h"
#include "D3DCore.h"
#include "d3dUtil.h"
#include "LightHelper.h"


Effect::Effect()
{

}
	
Effect::~Effect()
{

}

void Effect::Init()
{
	// Connect handles to the effect variables.
	mfxWVP = mEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	mfxWorld = mEffect->GetVariableByName("gWorld")->AsMatrix();
	mfxWorldInvTranspose = mEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mfxEyePosW = mEffect->GetVariableByName("gEyePosW")->AsVector();
	mfxLights = mEffect->GetVariableByName("gLights");
	mfxMaterial = mEffect->GetVariableByName("gMaterial");
	mfxNumLights = mEffect->GetVariableByName("gNumLights");
}

//! Creates the input layout that will get set before the Input-Assembler state.
/**
@note don't call this function on your own, the EffectManager handles this and makes sure it only runs once
*/
ID3D11InputLayout* Effect::CreateInputLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11InputLayout* inputLayout;

	// Create the input layout
    D3DX11_PASS_DESC passDesc;
    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(gGame->GetD3D()->GetDevice()->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &inputLayout));

	return inputLayout;
}

//! Applies the effect to the render pipeline.
void Effect::Apply()
{
	// Applies the changes effect and it changes to the pipeline.
	mTech->GetPassByIndex(0)->Apply(0, gGame->GetD3D()->GetContext());
}

ID3DX11Effect* Effect::GetFx()
{
	return mEffect;
}

ID3DX11EffectTechnique* Effect::GetTech()
{
	return mTech;
}
	
ID3D11InputLayout* Effect::GetInputLayout()
{
	return mInputLayout;
}

void Effect::SetWorldViewProj(CXMMATRIX matrix)
{
	mfxWVP->SetMatrix(reinterpret_cast<const float*>(&matrix));
}

void Effect::SetWorld(CXMMATRIX matrix)
{
	mfxWorld->SetMatrix(reinterpret_cast<const float*>(&matrix));
}
	
void Effect::SetWorldInvTranspose(CXMMATRIX matrix)
{
	mfxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&matrix));
}
	
void Effect::SetEyePosition(FXMVECTOR eyePos)
{
	mfxEyePosW->SetRawValue(&eyePos, 0, sizeof(eyePos));
}
	
void Effect::SetMaterial(Material material)
{
	mfxMaterial->SetRawValue(&material, 0, sizeof(material));
}

void Effect::SetLights(vector<Light> lights)
{
	mfxLights->SetRawValue(&lights[0], 0, sizeof(Light) * lights.size());
	float size = lights.size();
	mfxNumLights->SetRawValue(&size, 0, sizeof(float));
}

void Effect::SetEffect(ID3DX11Effect* effect)
{
	mEffect = effect;
}
	
void Effect::SetTech(ID3DX11EffectTechnique* tech)
{
	mTech = tech;
}
	
void Effect::SetInputLayout(ID3D11InputLayout* inputLayout)
{ 
	mInputLayout = inputLayout; 
}