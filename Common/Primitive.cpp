#include "Primitive.h"
#include "Vertex.h"
#include "d3dUtil.h"
#include <vector>

using namespace std;

Primitive::Primitive()
{

}
	
Primitive::~Primitive()
{

}

void Primitive::Cleanup()
{
	// Release the buffers.
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);
}

void Primitive::Draw(ID3D11DeviceContext* dc)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// [NOTE] Important with right format!! 16 or 32!
	dc->DrawIndexed(mNumIndices, 0, 0);
}
	
void Primitive::SetVertices(ID3D11Device* device, Vertex* vertices, UINT count)
{
	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * count;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Set the init data.
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = vertices;

	// Create the vertex buffer.
	HR(device->CreateBuffer(&vbd, &initData, &mVertexBuffer));

	mNumVertices = count;
}
	
void Primitive::SetIndices(ID3D11Device* device, const UINT* indices, UINT count)
{
	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * count;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Set the init data.
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = indices;

	// Create the index buffer.
	HR(device->CreateBuffer(&ibd, &initData, &mIndexBuffer));

	mNumIndices = count;
}