#include "EffectManager.h"
#include "Effect.h"
#include "Runnable.h"
#include "D3DCore.h"
#include <fstream>
#include <vector>
#include "d3dUtil.h"

EffectManager::EffectManager()
{

}
	
EffectManager::~EffectManager()
{

}

//! Loads and returns an effect. Effects already loaded are stored to decrease memory usage.
/**
@param filename the effect file to load, ex "color.fx"
*/
Effect* EffectManager::LoadEffect(string filename, string technique)
{
	// Effect already loaded.
	if(mEffectMap.find(filename) != mEffectMap.end())
	{
		Effect* effect = new Effect();

		effect->SetEffect(mEffectMap[filename].effect);
		effect->SetTech(mEffectMap[filename].tech);
		effect->SetInputLayout(mEffectMap[filename].inputLayout);

		return effect;
	}
	else
	{
		// Create the FX.
		EffectData data;
		data.effect	= CreateFx(filename);
		data.tech   = data.effect->GetTechniqueByName(technique.c_str());

		// Crate the effect and set values.
		Effect* effect = new Effect();
		effect->SetEffect(data.effect);
		effect->SetTech(data.tech);

		// Let the effect itself control the input layout.
		data.inputLayout = effect->CreateInputLayout();
		effect->SetInputLayout(data.inputLayout);

		// Add the effect data to the map.
		mEffectMap[filename] = data;

		return effect;
	}
}

//! Creates a ID3DX11Effect instance from memory. Called by LoadEffect().
ID3DX11Effect* EffectManager::CreateFx(string filename)
{
	ID3DX11Effect* effect;

	// Create the effect from memory.
	std::ifstream fin("Common/" + filename + "o", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, gGame->GetD3D()->GetDevice(), &effect));

	return effect;
}