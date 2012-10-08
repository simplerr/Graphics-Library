#include "SkinnedMesh.h"
#include "Graphics.h"
#include "Effects.h"
#include "Primitive.h"
#include <assimp\scene.h>

SkinnedMesh::SkinnedMesh()
{
	mTexture = nullptr;
	mPrimitive = nullptr;
}

SkinnedMesh::~SkinnedMesh()
{

}

void SkinnedMesh::SetVertices(ID3D11Device* device, const SkinnedVertex* vertices, int size)
{
	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SkinnedVertex) * size;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Set the init data.
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = vertices;

	// Create the vertex buffer.
	HR(device->CreateBuffer(&vbd, &initData, &mVertexBuffer));
}

void SkinnedMesh::SetIndices(ID3D11Device* device, vector<UINT> indices)
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

void SkinnedMesh::Draw(Graphics* pGraphics)
{
	// Set the material properties for this SkinnedMesh.
	Effects::BasicFX->SetMaterial(mMaterial);
	Effects::BasicFX->SetTexture(mTexture);
	Effects::BasicFX->GetSkinnedTech()->GetPassByIndex(0)->Apply(0, GetD3DContext());

	ID3D11DeviceContext* dc = pGraphics->GetContext();

	UINT stride = sizeof(SkinnedVertex);
	UINT offset = 0;
	// Set the input layout and the primitive topology.
	dc->IASetInputLayout(Effects::BasicFX->GetInputLayout());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	// [NOTE] Important with right format!! 16 or 32!
	dc->DrawIndexed(mNumIndices, 0, 0);

	// Draw the SkinnedMesh primitive.
	//mPrimitive->Draw<SkinnedVertex>(pGraphics->GetContext());
}

void SkinnedMesh::Update(float dt)
{
	
}

void SkinnedMesh::SetPrimitive(Primitive* primitive)
{
	mPrimitive = primitive;
}

void SkinnedMesh::LoadTexture(string filename)
{
	mTexture = GetGraphics()->LoadTexture(filename);
}

void SkinnedMesh::SetMaterial(Material material)
{
	mMaterial = material;
}
	
void SkinnedMesh::SetTexture(Texture2D* texture)
{
	mTexture = texture;
}

Primitive* SkinnedMesh::GetPrimitive()
{
	return mPrimitive;
}

Material SkinnedMesh::GetMaterial()
{
	return mMaterial;
}

