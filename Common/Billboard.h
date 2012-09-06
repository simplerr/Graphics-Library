#pragma once
#include "Vertex.h"
#include <string>
using namespace std;

class Graphics;
struct Texture2D;

class Billboard
{
public:
	Billboard(Graphics* pGraphics, XMFLOAT3 position, XMFLOAT2 size, string texture);
	~Billboard();

	void SetPosition(XMFLOAT3 position);
	void SetSize(XMFLOAT2 size);
	void SetTexture(Texture2D* texture);

	XMFLOAT3 GetPosition();
	XMFLOAT2 GetSize();
private:
	BillboardVertex		mVertex;
	Texture2D*			mTexture;
};