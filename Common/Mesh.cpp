#include "Mesh.h"
#include "Graphics.h"
#include "Effects.h"
#include "Primitive.h"

Mesh::Mesh()
{
	mTexture = nullptr;
	mPrimitive = nullptr;
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

	// Draw the mesh primitive.
	mPrimitive->Draw(pGraphics->GetContext());
}

void Mesh::SetPrimitive(Primitive* primitive)
{
	mPrimitive = primitive;
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

Primitive* Mesh::GetPrimitive()
{
	return mPrimitive;
}

Material Mesh::GetMaterial()
{
	return mMaterial;
}