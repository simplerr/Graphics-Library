//***************************************************************************************
// LightHelper.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Light.h"

Light::Light() 
{
	ZeroMemory(this, sizeof(this));
	mType = 0;
}

void Light::SetMaterials(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular)
{
	mMaterial.ambient  = ambient;
	mMaterial.diffuse  = diffuse;
	mMaterial.specular	= specular;
}
	
void Light::SetPosition(XMFLOAT3 position)
{
	mPosition = position;
}
	
void Light::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

void Light::SetRange(float range)
{
	mRange = range;
}

void Light::SetDirection(XMFLOAT3 direction)
{
	// Normalize the direction.
	XMVECTOR dir = XMLoadFloat3(&direction);
	dir = XMVector3Normalize(dir);
	XMStoreFloat3(&mDirection, dir);
}
	
void Light::SetDirection(float x, float y, float z)
{
	SetDirection(XMFLOAT3(x, y, z));
}

void Light::SetSpot(float spot)
{
	mSpot = spot;
}
	
void Light::SetAtt(float a0, float a1, float a2)
{
	mAtt = XMFLOAT3(a0, a1, a2);
}
	
void Light::SetType(LightType type)
{
	mType = type;
}
 
XMFLOAT3 Light::GetPosition()
{
	return mPosition;
}
	
XMFLOAT3 Light::GetDirection()
{
	return mDirection;
}
	
XMFLOAT3 Light::GetAtt()
{
	return mAtt;
}
	
float Light::GetSpot()
{
	return mSpot;
}

float Light::GetRange()
{
	return mRange;
}

Material Light::GetMaterial()
{
	return mMaterial;
}
 