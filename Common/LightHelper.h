#pragma once

#include <Windows.h>
#include <xnamath.h>

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

struct Material
{
	Material() { 
		ZeroMemory(this, sizeof(this)); 
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular; // w = SpecPower
};

class Light
{
public:
	Light() {
		ZeroMemory(this, sizeof(this));
		type = 0;
	}

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