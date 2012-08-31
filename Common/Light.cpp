//***************************************************************************************
// LightHelper.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Light.h"

Light::Light() 
{
	ZeroMemory(this, sizeof(this));
	type = 0;
}

void Light::SetMaterials(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular)
{
	this->material.ambient  = ambient;
	this->material.diffuse  = diffuse;
	this->material.specular	= specular;
}
	
void Light::SetPosition(XMFLOAT3 position)
{
	this->position = position;
}
	
void Light::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

void Light::SetRange(float range)
{
	this->range = range;
}

void Light::SetDirection(XMFLOAT3 direction)
{
	this->direction = direction;
}
	
void Light::SetDirection(float x, float y, float z)
{
	SetDirection(XMFLOAT3(x, y, z));
}

void Light::SetSpot(float spot)
{
	this->spot = spot;
}
	
void Light::SetAtt(float a0, float a1, float a2)
{
	this->att = XMFLOAT3(a0, a1, a2);
}
	
void Light::SetType(LightType type)
{
	this->type = type;
}
 
 