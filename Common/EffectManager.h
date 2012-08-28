#pragma once

#include <map>
#include <D3DX11.h>
#include <string>
#include "d3dx11Effect.h"
using namespace std;

class Effect;

//! Structure used to store the loaded effects in EffectManagers map.
struct EffectData
{
	ID3DX11Effect* effect;
	ID3DX11EffectTechnique* tech;
	ID3D11InputLayout* inputLayout; 
};

/**
	Handles loading of effects.
	Makes sure an effect only gets loaded once.
	Returns pointer if an effect already is loaded.
*/
class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	Effect* LoadEffect(string filename);
private:
	ID3DX11Effect* CreateFx(string filename);
	map<string, EffectData> mEffectMap;
};