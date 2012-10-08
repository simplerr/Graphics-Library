#include "SkinnedMesh.h"
#include "Graphics.h"
#include "Effects.h"
#include "Primitive.h"
#include <assimp\scene.h>
#include <fstream>

SkinnedMesh::SkinnedMesh()
{
	mTexture = nullptr;
	mPrimitive = nullptr;
}

SkinnedMesh::~SkinnedMesh()
{

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

	// Draw the SkinnedMesh primitive.
	mPrimitive->Draw<SkinnedVertex>(pGraphics->GetContext());
}

void SkinnedMesh::Update(float dt)
{
	
}

void SkinnedMesh::Save(ofstream& fout)
{

}
	
void SkinnedMesh::Load(ifstream& fin)
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