#include "Object3D.h"
#include "Graphics.h"
#include "Effect.h"
#include "Runnable.h"
#include "../Primitive.h"
#include "PrimitiveFactory.h"
#include "D3DCore.h"

XMMATRIX view;
XMMATRIX proj;

Object3D::Object3D()
{
	// Load the effect.
	mEffect = gGame->GetGraphics()->LoadEffect("color.fx");

	//mPosition = XMVectorZero();

	// Create the primitive.
	mPrimitive = gPrimitiveFactory->CreateBox();

	// Build the view matrix.
	XMVECTOR pos    = XMVectorSet(6, 6, 6, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	view = XMMatrixLookAtLH(pos, target, up);

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	proj = XMMatrixPerspectiveFovLH(0.25f*3.14f, 800.0f/600.0f, 1.0f, 1000.0f);
}
	
Object3D::~Object3D()
{

}

void Object3D::Update(float dt)
{

}
	
void Object3D::Draw(Graphics* pGraphics)
{
	ID3D11DeviceContext* context = pGraphics->GetD3D()->GetContext();

	// Set the input layout and the primitive topology.
	context->IASetInputLayout(mEffect->GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the effect variables.
	mEffect->SetWorldViewProj(&(view * proj));
	mEffect->Apply();

	// Call the primitives draw function.
	mPrimitive->Draw(context);
}