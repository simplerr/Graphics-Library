#include "TextMenu.h"
#include "Label.h"
#include "Graphics.h"
#include "LuaWrapper.h"
#include "d3dUtil.h"

TextMenu::TextMenu(float x, float y, string name)
	: BasicControl(x, y, name)
{
	SetBkgdTexture("textures/snow.dds");
	SetDrawBkgd(true);
	SetFontData("Arial", 30, GLib::ColorRGBA(0, 0, 0, 255));
	SetSpacing(20.0f);
	SetCenteredItems(false);
}

TextMenu::~TextMenu()
{

}

void TextMenu::Update(GLib::Input* pInput, float dt)
{
	DeactivateAllItems();
}

void TextMenu::Draw(GLib::Graphics* pGraphics)
{
	if(IsDrawingBkgd()) {
		GLib::Rect rect = GetRect();
		pGraphics->DrawScreenQuad(GetBkgdTexture(), GetPosition().x + rect.Width()/2.0f, GetPosition().y + rect.Height()/2.0f, rect.Width()*mBkgdScale, rect.Height()*mBkgdScale);
	}

	for(int i = 0; i < mItemList.size(); i++)
		mItemList[i]->Draw(pGraphics);
}

void TextMenu::AddItem(Label* pItem)
{
	pItem->SetFontData(mFontData.family, mFontData.size, mFontData.color);
	pItem->SetParent(this);
	mItemList.push_back(pItem);

	//PerformLayout();
}

void TextMenu::AddItem(string name, string text)
{
	Label* label = new Label(0, 0, name, text);
	AddItem(label);
}

void TextMenu::LoadLuaProperties(LuaWrapper* pLuaWrapper)
{
	float x = pLuaWrapper->GetTableNumber(GetName(), "pos_x");
	float y = pLuaWrapper->GetTableNumber(GetName(), "pos_y");
	float spacing = pLuaWrapper->GetTableNumber(GetName(), "spacing");
	float fontSize = pLuaWrapper->GetTableNumber(GetName(), "font_size");
	float bkgdScale = pLuaWrapper->GetTableNumber(GetName(), "bkgd_scale");
	string fontFamily = pLuaWrapper->GetTableString(GetName(), "font_family");
	string color = pLuaWrapper->GetTableString(GetName(), "font_color");
	string itemBkgd = pLuaWrapper->GetTableString(GetName(), "item_bkgd");
	string menuBkgd = pLuaWrapper->GetTableString(GetName(), "menu_bkgd");
	int drawBkgd = pLuaWrapper->GetTableNumber(GetName(), "draw_bkgd");
	int centeredItems = pLuaWrapper->GetTableNumber(GetName(), "centered_items");
	float itemBkgdScale = pLuaWrapper->GetTableNumber(GetName(), "item_bkgd_scale");

	SetBkgdScale(bkgdScale);
	SetBkgdTexture(menuBkgd);
	SetDrawBkgd(drawBkgd == 1 ? true : false);
	SetPosition(x, y);
	SetSpacing(spacing);
	SetCenteredItems(centeredItems);
	SetFontData(fontFamily, fontSize, GLib::StripRGBA(color));

	for(int i = 0; i < mItemList.size(); i++) {
		mItemList[i]->SetBkgdTexture(itemBkgd);
		mItemList[i]->SetBkgdScale(itemBkgdScale);
	}

	PerformLayout();
}

void TextMenu::PerformLayout()
{
	// Find the widest menu item.
	float maxWidth = -1;
	for(int i = 0; i < mItemList.size(); i++) {
		mItemList[i]->SetCustomWidth(0);

		if(mItemList[i]->GetRect().Width() > maxWidth)
			maxWidth = mItemList[i]->GetRect().Width();
	}

	if(mItemList.size() != 0)
	{
		mItemList[0]->SetCentered(mCenteredItems);
		mItemList[0]->SetPosition(0, 0);
		mItemList[0]->SetCustomWidth(maxWidth);

		for(int i = 1; i < mItemList.size(); i++)
		{
			float lastHeight = mItemList[i-1]->GetRect().Height();
			mItemList[i]->SetPosition(0, mItemList[i-1]->GetPosition().y - GetPosition().y + lastHeight + mSpacing);
			mItemList[i]->SetCustomWidth(maxWidth);
			mItemList[i]->SetCentered(mCenteredItems);
		}
	}
}

void TextMenu::OnLeftBtnPressed(XMFLOAT3 pos)
{
	for(int i = 0; i < mItemList.size(); i++)
	{
		if(mItemList[i]->PointInsideControl(pos)) {
			// Callback.
			if(!OnItemPressed.empty())
				OnItemPressed(mItemList[i]);
		}
	}
}

void TextMenu::OnMouseHoover(XMFLOAT3 pos)
{
	for(int i = 0; i < mItemList.size(); i++)
	{
		if(mItemList[i]->PointInsideControl(pos)) 
			mItemList[i]->SetDrawBkgd(true);
	}
}

void TextMenu::SetFontData(string family, int size, UINT32 color)
{
	mFontData = FontData(family, size, color);

	for(int i = 0; i < mItemList.size(); i++)
		mItemList[i]->SetFontData(mFontData.family, mFontData.size, mFontData.color);
}

GLib::Rect TextMenu::GetRect()
{
	GLib::Rect rect(999999, -999999, 9999999, -99999999);

	for(int i = 0; i < mItemList.size(); i++)
	{
		rect.left = min(rect.left, mItemList[i]->GetRect().left);
		rect.right = max(rect.right, mItemList[i]->GetRect().right);
		rect.top = min(rect.top, mItemList[i]->GetRect().top);
		rect.bottom = max(rect.bottom, mItemList[i]->GetRect().bottom);
	}

	return rect;
}

void TextMenu::DeactivateAllItems()
{
	for(int i = 0; i < mItemList.size(); i++)
		mItemList[i]->SetDrawBkgd(false);
}

void TextMenu::SetSpacing(float spacing)
{
	mSpacing = spacing;
}

void TextMenu::SetBkgdScale(float scale)
{
	mBkgdScale = scale;
}

void TextMenu::SetCenteredItems(bool centered)
{
	mCenteredItems = centered;
}