#include "StaticMesh.h"
#include "Graphics.h"
#include "Effects.h"
#include "Primitive.h"

StaticMesh::StaticMesh()
{
	mTexture = nullptr;
	mPrimitive = nullptr;
}
	
//! Cleanup.
StaticMesh::~StaticMesh()
{

}

//! Draws the mesh.
void StaticMesh::Draw(Graphics* pGraphics)
{
	// Set the material properties for this mesh.
	Effects::BasicFX->SetMaterial(mMaterial);
	Effects::BasicFX->SetTexture(mTexture);
	Effects::BasicFX->Apply();

	// Draw the mesh primitive.
	mPrimitive->Draw<Vertex>(pGraphics->GetContext());
}

//! Sets the primitive.
void StaticMesh::SetPrimitive(Primitive* primitive)
{
	mPrimitive = primitive;
}

//! Sets the texture by calling Graphics::LoadTexture(...).
void StaticMesh::LoadTexture(string filename)
{
	mTexture = GetGraphics()->LoadTexture(filename);
}

//! Sets the material.
void StaticMesh::SetMaterial(Material material)
{
	mMaterial = material;
}
	
//! Sets the texture.
void StaticMesh::SetTexture(Texture2D* texture)
{
	mTexture = texture;
}

//! Returns the primitive.
Primitive* StaticMesh::GetPrimitive()
{
	return mPrimitive;
}

//! Returns the material.
Material StaticMesh::GetMaterial()
{
	return mMaterial;
}