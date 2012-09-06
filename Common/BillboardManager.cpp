#include "BillboardManager.h"
#include "Billboard.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "D3DCore.h"
#include "Effects.h"
#include "Runnable.h"

BillboardManager::BillboardManager()
{
	mBuildVertexBuffer = false;
	mTexture = gGame->GetGraphics()->LoadTexture("textures\\grass.png");
}
	
BillboardManager::~BillboardManager()
{

}

//! Adds a billboard.
void BillboardManager::AddBillboard(Billboard* billboard)
{
	mBillboardList.push_back(billboard);
	BuildVertexBuffer(gGame->GetD3D()->GetDevice());	//  [HACK]
}
	
//!  Draws all the billboards.
void BillboardManager::DrawAll(Graphics* pGraphics)
{
	if(mBuildVertexBuffer)
		BuildVertexBuffer(pGraphics->GetD3D()->GetDevice());

	// Draw all vertices in the buffer.
	pGraphics->DrawBillboards(mVB, mTexture, mBillboardList.size());

	mBuildVertexBuffer = false;
}

//! Builds the vertex buffer.
void BillboardManager::BuildVertexBuffer(ID3D11Device* device)
{
	BillboardVertex* vertices = new BillboardVertex[mBillboardList.size()];

	// Copy over the position and size.
	for(int i = 0; i < mBillboardList.size(); i++)
	{
		vertices[i].Pos	 = mBillboardList[i]->GetPosition();
		vertices[i].Size = mBillboardList[i]->GetSize();
	}

	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(BillboardVertex) * mBillboardList.size();
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
void BillboardManager::RebuildVertexBuffer()
{
	mBuildVertexBuffer = true;
}