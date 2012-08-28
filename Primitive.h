#pragma once

#include <D3DX11.h>
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

	void SetVertices(ID3D11Device* device, Vertex* vertices, UINT count);
	void SetIndices(ID3D11Device* device, const UINT* indices, UINT count);

	void Draw(ID3D11DeviceContext* dc);
private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	UINT mNumVertices;
	UINT mNumIndices;
};