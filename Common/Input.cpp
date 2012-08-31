#include "Input.h"
#include "Runnable.h"

//! Constructor.
Input::Input()
{
	// Set every key/button 0.
	ZeroMemory(mLastKeyState, sizeof(mLastKeyState));
	ZeroMemory(mKeyState, sizeof(mKeyState));

	HR(DirectInput8Create(gGame->GetInstance(), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&mDInput, 0));

	// Get the cursor starting position.
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	mMousePosition.x = mousePosition.x;
	mMousePosition.y = mousePosition.y;

	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(gGame->GetHwnd(), DISCL_NONEXCLUSIVE|DISCL_FOREGROUND));
	HR(mMouse->Acquire());

	// No delta movement to start with
	mDx = mDy = 0.0f;
}

//! Destructor.
Input::~Input()
{
	ReleaseCOM(mDInput);
	mMouse->Unacquire();
	ReleaseCOM(mMouse);
}

//! Update the key state.
/**
@param dt The delta time since the last frame.
*/
void Input::Update(float dt)
{
	// Set the old states.
	memcpy(mLastKeyState, mKeyState, sizeof(mKeyState));

	// Get the current keyboard state.
	GetKeyboardState(mKeyState);
}

//! Draws debug information.
void Input::Draw()
{

}

//! Updates the mouse position.
/**
@param msg The message.
@param wParam Message data.
@param lParam Message data.
*/
LRESULT Input::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_MOUSEMOVE:	{
		SetMousePosition(XMFLOAT3(LOWORD(lParam), HIWORD(lParam), 0));
		return 0;
		}
	case WM_LBUTTONDOWN:	{
		//showMsg("doo");
		return 0;
		}
	}
}

//! Checks if the key was pressed.
/**
@param key The to check.
@return True if pressed.
*/
bool Input::KeyPressed(int key)
{
	if(key > 255 || key < 0)
		return false;

	// Usees bitwise AND to remove the least significant bit which is set if the key was pressed after the previous call to GetAsyncKeyState.
	// See http://msdn.microsoft.com/en-us/library/windows/desktop/ms646293(v=VS.85).aspx for more info.
	// The same thing in keyDown() and keyReleased()
	return (mKeyState[key] & 0x80) && !(mLastKeyState[key] & 0x80);
}

//! Checks if the key is down.
/**
@param key The to check.
@return True if down.
*/
bool Input::KeyDown(int key)
{
	if(key > 255 || key < 0)
		return false;

	return mKeyState[key] & 0x80;
}

//! Checks if the key was released.
/**
@param key The to check.
@return True if released.
*/
bool Input::KeyReleased(int key)
{
	if(key > 255 || key < 0)
		return false;

	return !(mKeyState[key] & 0x80) && (mLastKeyState[key] & 0x80);
}

//! Get the mouse position
/**
@return The mouse position.
*/
XMFLOAT3 Input::MousePosition()
{
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(gGame->GetHwnd(), &pos);

	RECT r;
	GetWindowRect(gGame->GetHwnd(), &r);
	float width = r.right - r.left;
	float height = r.bottom - r.top;

	GetClientRect(gGame->GetHwnd(), &r);
	XMFLOAT3 position = mMousePosition;
	position.x = (mMousePosition.x / (float)r.right) * width;
	position.y = (mMousePosition.y / (float)r.bottom) * height;

	//position.x /= (gGame->widthRatio();
	//position.y /= gGame->heightRatio();
	return position;
}

//! Set the mouse position.
/**
@param pos The new position.
@note Doesn't acctually change the mouse position on the screen, just the data.
*/
void Input::SetMousePosition(XMFLOAT3 pos)
{
	mDx = pos.x - MousePosition().x;
	mDy = pos.y - MousePosition().y;

	mMousePosition = pos;
}

//! Returns horizontal delta movement.
float Input::MouseDx()
{
	return (float)mMouseState.lX;
}

//! Returns vertical delta movement.
float Input::MouseDy()
{
	return (float)mMouseState.lY;
}

float Input::MouseDz()
{
	return (float)mMouseState.lZ;
}

void Input::Poll()
{
	// Poll mouse.
	memcpy(&mLastMouseState, &mMouseState, sizeof(mMouseState));
	HRESULT hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState); 
	if( FAILED(hr) )
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to acquire for next time we poll.
		hr = mMouse->Acquire(); 
	}
}

void Input::GetWorldPickingRay(D3DXVECTOR3& originW, D3DXVECTOR3& dirW)
{
	//// Get the screen point clicked.
	//POINT s;
	//GetCursorPos(&s);

	//// Make it relative to the client area window.
	//ScreenToClient(gGame->getMainWnd(), &s);

	//// By the way we've been constructing things, the entire 
	//// backbuffer is the viewport.
	//float w = (float)gGame->getScreenWidth();
	//float h = (float)gGame->getScreenHeight();

	//D3DXMATRIX proj = gCamera->getProjectionMatrix();

	//float x = (2.0f*s.x/w - 1.0f) / proj(0,0);
	//float y = (-2.0f*s.y/h + 1.0f) / proj(1,1);

	//// Build picking ray in view space.
	//D3DXVECTOR3 origin(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 dir(x, y, 1.0f);

	//// So if the view matrix transforms coordinates from 
	//// world space to view space, then the inverse of the
	//// view matrix transforms coordinates from view space
	//// to world space.
	//D3DXMATRIX invView;
	//D3DXMatrixInverse(&invView, 0, &gCamera->getViewMatrix());

	//// Transform picking ray to world space.
	//D3DXVec3TransformCoord(&originW, &origin, &invView);
	//D3DXVec3TransformNormal(&dirW, &dir, &invView);
	//D3DXVec3Normalize(&dirW, &dirW);
}