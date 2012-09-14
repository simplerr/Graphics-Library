#pragma once

#include <map>
#include <string>
#include <D3DX11.h>
#include <xnamath.h>
using namespace std;

class Primitive;
class Terrain;

/**
	Handles creation of different primitives.
	Stores each primitive in a map to enable instancing.
*/
class PrimitiveFactory
{
public:
	PrimitiveFactory();
	~PrimitiveFactory();

	Primitive* CreateBox();
	Primitive* CreateGrid(float width, float depth, UINT m, UINT n);
	Primitive* CreateQuad();
	Primitive* CreateSphre(float radius, UINT sliceCount, UINT stackCount);
	Primitive* CreateTerrain(Terrain* terrain);

	// For grid landscapes.
	float GetHeight(float x, float z);
	XMFLOAT3 GetHillNormal(float x, float z);
private:
	map<string, Primitive> mPrimitiveMap;
};

extern PrimitiveFactory* gPrimitiveFactory;