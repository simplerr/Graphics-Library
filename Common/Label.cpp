#include "Label.h"
#include "Graphics.h"
#include "d3dUtil.h"

GLib::Texture2D* texture;

Label::Label(int x, int y, string name, string text)
	: BasicControl(x, y, name)
{
	SetFontData("Arial", 30);
	SetText(text);
	SetBkgdTexture("textures/crate.dds");
	SetCustomWidth(0);
}

Label::~Label()
{

}

void Label::Draw(GLib::Graphics* pGraphics)
{
	if(IsDrawingBkgd()) 
	{
		GLib::Rect rect = GetRect();
		pGraphics->DrawScreenQuad(GetBkgdTexture(), GetPosition().x + rect.Width()/2.0f, GetPosition().y + rect.Height()/2.0f, rect.Width(), rect.Height());
	}

	if(mCentered) {
		pGraphics->DrawText(GetText(), GetPosition().x + GetRect().Width()/2 - GetTextWidth()/2, GetPosition().y + GetRect().Height()/2 - GetTextHeight()/2 - mFontData.size/5.0f, mFontData.size, mFontData.color, mFontData.family);
	}
	else
		pGraphics->DrawText(GetText(), GetPosition().x, GetPosition().y + GetRect().Height()/2 - GetTextHeight()/2 - mFontData.size/5.0f, mFontData.size, mFontData.color, mFontData.family);
}

void Label::OnLeftBtnPressed(XMFLOAT3 pos)
{

}

void Label::OnMouseHoover(XMFLOAT3 pos)
{

}

//! Returns the labels rect in global coordinates.
GLib::Rect Label::GetRect()
{
	GLib::Rect rect = GLib::GetGraphics()->MeasureText(GetText(), mFontData.size, mFontData.family);

	float x = rect.left + rect.Width()/2;
	float y = rect.top + rect.Height()/2;

	float width = rect.Width() * GetBkgdScale();
	float height = rect.Height() * GetBkgdScale();

	rect.left = x - width / 2;
	rect.right = x + width / 2;
	rect.top = y - height / 2;
	rect.bottom = y + height / 2;

	if(mCustomWidth == 0)
		return GLib::Rect(GetPosition().x, rect.right + GetPosition().x, GetPosition().y, rect.bottom + GetPosition().y);
	else
		return GLib::Rect(GetPosition().x, mCustomWidth + GetPosition().x, GetPosition().y, rect.bottom + GetPosition().y);
}

float Label::GetTextWidth()
{
	return GLib::GetGraphics()->MeasureText(GetText(), mFontData.size, mFontData.family).Width();
}

float Label::GetTextHeight()
{
	return GLib::GetGraphics()->MeasureText(GetText(), mFontData.size, mFontData.family).Height();
}

void Label::SetFontData(string family, int size, UINT32 color)
{
	mFontData.family = family;
	mFontData.size = size;
	mFontData.color = color;
}

void Label::SetText(string text)
{
	mText = text;
}

string Label::GetText()
{
	return mText;
}

void Label::SetCustomWidth(int width)
{
	mCustomWidth = width;
}

void Label::SetCentered(bool centered)
{
	mCentered = centered;
}