#include "Mesh.h"
#include "Graphics.h"
#include "Effects.h"

Mesh::Mesh()
{
	mTexture = nullptr;
}
	

Mesh::~Mesh()
{

}

void Mesh::Draw(Graphics* pGraphics)
{
	// Set the material properties for this mesh.
	Effects::BasicFX->SetMaterial(mMaterial);
	Effects::BasicFX->SetTexture(mTexture);
	Effects::BasicFX->Apply();

	ID3D11DeviceContext* context = pGraphics->GetContext();

	// Set the vertex and index buffers to the Input Assembler stage.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// [NOTE] Important with right format!! 16 or 32!

	// Draw the mesh.
	context->DrawIndexed(mNumIndices, 0, 0);
}

void Mesh::SetVertices(ID3D11Device* device, vector<Vertex> vertices)
{
	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
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
	XNA::ComputeBoundingAxisAlignedBoxFromPoints(&mBoundingBox, vertices.size(), &vertices[0].Pos, sizeof(Vertex));
}
	
void Mesh::SetIndices(ID3D11Device* device, vector<UINT> indices)
{
	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Set the init data.
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = &indices[0];

	// Create the index buffer.
	HR(device->CreateBuffer(&ibd, &initData, &mIndexBuffer));

	mNumIndices = indices.size();
}

void Mesh::LoadTexture(string filename)
{
	mTexture = GetGraphics()->LoadTexture(filename);
}

void Mesh::SetMaterial(Material material)
{
	mMaterial = material;
}
	
void Mesh::SetTexture(Texture2D* texture)
{
	mTexture = texture;
}

