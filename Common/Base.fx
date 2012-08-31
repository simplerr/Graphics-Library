#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
	Light	gLights[10];
	float3	gEyePosW;
	float	gNumLights;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	Material gMaterial;
	bool	 gUseTexture;
};

Texture2D gTexture;

SamplerState textureSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL		: POSITION;
	float3 NormalL	: NORMAL;
	float2 Tex		: TEXCOORD;
};

struct VertexOut
{
	float4 PosH		: SV_POSITION;
    float3 PosW		: POSITION;
    float3 NormalW	: NORMAL;
	float2 Tex		: TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
		
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Pass on the texture.
	vout.Tex = vin.Tex;
	
	return vout;
}
  
float4 PS(VertexOut pin) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW); 

	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	// Identity as default.
	float4 texColor = float4(1, 1, 1, 1);
	if(gUseTexture)
		texColor = gTexture.Sample(textureSampler, pin.Tex);

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Loop through all lights
	for(int i = 0; i < gNumLights; i++)
	{
		// Sum the light contribution from each light source.
		float4 A, D, S;

		if(gLights[i].type == 0)		// Directional light
			ComputeDirectionalLight(gMaterial, gLights[i], pin.NormalW, toEyeW, A, D, S);
		else if(gLights[i].type == 1)	// Point light.
			ComputePointLight(gMaterial, gLights[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);
		else if(gLights[i].type == 2)	// Spot light.
			ComputeSpotLight(gMaterial, gLights[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);

		ambient += A;  
		diffuse += D;
		spec    += S;
	}
	   
	float4 litColor = texColor*(ambient + diffuse) + spec;

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.diffuse.a * texColor.a;

    return litColor;
}

technique11 BaseTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}



