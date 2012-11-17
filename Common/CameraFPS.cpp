#include "CameraFPS.h"
#include "Input.h"


namespace GLib
{

	CameraFPS::CameraFPS() 
		: Camera()
	{
		SetRotateButton(-1);
	}

	CameraFPS::~CameraFPS()
	{

	}

	void CameraFPS::Update(Input* pInput, float dt)
	{
		// Get the look direction
		XMVECTOR direction;
		direction = XMLoadFloat3(&GetDirection());

		// Forward - backward.
		if(pInput->KeyDown('W')) 
			Move(GetDirection() * GetMovementSpeed());
		else if(pInput->KeyDown('S')) 
			Move(GetDirection() * -GetMovementSpeed());

		// Left - right.
		if(pInput->KeyDown('A')) 
			Move(GetRight() * -GetMovementSpeed());
		else if(pInput->KeyDown('D')) 
			Move(GetRight() * GetMovementSpeed());

		// Rotate the camera.
		if(mRotateButton == -1 || pInput->KeyDown(mRotateButton))
			Rotate(pInput);

		// Update the view matrix.
		UpdateViewMatrix();
	}

	void CameraFPS::SetRotateButton(int button)
	{
		mRotateButton = button;
	}
}