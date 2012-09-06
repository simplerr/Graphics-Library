#pragma once

#include <D3DX11.h>
#include <vector>
#include "Vertex.h"
using namespace std;

class Billboard;
class Graphics;
class Texture2D;

/**
	Contain a vertex buffer with points representing billboards.
*/
class BillboardManager
{
public:
	BillboardManager();
	~BillboardManager();

	void DrawAll(Graphics* pGraphics);

	void AddBillboard(Billboard* billboard);
	void BuildVertexBuffer(ID3D11Device* device);
	void RebuildVertexBuffer();
private:
	vector<Billboard*>	mBillboardList;
	ID3D11Buffer*		mVB;
	Texture2D*			mTexture;
	bool				mBuildVertexBuffer;
};