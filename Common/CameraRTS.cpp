#include "CameraRTS.h"
#include "Input.h"

namespace GLib
{

CameraRTS::CameraRTS() 
	: Camera()
{
	SetMovementSpeed(0.2f);
}

CameraRTS::~CameraRTS()
{

}

void CameraRTS::Update(Input* pInput, float dt)
{
	XMFLOAT3 mousePos = pInput->MousePosition();

	int scrollBarSize = 30.0f;
	if(mousePos.x < scrollBarSize || mousePos.x > 65000) // [NOTE] For some reason mousePos.x can be 65 534 when outside the window.
		Move(XMFLOAT3(1, 0, 0) * GetMovementSpeed());
	else if(mousePos.x > GetClientWidth() - scrollBarSize)
		Move(XMFLOAT3(-1, 0, 0) * GetMovementSpeed());

	if(mousePos.y < scrollBarSize)
		Move(XMFLOAT3(0, 0, -1) * GetMovementSpeed());
	else if(mousePos.y > GetClientHeight() - scrollBarSize)
		Move(XMFLOAT3(0, 0, 1) * GetMovementSpeed());

	Move(GetDirection() * pInput->MouseDz() * 0.10f);

	// Update the view matrix.
	UpdateViewMatrix();
}

}