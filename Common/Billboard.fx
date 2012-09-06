#include "LightHelper.fx"

//! Constant buffers that updates per frame.
cbuffer cbPerFrame
{
	// Lights & num lights.
	Light	gLights[10];
	float	gNumLights;

	// Eye position (world space).
	float3	gEyePosW;

	// Fog.
	float	gFogStart;
	float	gFogRange;
	float4	gFogColor;
};

//! Constant buffers that updates per object.
cbuffer cbPerObject
{
	float4x4 gViewProj;
	Material gMaterial;
	bool	 gUseTexture;
};

//! Texture coordinates.
cbuffer cbFixed
{
	float2 gTexC[4] = 
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
};

//! The texture to use.
Texture2D gTexture;

//! Sampler state.
SamplerState samplerState
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//! Input for the vertex shader.
struct VertexIn
{
	float3 PosW  : POSITION;
	float2 SizeW : SIZE;
};

//! Output by the vertex shader.
struct VertexOut
{
	float3 CenterW : POSITION;
	float2 SizeW   : SIZE;
};

//! Output by the geometry shader, input for the pixel shader.
struct GeoOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
};

//! Vertex shader that just passes the data to the geometry shader.
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Just pass data over to geometry shader.
	vout.CenterW = vin.PosW;
	vout.SizeW   = vin.SizeW;

	return vout;
}

//! Geometry shader that expands a point to a quad.
[maxvertexcount(4)]
void GS(point VertexOut gin[1], inout TriangleStream<GeoOut> triStream)
{	
	// Compute the local coordinate system of the sprite relative to the world
	// space such that the billboard is aligned with the y-axis and faces the eye.
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = gEyePosW - gin[0].CenterW;
	look.y = 0.0f; // y-axis aligned, so project to xz-plane
	look = normalize(look);
	float3 right = cross(up, look);

	// Compute triangle strip vertices (quad) in world space.
	float halfWidth  = 0.5f*gin[0].SizeW.x;
	float halfHeight = 0.5f*gin[0].SizeW.y;
	
	float4 v[4];
	v[0] = float4(gin[0].CenterW + halfWidth*right - halfHeight*up, 1.0f);
	v[1] = float4(gin[0].CenterW + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(gin[0].CenterW - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(gin[0].CenterW - halfWidth*right + halfHeight*up, 1.0f);

	// Transform quad vertices to world space and output them as a triangle strip.
	GeoOut gout;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		gout.PosH     = mul(v[i], gViewProj);
		gout.PosW     = v[i].xyz;
		gout.NormalW  = look;
		gout.Tex      = gTexC[i];
		
		triStream.Append(gout);
	}
}

//! Pixel shader that applies ligthing and fogging.
float4 PS(GeoOut pin) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW); 

	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	// Identity as default.
	float4 texColor = float4(1, 1, 1, 1);
	if(gUseTexture)
		texColor = gTexture.Sample(samplerState, pin.Tex);

	// Apply lighting.
	float4 litColor;
	ApplyLighting(gNumLights, gLights, gMaterial, pin.PosW, pin.NormalW, toEyeW, texColor, litColor);

	//! Apply fogging.
	float distToEye = length(gEyePosW - pin.PosW);
	float fogLerp = saturate( (distToEye - gFogStart) / gFogRange ); 

	// Blend the fog color and the lit color.
	litColor = lerp(litColor, gFogColor, fogLerp);

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.diffuse.a * texColor.a;

    return litColor;
}

//! The billboard technique.
technique11 BillboardTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}