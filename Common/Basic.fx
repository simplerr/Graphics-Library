#include "LightHelper.fx"
 
//! Constant buffers updates per frame.
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

//! Constant buffers updates per object.
cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
	bool	 gUseTexture;
};

//! The texture to use.
Texture2D gTexture;

//! The sampler state to use with the texture.
SamplerState textureSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressV = WRAP;
};

//! Input parameter to the vertex shader.
struct VertexIn
{
	float3 PosL		: POSITION;
	float3 NormalL	: NORMAL;
	float2 Tex		: TEXCOORD;
};

//! Output by the vertex shader -> input for the pixel shader.
struct VertexOut
{
	float4 PosH		: SV_POSITION;
    float3 PosW		: POSITION;
    float3 NormalW	: NORMAL;
	float2 Tex		: TEXCOORD;
};

//! Vertex shader that transforms coordinates and normals to the world and homogeneous space.
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
		
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Pass on the texture.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	
	return vout;
}

//! Pixel shader that applies ligthing and fogging.
float4 PS(VertexOut pin) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW); 

	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	// Identity as default.
	float4 texColor = float4(1, 1, 1, 1);
	if(gUseTexture)
		texColor = gTexture.Sample(textureSampler, pin.Tex);

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

//! The standard technique used by most 3D objects.
technique11 BasicTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}