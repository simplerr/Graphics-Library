#pragma once

#include <D3DX11.h>

class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11BlendState*		TransparentBS;
	static ID3D11DepthStencilState* EnableAllDSS;
	static ID3D11DepthStencilState* NoDoubleBlendDSS;
};