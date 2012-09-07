#include "BillboardManager.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "D3DCore.h"
#include "Effects.h"
#include "Runnable.h"

BillboardManager::BillboardManager(string texture)
{
	mRebuild = false;
	mTexture = gGame->GetGraphics()->LoadTexture(texture);
}
	
//! Cleanup.
BillboardManager::~BillboardManager()
{
	for(int i = 0; i < mBillboardList.size(); i++)
		delete mBillboardList[i];

	ReleaseCOM(mVB);
}

//! Adds a billboard.
void BillboardManager::AddBillboard(BillboardVertex* billboard)
{
	mBillboardList.push_back(billboard);
	BuildVertexBuffer(gGame->GetD3D()->GetDevice());	//  [HACK]
}

//! Builds the vertex buffer.
void BillboardManager::BuildVertexBuffer(ID3D11Device* device)
{
	BillboardVertex* vertices = new BillboardVertex[mBillboardList.size()];

	// Copy over the position and size.
	for(int i = 0; i < mBillboardList.size(); i++)
	{
		vertices[i].Pos	 = mBillboardList[i]->Pos;
		vertices[i].Size = mBillboardList[i]->Size;
	}

	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(BillboardVertex) * mBillboardList.size() - sizeof(BillboardManager*) * mBillboardList.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Set the init data.
    D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices;

	// Create the vertex buffer.
	HR(device->CreateBuffer(&vbd, &initData, &mVB));

	// Cleanup.
	delete [] vertices;
}

//! Called when a billboard has moved in order to update it's position.
void BillboardManager::SetRebuild(bool rebuild)
{
	mRebuild = rebuild;
}

//! Returns the vertex buffer.
ID3D11Buffer* BillboardManager::GetVertexBuffer()
{
	return mVB;
}

//! Returns the number of billboards
int BillboardManager::GetNumBillboards()
{
	return mBillboardList.size();
}

//! Returns the texture.
Texture2D* BillboardManager::GetTexture()
{
	return mTexture;
}

//! Returns true if the vertex buffer should get rebuilt.
bool BillboardManager::GetRebuild()
{
	return mRebuild;
}