#pragma once

#include <Windows.h>
#include <xnamath.h>
#include "d3dx11Effect.h"
#include "Light.h"
#include "xnacollision.h"

using namespace XNA;

class Graphics;
class BasicEffect;
class Primitive;
class Model;
struct Texture2D;

/**
	Base class for all objects.
	Contains a primitive, effect and position information.
*/
class Object3D
{
public:
	Object3D();
	~Object3D();

	void Update(float dt);
	void Draw(Graphics* pGraphics);

	// Getters & setters.
	XMMATRIX GetWorldMatrix();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();
	Material GetMaterial();
	AxisAlignedBox GetBoundingBox();
	Primitive* GetPrimitive();

	void SetModel(Model* model);
	void LoadTexture(string filename, float scale = 1.0f);
	void SetTexture(Texture2D* texture);
	void LoadNormalMap(string filename);
	void SetNormalMap(Texture2D* normalMap);
	void SetPrimitive(Primitive* primitive);
	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);
	void SetMaterial(Material material);
private:
	Texture2D*	mTexture;
	Texture2D*	mNormalMap;
	Primitive*	mPrimitive;
	Model*		mModel;
	Material	mMaterial;
	XMFLOAT3	mPosition;
	XMFLOAT3	mRotation;
	XMFLOAT3	mScale;
};