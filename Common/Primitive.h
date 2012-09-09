#pragma once

#include <D3DX11.h>
#include <vector>
#include "xnacollision.h"
using namespace std;
using namespace XNA;
struct Vertex;

/**
	Contains a vertex and index buffer.
*/
class Primitive
{
public:
	Primitive();
	~Primitive();

	void Cleanup();

	void SetVertices(ID3D11Device* device, vector<Vertex> vertices, UINT count);
	void SetIndices(ID3D11Device* device, const UINT* indices, UINT count);

	void Draw(ID3D11DeviceContext* dc);

	AxisAlignedBox GetBoundingBox();
private:
	ID3D11Buffer*	mVertexBuffer;
	ID3D11Buffer*	mIndexBuffer;
	AxisAlignedBox	mBoundingBox;
	UINT mNumVertices;
	UINT mNumIndices;
};