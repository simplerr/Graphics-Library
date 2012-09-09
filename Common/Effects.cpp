#include <fstream>
#include "Effects.h"
#include "Runnable.h"
#include "D3DCore.h"
#include "d3dUtil.h"
#include "Light.h"
#include "Graphics.h"

//! Initialize the effects.
BasicEffect*		Effects::BasicFX		= nullptr;
BillboardEffect*	Effects::BillboardFX	= nullptr;
BlurEffect*			Effects::BlurFX			= nullptr;

#pragma region Code for the static effect handler Effects.

//! Inits all effects.
void Effects::InitAll()
{
	/**
		Init the basic effect.
	*/
	BasicFX = new BasicEffect();
	BasicFX->CreateInputLayout();
	BasicFX->Init();

	/**
		Init the billboard effect.
	*/
	BillboardFX = new BillboardEffect();
	BillboardFX->CreateInputLayout();
	BillboardFX->Init();

	/**
		Init the blur effect.
	*/
	BlurFX = new BlurEffect();
	BlurFX->Init();
}

//! Cleans up all effects.
void Effects::DestroyAll()
{
	delete BasicFX;
	delete BillboardFX;
	delete BlurFX;
}

#pragma endregion

#pragma region Code for the effect base class Effect.

//! Loads the effect from the file and creates the standard technique.
Effect::Effect(string filename, string technique)
{
	// Create the effect from memory.
	std::ifstream fin("Common/" + filename + "o", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, gGame->GetD3D()->GetDevice(), &mEffect));

	// Set the technique.
	mTech = mEffect->GetTechniqueByName(technique.c_str());
}
	
Effect::~Effect()
{

}

//! Applies the effect to the render pipeline.
void Effect::Apply()
{
	// Applies the changes effect and it changes to the pipeline.
	mTech->GetPassByIndex(0)->Apply(0, gGame->GetD3D()->GetContext());
}

//! Returns the ID3DX11Effect member.
ID3DX11Effect* Effect::GetFx()
{
	return mEffect;
}

//! Returns the standard technique.
ID3DX11EffectTechnique* Effect::GetTech()
{
	return mTech;
}
	
//! Returns the input layout.
ID3D11InputLayout* Effect::GetInputLayout()
{
	return mInputLayout;
}

//! Sets the ID3DX11Effect member.
void Effect::SetEffect(ID3DX11Effect* effect)
{
	mEffect = effect;
}
	
//! Sets the standard technique.
void Effect::SetTech(string technique)
{
	mTech = mEffect->GetTechniqueByName(technique.c_str());
}

#pragma endregion

#pragma region Code for the basic effect class BasicEffect.

BasicEffect::BasicEffect()
	: Effect("Basic.fx", "BasicTech")
{

}
	
BasicEffect::~BasicEffect()
{

}

//! Inits all effect variable handles.
void BasicEffect::Init()
{
	// Connect handlers to the effect variables.
	mfxWVP				 = mEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	mfxWorld			 = mEffect->GetVariableByName("gWorld")->AsMatrix();
	mfxWorldInvTranspose = mEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mfxTexTransform		 = mEffect->GetVariableByName("gTexTransform")->AsMatrix();
	mfxEyePosW			 = mEffect->GetVariableByName("gEyePosW")->AsVector();
	mfxLights			 = mEffect->GetVariableByName("gLights");
	mfxMaterial			 = mEffect->GetVariableByName("gMaterial");
	mfxNumLights		 = mEffect->GetVariableByName("gNumLights");
	mfxUseTexture		 = mEffect->GetVariableByName("gUseTexture");
	mfxTexture			 = mEffect->GetVariableByName("gTexture")->AsShaderResource();
	mfxFogStart			 = mEffect->GetVariableByName("gFogStart")->AsScalar();
	mfxFogRange			 = mEffect->GetVariableByName("gFogRange")->AsScalar();
	mfxFogColor			 = mEffect->GetVariableByName("gFogColor")->AsVector();
}

//! Creates the input layout that will get set before the Input-Assembler state. The EffectManager calls this function.
void BasicEffect::CreateInputLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout.
    D3DX11_PASS_DESC passDesc;
    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(gGame->GetD3D()->GetDevice()->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &mInputLayout));
}

//! Sets the lights to use in the shader.
void BasicEffect::SetLights(LightList* lights)
{
	Light lightArray[10];
	for(int i = 0; i < lights->size(); i++)
		lightArray[i] = *lights->operator[](i);

	// Sets the light list in the effect file.
	mfxLights->SetRawValue((void*)lightArray, 0, sizeof(Light) * lights->size());
	float size = lights->size();
	mfxNumLights->SetRawValue(&size, 0, sizeof(float));
}

//! Sets the texture to use in the shader.
void BasicEffect::SetTexture(Texture2D* texture)
{
	SetUseTexture(texture == nullptr ? false : true);
	if(texture != nullptr) {
		mfxTexture->SetResource(texture->shaderResourceView);
		XMMATRIX transform = XMMatrixScaling(texture->scale, texture->scale, 0);
		mfxTexTransform->SetMatrix((const float*)&transform);
	}
}

#pragma endregion

#pragma region Code for the billboard effect class BillboardEffect.

BillboardEffect::BillboardEffect()
	: Effect("Billboard.fx", "BillboardTech")
{

}
	
BillboardEffect::~BillboardEffect()
{

}

void BillboardEffect::Init()
{
	// Connect handles to the effect variables.
	mfxViewProj		= mEffect->GetVariableByName("gViewProj")->AsMatrix();
	mfxEyePosW		= mEffect->GetVariableByName("gEyePosW")->AsVector();
	mfxFogColor		= mEffect->GetVariableByName("gFogColor")->AsVector();
	mfxFogStart		= mEffect->GetVariableByName("gFogStart")->AsScalar();
	mfxFogRange		= mEffect->GetVariableByName("gFogRange")->AsScalar();
	mfxLights		= mEffect->GetVariableByName("gLights");
	mfxMaterial		= mEffect->GetVariableByName("gMaterial");
	mfxTexture		= mEffect->GetVariableByName("gTexture")->AsShaderResource();
	mfxUseTexture	= mEffect->GetVariableByName("gUseTexture");
	mfxNumLights	= mEffect->GetVariableByName("gNumLights");
}
	
void BillboardEffect::CreateInputLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",   0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout.
    D3DX11_PASS_DESC passDesc;
    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(gGame->GetD3D()->GetDevice()->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &mInputLayout));
}

void BillboardEffect::SetTexture(Texture2D* texture)
{
	SetUseTexture(texture == nullptr ? false : true);
	mfxTexture->SetResource(texture->shaderResourceView);
}
	
void BillboardEffect::SetLights(LightList* lights)
{
	Light lightArray[10];
	for(int i = 0; i < lights->size(); i++)
		lightArray[i] = *lights->operator[](i);

	// Sets the light list in the effect file.
	mfxLights->SetRawValue((void*)lightArray, 0, sizeof(Light) * lights->size());
	float size = lights->size();
	mfxNumLights->SetRawValue(&size, 0, sizeof(float));
}
#pragma endregion

BlurEffect::BlurEffect()
	: Effect("Blur.fx", "VertBlur")
{

}
	
BlurEffect::~BlurEffect()
{

}

void BlurEffect::Init()
{
	HorzTech	= mEffect->GetTechniqueByName("HorzBlur");
	VertTech	= mEffect->GetTechniqueByName("VertBlur");

	Weights     = mEffect->GetVariableByName("gWeights")->AsScalar();
	InputMap    = mEffect->GetVariableByName("gInput")->AsShaderResource();
	OutputMap   = mEffect->GetVariableByName("gOutput")->AsUnorderedAccessView();
}