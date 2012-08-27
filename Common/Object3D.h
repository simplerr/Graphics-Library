#pragma once

#include <Windows.h>
#include <xnamath.h>

class Graphics;
class Effect;
class Primitive;

class Object3D
{
public:
	Object3D();
	~Object3D();

	void Update(float dt);
	void Draw(Graphics* pGraphics);
private:
	Primitive* mPrimitive;
	Effect* mEffect;
	XMVECTOR mPosition;
};