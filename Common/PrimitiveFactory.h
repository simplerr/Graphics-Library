#pragma once

#include <map>
#include <string>
#include <D3DX11.h>
using namespace std;

class Primitive;

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