#include "Object3D.h"
#include "Graphics.h"
#include "Effects.h"
#include "Runnable.h"
#include "Primitive.h"
#include "PrimitiveFactory.h"
#include "D3DCore.h"
#include "Model.h"

Object3D::Object3D()
{
	mPrimitive	= nullptr;
	mModel		= nullptr;
	mTexture	= nullptr;
	mNormalMap	= nullptr;

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
	// Draw the model.
	if(mModel != 0)
		mModel->Draw(pGraphics, GetWorldMatrix());
	else
	// Draw the primitive with the BasicFX effect.
		pGraphics->DrawPrimitive(mPrimitive, GetWorldMatrix(), mTexture, mNormalMap, GetMaterial(), Effects::BasicFX);
	//pGraphics->DrawBoundingBox(&GetBoundingBox(), GetWorldMatrix(), Colors::Blue);
}

//! Loads the texture to pass to the shader.
void Object3D::LoadTexture(string filename, float scale)
{
	mTexture = gGame->GetGraphics()->LoadTexture(filename, scale);
}

void Object3D::SetModel(Model* model)
{
	mModel = model;
}

//! Sets the texture to pass to the shader.
void Object3D::SetTexture(Texture2D* texture)
{
	mTexture = texture;
}

void Object3D::LoadNormalMap(string filename)
{
	mNormalMap = gGame->GetGraphics()->LoadTexture(filename);
}
	
void Object3D::SetNormalMap(Texture2D* normalMap)
{
	mNormalMap = normalMap;
}

//! Sets the objects material.
void Object3D::SetMaterial(Material material)
{
	mMaterial = material;
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

Model* Object3D::GetModel()
{
	return mModel;
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

Primitive* Object3D::GetPrimitive()
{
	return mPrimitive;
}

//! Returns the bounding box in WORLD space.
AxisAlignedBox Object3D::GetBoundingBox()
{
	AxisAlignedBox aabb = mPrimitive->GetBoundingBox();

	// Break up the world matrix into it's components.
	XMVECTOR scale, rotation, translation;
	XMMatrixDecompose(&scale, &rotation, &translation, GetWorldMatrix());

	// Transform the AABB with the components.
	TransformAxisAlignedBoxCustom(&aabb, &aabb, scale, rotation, translation);

	return aabb;
}