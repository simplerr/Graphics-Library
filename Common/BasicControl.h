#pragma once
#include <string>
#include "d3dUtil.h"

using namespace std;

namespace GLib {
	class Input;
	class Graphics;
	class Texture2D;
}

class LuaWrapper;

struct FontData
{
	FontData() {};
	FontData(string family, int size, UINT32 color) {
		this->family = family;
		this->size = size;
		this->color = color;
	}

	string family;
	int size;
	UINT32 color;
};

class BasicControl
{
public:
	BasicControl(float x, float y, string name);
	~BasicControl();

	virtual void Update(GLib::Input* pInput, float dt) {};
	virtual void Draw(GLib::Graphics* pGraphics) = 0;
	virtual GLib::Rect GetRect() = 0;
	virtual void OnLeftBtnPressed(XMFLOAT3 pos) = 0;
	virtual void OnRightBtnPressed(XMFLOAT3 pos) {};
	virtual void OnMouseHoover(XMFLOAT3 pos) = 0;
	virtual void LoadLuaProperties(LuaWrapper* pLuaWrapper) {};
	virtual void PerformLayout() {};

	void SetParent(BasicControl* pParent);
	void SetBkgdTexture(string textureName);
	void SetDrawBkgd(bool drawBkgd);
	void SetName(string name);
	void SetActivated(bool activated);
	void SetPosition(float x, float y);
	void SetBkgdScale(float scale);

	XMFLOAT2 GetPosition();
	GLib::Texture2D* GetBkgdTexture();
	BasicControl* GetParent();
	string GetName();
	bool GetActivated();
	float GetBkgdScale();
	bool IsDrawingBkgd();
	

	bool PointInsideControl(XMFLOAT3 point);

private:
	GLib::Texture2D* mBkgdTexture;
	XMFLOAT2 mPosition;
	BasicControl* mParent;
	string mName;
	string mLuaScript;
	bool mDrawBkgdTexture;
	bool mActivated;
	float mBkgdScale;
};