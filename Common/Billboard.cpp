#include "Billboard.h"
#include "Graphics.h"

Billboard::Billboard(Graphics* pGraphics, XMFLOAT3 position, XMFLOAT2 size, string texture)
{
	SetPosition(position);
	SetSize(size);
	SetTexture(pGraphics->LoadTexture(texture));
}
	
Billboard::~Billboard()
{

}

void Billboard::SetPosition(XMFLOAT3 position)
{
	mVertex.Pos = position;
}
	
void Billboard::SetSize(XMFLOAT2 size)
{
	mVertex.Size = size;
}

void Billboard::SetTexture(Texture2D* texture)
{
	mTexture = texture;
}

XMFLOAT3 Billboard::GetPosition()
{
	return mVertex.Pos;
}
	
XMFLOAT2 Billboard::GetSize()
{
	return mVertex.Size;
}