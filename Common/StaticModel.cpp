#include "StaticModel.h"
#include "Effects.h"
#include "Camera.h"
#include "Graphics.h"
#include "ShadowMap.h"
#include "StaticMesh.h"

StaticModel::StaticModel()
{

}
	
//! Cleanup.
StaticModel::~StaticModel()
{
	for(int i = 0; i < mMeshList.size(); i++) 
		delete mMeshList[i];
}

//! Draws all the meshes.
void StaticModel::Draw(Graphics* pGraphics, CXMMATRIX world)
{
	// Set the input layout and the primitive topology.
	GetD3DContext()->IASetInputLayout(Effects::BasicFX->GetInputLayout());
	GetD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX view = XMLoadFloat4x4(&pGraphics->GetCamera()->GetViewMatrix());
	XMMATRIX proj = XMLoadFloat4x4(&pGraphics->GetCamera()->GetProjectionMatrix());
	Effects::BasicFX->SetWorld(world);
	Effects::BasicFX->SetWorldViewProj(world*view*proj);
	Effects::BasicFX->SetWorldInvTranspose(InverseTranspose(world));
	Effects::BasicFX->SetEyePosition(pGraphics->GetCamera()->GetPosition());
	Effects::BasicFX->SetLights(pGraphics->GetLightList());
	Effects::BasicFX->SetNormalMap(0);
	Effects::BasicFX->SetShadowMap(pGraphics->GetShadowMap()->GetSRV());
	Effects::BasicFX->SetShadowTransform(world * XMLoadFloat4x4(&pGraphics->GetShadowMap()->GetShadowTransform()));
	Effects::BasicFX->SetFogColor(pGraphics->GetFogColor());
	Effects::BasicFX->SetFogStart(1000.0f);
	Effects::BasicFX->SetFogRange(50.0f);
	Effects::BasicFX->SetUseAnimation(false);

	// Loop through and draw each mesh.
	for(int i = 0; i < mMeshList.size(); i++)
	{
		mMeshList[i]->Draw(pGraphics);
	}
}

//! Adds a mesh to the list.
void StaticModel::AddMesh(StaticMesh* mesh)
{
	mMeshList.push_back(mesh);
}

//! Returns the mesh list.
MeshList* StaticModel::GetMeshList()
{
	return &mMeshList;
}