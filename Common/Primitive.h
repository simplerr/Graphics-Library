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
	template <class VertexType>
	Primitive(ID3D11Device* device, vector<VertexType> vertices, vector<UINT> indices);
	Primitive();
	~Primitive();

	void Cleanup();

	template <class VertexType>
	void SetVertices(ID3D11Device* device, vector<VertexType> vertices);
	void SetIndices(ID3D11Device* device, vector<UINT> indices);

	void Draw(ID3D11DeviceContext* dc);

	AxisAlignedBox GetBoundingBox();
private:
	ID3D11Buffer*	mVertexBuffer;
	ID3D11Buffer*	mIndexBuffer;
	AxisAlignedBox	mBoundingBox;
	UINT mNumVertices;
	UINT mNumIndices;
};

/**
	Template functions has to be in the header. 
*/
template <class VertexType>
Primitive::Primitive(ID3D11Device* device, vector<VertexType> vertices, vector<UINT> indices)
{
	SetVertices(device, vertices);
	SetIndices(device, indices);
}

template <class VertexType>
void Primitive::SetVertices(ID3D11Device* device, vector<VertexType> vertices)
{
	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexType) * vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Set the init data.
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = &vertices[0];

	// Create the vertex buffer.
	HR(device->CreateBuffer(&vbd, &initData, &mVertexBuffer));

	mNumVertices = vertices.size();

	// Compute the AABB.
	XNA::ComputeBoundingAxisAlignedBoxFromPoints(&mBoundingBox, vertices.size(), &vertices[0].Pos, sizeof(VertexType));
}