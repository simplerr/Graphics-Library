#pragma once

#include <string>
using namespace std;

class Primitive;
class Texture2D;

class Sky
{
public:
	Sky(string texture, float radius);
	~Sky();

	void Draw();
private:
	Primitive* mPrimitive;
	Texture2D* mTexture;
};