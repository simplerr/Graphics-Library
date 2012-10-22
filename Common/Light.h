#pragma once

#include <Windows.h>
#include <vector>
#include <xnamath.h>
#include <assimp\material.h>

using namespace std;

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

	Material(aiColor4D ambient, aiColor4D diffuse, aiColor4D specular) {
		this->ambient.x = ambient.r;
		this->ambient.y = ambient.g;
		this->ambient.z = ambient.b;
		this->ambient.w = ambient.a;

		this->diffuse.x = diffuse.r;
		this->diffuse.y = diffuse.g;
		this->diffuse.z = diffuse.b;
		this->diffuse.w = diffuse.a;

		this->specular.x = specular.r;
		this->specular.y = specular.g;
		this->specular.z = specular.b;
		this->specular.w = specular.a;
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular; // w = SpecPower
};

class Light
{
public:
	Light();

	// Setters
	void SetMaterials(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular);
	void SetPosition(XMFLOAT3 position);
	void SetPosition(float x, float y, float z);
	void SetRange(float range);
	void SetDirection(XMFLOAT3 direction);
	void SetDirection(float x, float y, float z);
	void SetSpot(float spot);
	void SetAtt(float a0, float a1, float a2);
	void SetType(LightType type);

	// Getters
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetDirection();
	XMFLOAT3 GetAtt();
	Material GetMaterial();
	float GetRange();
	float GetSpot();
private:
	// Light color
	Material mMaterial;

	// Packed into 4D vector: (position, range)
	XMFLOAT3 mPosition;
	float mRange;

	// Packed into 4D vector: (direction, spot)
	XMFLOAT3 mDirection;
	float mSpot;

	// Packed into 4D vector: (att, type)
	XMFLOAT3 mAtt;
	float mType;	// 0 = directional, 1 = point light, 2 = spot light
};