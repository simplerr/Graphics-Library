#pragma once

#include <map>
#include <string>
#include <D3DX11.h>
using namespace std;

class Primitive;

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
private:
	map<string, Primitive> mPrimitiveMap;
};

extern PrimitiveFactory* gPrimitiveFactory;