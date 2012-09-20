#include "Sky.h"
#include "Graphics.h"
#include "Runnable.h"
#include "PrimitiveFactory.h"
#include "Camera.h"
#include "Effects.h"
#include "Primitive.h"

Sky::Sky(string texture, float radius)
{
	mTexture = gGame->GetGraphics()->LoadTexture(texture);
	mPrimitive = gPrimitiveFactory->CreateSphere(5000.0f, 30, 30);
}
	
Sky::~Sky()
{

}

void Sky::Draw()
{
	// Center the the skybox around the camera.
	Camera* camera = gGame->GetGraphics()->GetCamera();
	XMFLOAT3 eyePos = camera->GetPosition();
	XMMATRIX T = XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);

	XMMATRIX WVP = T * XMLoadFloat4x4(&camera->GetViewMatrix()) * XMLoadFloat4x4(&camera->GetProjectionMatrix());

	Effects::SkyFX->SetWorldViewProj(WVP);
	Effects::SkyFX->SetCubeMap(mTexture->shaderResourceView);

	ID3D11DeviceContext* context = gGame->GetGraphics()->GetContext();
	context->IASetInputLayout(Effects::SkyFX->GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Effects::SkyFX->GetTech()->GetPassByIndex(0)->Apply(0, context);
	mPrimitive->Draw(context);

	mPrimitive->Draw(context);
}