#include "ControlManager.h"
#include "BasicControl.h"
#include "LuaWrapper.h"
#include "Input.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "D3DCore.h"

ControlManager::ControlManager(string luaScript)
{
	mLuaScript = luaScript;
	mLuaWrapper = new LuaWrapper(luaScript);
}

ControlManager::~ControlManager()
{
	delete mLuaWrapper;

	for(int i = 0; i < mControlList.size(); i++)
		delete mControlList[i];
}

void ControlManager::Update(GLib::Input* pInput, float dt)
{
	bool leftMousePressed = pInput->KeyPressed(VK_LBUTTON);
	bool rightMousePressed = pInput->KeyPressed(VK_RBUTTON);

	XMFLOAT3 mousePos = pInput->MousePosition();
	/*mousePos.x *= GLib::GetGraphics()->GetD3D()->GetDimensionRatio().right;
	mousePos.y *= GLib::GetGraphics()->GetD3D()->GetDimensionRatio().bottom;*/

	// Reload Lua script? Ctrl + R
	if(pInput->KeyDown(VK_LCONTROL) && pInput->KeyPressed('R'))
		LoadLuaProperties();

	for(int i = 0; i < mControlList.size(); i++)
	{
		BasicControl* control = mControlList[i];

		if(control->GetActivated())
			control->Update(pInput, dt);

		if(control->PointInsideControl(mousePos))
		{
			if(leftMousePressed)
			{
				// Activate the pressed control.
				DeactivateAllControls();

				// OnPress event.
				control->SetActivated(true);
				control->OnLeftBtnPressed(mousePos);
			}
			else if(rightMousePressed)
				control->OnRightBtnPressed(mousePos);
			else
				control->OnMouseHoover(mousePos);
		}
	}
}

void ControlManager::Draw(GLib::Graphics* pGraphics)
{
	for(int i = 0; i < mControlList.size(); i++)
	{
		mControlList[i]->Draw(pGraphics);
	}
}

void ControlManager::OnResize(float width, float height)
{
	for(int i = 0; i < mControlList.size(); i++)
	{
		XMFLOAT2 pos = mControlList[i]->GetPosition();
		GLib::Rect ratio = GLib::GetGraphics()->GetD3D()->GetDimensionRatio();

		float newX, newY;
		if(mControlList[i]->GetAlignment().fixedX)
			newX = pos.x;
		else 
			newX = pos.x / ratio.right;

		if(mControlList[i]->GetAlignment().fixedY)
			newY = pos.y;
		else 
			newY = pos.y / ratio.bottom;

		mControlList[i]->SetPosition(newX, newY);
		mControlList[i]->PerformLayout();
	}
}

void ControlManager::AddControl(BasicControl* pControl)
{
	mControlList.push_back(pControl);
}

void ControlManager::LoadLuaProperties()
{
	mLuaWrapper->Reload();
	for(int i = 0; i < mControlList.size(); i++)
		mControlList[i]->LoadLuaProperties(mLuaWrapper);

	OnResize(GLib::GetClientWidth(), GLib::GetClientHeight());
}

void ControlManager::DeactivateAllControls()
{
	for(int i = 0; i < mControlList.size(); i++)
		mControlList[i]->SetActivated(false);
}