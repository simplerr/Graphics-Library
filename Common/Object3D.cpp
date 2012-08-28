#include "Object3D.h"
#include "Graphics.h"
#include "Effect.h"
#include "Runnable.h"
#include "../Primitive.h"
#include "PrimitiveFactory.h"
#include "D3DCore.h"

Object3D::Object3D()
{
	// Load the effect.
	mEffect = gGame->GetGraphics()->LoadEffect("color.fx");

	//mPosition = XMVectorZero();

	// Create the primitive.
	mPrimitive = gPrimitiveFactory->CreateBox();

	mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}
	
Object3D::~Object3D()
{

}

void Object3D::Update(float dt)
{

}
	
void Object3D::Draw(Graphics* pGraphics)
{
	// Draw the primitive.
	pGraphics->DrawPrimitive(mPrimitive, GetWorldMatrix(), mEffect);
}

XMFLOAT4X4 Object3D::GetWorldMatrix()
{
	XMMATRIX T, R, S, W;
	T = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	R = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
	S = XMMatrixScaling(mScale.x, mScale.y, mScale.z);

	W = S*R*T;

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, W);

	return world;
}

XMFLOAT3 Object3D::GetPosition()
{
	return mPosition;
}

XMFLOAT3 Object3D::GetRotation()
{
	return mRotation;
}
	
XMFLOAT3 Object3D::GetScale()
{
	return mScale;
}

void Object3D::SetPosition(XMFLOAT3 position)
{
	mPosition = position;
}
	
void Object3D::SetRotation(XMFLOAT3 rotation)
{
	mRotation = rotation;
}
	
void Object3D::SetScale(XMFLOAT3 scale)
{
	mScale = scale;
}