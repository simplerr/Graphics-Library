#include "SkinnedModel.h"
#include "Effects.h"
#include "Camera.h"
#include "Graphics.h"
#include "ShadowMap.h"
#include "SkinnedMesh.h"
#include "cAnimationController.h"

SkinnedModel::SkinnedModel()
{
	mAnimator = nullptr;
	mElapsedTime = 0.0f;
}
	
SkinnedModel::~SkinnedModel()
{

}

void SkinnedModel::Cleanup()
{
	for(int i = 0; i < mMeshList.size(); i++) 
		delete mMeshList[i];
}

void SkinnedModel::Update(float dt)
{
	for(int i = 0; i < mMeshList.size(); i++) 
		mMeshList[i]->Update(dt);

	mElapsedTime += dt;
}

void SkinnedModel::Draw(Graphics* pGraphics, CXMMATRIX world)
{
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

	// Bone transforms.
	vector<XMFLOAT4X4> finalTransforms = mAnimator->GetTransforms(mElapsedTime);
	Effects::BasicFX->SetBoneTransforms(&finalTransforms[0], finalTransforms.size());

	// Loop through and draw each mesh.
	for(int i = 0; i < mMeshList.size(); i++)
	{
		mMeshList[i]->Draw(pGraphics);
	}
}

void SkinnedModel::SetAnimator(SceneAnimator* animator)
{
	mAnimator = animator;
}

void SkinnedModel::SetAnimation(string animation)
{
	mAnimator->SetAnimation(animation);
}
	
void SkinnedModel::SetAnimation(int index)
{
	mAnimator->SetAnimIndex(index);
}

void SkinnedModel::AddMesh(SkinnedMesh* mesh)
{
	mMeshList.push_back(mesh);
}

SkinnedMeshList* SkinnedModel::GetMeshList()
{
	return &mMeshList;
}