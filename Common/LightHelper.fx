
/**
	This file contains all functions that does lighting calculations.
	Include it in any .fx file.
*/

//! Corresponds to the C++ class Material. Stores the ambient, diffuse and specular colors for a material.
struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular; // w = SpecPower
};

//! Corresponds to the C++ class Light, stores all information about a light.
struct Light
{
	// Light color.
	Material mat;

	float3 pos;
	float range;

	float3 dir;
	float spot;

	float3 att;
	float type;		 // 0 = directional, 1 = point light, 2 = spot light
};

//! Computes the colors for directional light.
void ComputeDirectionalLight(Material mat, Light light, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -light.dir;

	// Add ambient term.
	ambient = mat.ambient * light.mat.ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if(diffuseFactor > 0.0f)
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
					
		diffuse = diffuseFactor * mat.diffuse * light.mat.diffuse;
		spec    = specFactor * mat.specular * light.mat.specular;
	}
}

//! Computes the colors for a point light.
void ComputePointLight(Material mat, Light light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = light.pos - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if( d > light.range )
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mat.ambient * light.mat.ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if(diffuseFactor > 0.0f)
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
					
		diffuse = diffuseFactor * mat.diffuse * light.mat.diffuse;
		spec    = specFactor * mat.specular * light.mat.specular;
	}

	// Attenuate
	float att = 1.0f / dot(light.att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec    *= att;
}

//! Computes the colors for a spot light.
void ComputeSpotLight(Material mat, Light light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = light.pos - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if(d > light.range)
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mat.ambient * light.mat.ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if(diffuseFactor > 0.0f)
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
					
		diffuse = diffuseFactor * mat.diffuse * light.mat.diffuse;
		spec    = specFactor * mat.specular * light.mat.specular;
	}
	
	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, light.dir), 0.0f), light.spot);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(light.att, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec    *= att;
}

//! Takes a list of lights and calculate the resulting color for the pixel after all light calculations.
void ApplyLighting(int numLights, Light lights[10], Material material, float3 posW, float3 normalW, float3 toEyeW, float4 texColor, out float4 litColor)
{
	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Loop through all lights
	for(int i = 0; i < numLights; i++)
	{
		// Sum the light contribution from each light source.
		float4 A, D, S;

		if(lights[i].type == 0)		// Directional light
			ComputeDirectionalLight(material, lights[i], normalW, toEyeW, A, D, S);
		else if(lights[i].type == 1)	// Point light.
			ComputePointLight(material, lights[i], posW, normalW, toEyeW, A, D, S);
		else if(lights[i].type == 2)	// Spot light.
			ComputeSpotLight(material, lights[i], posW, normalW, toEyeW, A, D, S);

		ambient += A;  
		diffuse += D;
		spec    += S;
	}
	   
	litColor = texColor*(ambient + diffuse) + spec;
}