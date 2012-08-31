#include "Object3D.h"
#include "Graphics.h"
#include "Effect.h"
#include "Runnable.h"
#include "Primitive.h"
#include "PrimitiveFactory.h"
#include "D3DCore.h"

Object3D::Object3D()
{
	mEffect		= nullptr;
	mPrimitive	= nullptr;
	mTexture	= nullptr;

	SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	SetMaterial(Material(Colors::White));
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
	pGraphics->DrawPrimitive(mPrimitive, GetWorldMatrix(), mTexture, GetMaterial(), mEffect);
}

//! Sets the texture to pass to the shader.
void Object3D::SetTexture(string filename)
{
	mTexture = gGame->GetGraphics()->LoadTexture(filename);
}

//! Sets the objects material.
void Object3D::SetMaterial(Material material)
{
	mMaterial = material;
}

//! Sets the effect to use when drawing.
void Object3D::SetEffect(Effect* effect)
{
	mEffect = effect;
}

//! Sets the primitive to use when drawing.
void Object3D::SetPrimitive(Primitive* primitive)
{
	if(mPrimitive != nullptr)
		mPrimitive->Cleanup();

	mPrimitive = primitive;
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

//! Returns the world matrix.
XMMATRIX Object3D::GetWorldMatrix()
{
	XMMATRIX T, R, S, W;
	T = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	R = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
	S = XMMatrixScaling(mScale.x, mScale.y, mScale.z);

	return S*R*T;
}

Material Object3D::GetMaterial()
{
	return mMaterial;
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