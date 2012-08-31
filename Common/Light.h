#pragma once

#include <Windows.h>
#include <xnamath.h>

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

enum LightType
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT
};

struct Material
{
	Material() { 
		ZeroMemory(this, sizeof(this)); 
	}

	Material(XMFLOAT4 color) {
		ambient = diffuse = specular = color;
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular; // w = SpecPower
};

class Light
{
public:
	Light();

	void SetMaterials(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular);
	void SetPosition(XMFLOAT3 position);
	void SetPosition(float x, float y, float z);
	void SetRange(float range);
	void SetDirection(XMFLOAT3 direction);
	void SetDirection(float x, float y, float z);
	void SetSpot(float spot);
	void SetAtt(float a0, float a1, float a2);
	void SetType(LightType type);
private:
	// Light color
	Material material;

	// Packed into 4D vector: (position, range)
	XMFLOAT3 position;
	float range;

	// Packed into 4D vector: (direction, spot)
	XMFLOAT3 direction;
	float spot;

	// Packed into 4D vector: (att, type)
	XMFLOAT3 att;
	float type;	// 0 = directional, 1 = point light, 2 = spot light
};