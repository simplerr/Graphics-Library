#include "Button.h"
#include "Graphics.h"
#include "LuaWrapper.h"

Button::Button(int x, int y, string name, string text)
	: Label(x, y, name, text)
{
	SetDisabled(false);
	mMouseOver = false;
}

Button::~Button()
{

}

void Button::Update(GLib::Input* pInput, float dt)
{
	mMouseOver = false;
}

void Button::Draw(GLib::Graphics* pGraphics)
{
	GLib::Rect rect = GetRect();
	if(GetDisabled())
		pGraphics->DrawScreenQuad(mDisabledTexture, GetPosition().x, GetPosition().y, rect.Width(), rect.Height());
	else 
	{
		if(mMouseOver)
			pGraphics->DrawScreenQuad(mHooverTexture, GetPosition().x, GetPosition().y, rect.Width(), rect.Height());
		else
			pGraphics->DrawScreenQuad(GetBkgdTexture(), GetPosition().x, GetPosition().y, rect.Width(), rect.Height());
	}

	Label::Draw(pGraphics);
}

void Button::OnLeftBtnPressed(XMFLOAT3 pos)
{
	if(!OnPressed.empty() && !mDisabled)
		OnPressed(this);
}

void Button::OnMouseHoover(XMFLOAT3 pos)
{
	mMouseOver = true;
}

void Button::LoadLuaProperties(LuaWrapper* pLuaWrapper)
{
	Label::LoadLuaProperties(pLuaWrapper);

	string defaultTexture = pLuaWrapper->GetTableString(GetName(), "default_texture");
	string disabledTexture = pLuaWrapper->GetTableString(GetName(), "disabled_texture");
	string hooverTexture = pLuaWrapper->GetTableString(GetName(), "hoover_texture");

	SetBkgdTexture(defaultTexture);
	mDisabledTexture = GLib::GetGraphics()->LoadTexture(disabledTexture);
	mHooverTexture = GLib::GetGraphics()->LoadTexture(hooverTexture);
}

void Button::SetDisabled(bool disabled)
{
	mDisabled = disabled;
}

bool Button::GetDisabled()
{
	return mDisabled;
}