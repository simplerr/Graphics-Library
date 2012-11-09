#pragma once

#include <string>
using namespace std;

class Primitive;
class Texture2D;
class PrimitiveFactory;
class Graphics;

class Sky
{
public:
	Sky(Graphics* pGraphics, string texture, float radius);
	~Sky();

	void Draw(Graphics* pGraphics);
private:
	Primitive* mPrimitive;
	Texture2D* mTexture;
};