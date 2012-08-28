#pragma once

#include <Windows.h>
#include <xnamath.h>

class Graphics;
class Effect;
class Primitive;

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

	void SetEffect(Effect* effect);
	void SetPrimitive(Primitive* primitive);
	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);
	
private:
	Primitive* mPrimitive;
	Effect* mEffect;
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;
};