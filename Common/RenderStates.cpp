#include "RenderStates.h"
#include "d3dUtil.h"

ID3D11BlendState*			RenderStates::TransparentBS     = 0;
ID3D11DepthStencilState*	RenderStates::EnableAllDSS		= 0;  

void RenderStates::InitAll(ID3D11Device* device)
{
	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&transparentDesc, &TransparentBS));

	D3D11_DEPTH_STENCIL_DESC depthdesc;
	memset(&depthdesc, 0, sizeof(depthdesc));
	depthdesc.DepthEnable = (BOOL) false;
	depthdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthdesc.DepthFunc = D3D11_COMPARISON_NEVER;
	depthdesc.StencilEnable = (BOOL) false;
	HR(device->CreateDepthStencilState(&depthdesc, &EnableAllDSS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(TransparentBS);
	ReleaseCOM(EnableAllDSS);
}