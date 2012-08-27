#pragma once

#include <map>
#include <D3DX11.h>
#include <string>
#include "d3dx11Effect.h"
using namespace std;

class Effect;

struct EffectData
{
	ID3DX11Effect* effect;
	ID3DX11EffectTechnique* tech;
	ID3D11InputLayout* inputLayout; 
};

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