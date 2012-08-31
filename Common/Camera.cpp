#include "Camera.h"
#include "Input.h"
#include "Graphics.h"
#include "Runnable.h"

Camera::Camera()
{
	// Set the sensitivity and speed
	SetLookSensitivity(2.0f);
	SetMoveSpeed(0.03f);

	// Default position and target
	mPosition	= XMFLOAT3(32, 32, 32);
	mTarget		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	mUp			= XMFLOAT3(0.0f, 1.0f, 0.0f);		// Weird up vector

	// Calculate the new direction.
	UpdatePitchYaw();

	// Build the projection matrix
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PI * 0.25, (float)gGame->GetScreenWidth()/(float)gGame->GetScreenHeight(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, proj);

	UpdateViewMatrix();
}

Camera::~Camera()
{

}

//! Rotates, moves and updates the view matrix.
void Camera::Update(float dt)
{
	// Rotate and move
	Move();
	Rotate();

	// Update the view matrix.
	UpdateViewMatrix();
}

//! Updates the view matrix.
void Camera::UpdateViewMatrix()
{
	// Update the right vector.
	XMVECTOR up = XMLoadFloat3(&mUp);
	XMVECTOR dir = XMLoadFloat3(&GetDirection());
	XMVECTOR right = XMVector3Cross(up, dir);
	right = XMVector3Normalize(right);
	XMStoreFloat3(&mRight, right);

	// Update the view matrix
	XMVECTOR pos = XMLoadFloat3(&mPosition);
	XMVECTOR target = XMLoadFloat3(&mTarget);
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(pos, target, up));
}

//! Reads keyboard input and moves the camera correspondingly.
void Camera::Move()
{
	// Get the look direction
	XMVECTOR direction;
	direction = XMLoadFloat3(&GetDirection());

	// Fast scroll zooming
	mPosition += direction * gInput->MouseDz() / 8;
	mTarget += direction * gInput->MouseDz() / 8;

	// Check for keypresses
	if(gInput->KeyDown('W')) {
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) + direction*mVelocity);
		XMStoreFloat3(&mTarget, XMLoadFloat3(&mTarget) + direction*mVelocity);
	}
	else if(gInput->KeyDown('S')) {
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) - direction*mVelocity);
		XMStoreFloat3(&mTarget, XMLoadFloat3(&mTarget) - direction*mVelocity);
	}

	if(gInput->KeyDown('A')) {
		XMVECTOR right = XMLoadFloat3(&GetRight());
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) - right*mVelocity);
		XMStoreFloat3(&mTarget, XMLoadFloat3(&mTarget) - right*mVelocity);
	}
	else if(gInput->KeyDown('D')) {
		XMVECTOR right = XMLoadFloat3(&GetRight());
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) + right*mVelocity);
		XMStoreFloat3(&mTarget, XMLoadFloat3(&mTarget) + right*mVelocity);
	}
}

//! Reads mouse movement and rotates the camera correspondingly.
void Camera::Rotate()
{
	// Increase the pitch and yaw angles.
	mPitch += gInput->MouseDy() / (-13.0f / mSensitivity);
	mYaw += gInput->MouseDx() / (13.0f / mSensitivity);

	// Limit to PI/2 radians.
	if(mPitch > 0)
		mPitch = min(mPitch, 1.54f);
	else
		mPitch = max(mPitch, -1.54f);

	// Calculate the new direction.
	XMVECTOR direction;
	float r = cosf(mPitch);
	direction = XMVectorSet(r * sinf(mYaw), sinf(mPitch), r * cosf(mYaw), 0);

	// Set the new target.
	XMStoreFloat3(&mTarget, XMLoadFloat3(&mPosition) + direction);
}

//! Outputs debug information in the Output window.
void Camera::DrawDebug()
{
	char buffer[256];
	sprintf(buffer, "Pos: (%f, %f, %f)\n", mPosition.x, mPosition.y, mPosition.z);
	OutputDebugString(buffer);
	sprintf(buffer, "Up: (%f, %f, %f)\nTarget: (%f, %f, %f)\n", mUp.x, mUp.y, mUp.z, mTarget.x, mTarget.y, mTarget.z);
	OutputDebugString(buffer);
	sprintf(buffer, "[Direction] x: %f, y: %f, z: %f\n", GetDirection().x, GetDirection().y, GetDirection().z);
	OutputDebugString(buffer);
	sprintf(buffer, "Yaw: %f Pitch: %f\n", mYaw, mPitch);
	OutputDebugString(buffer);
}

//! Sets the cameras position.
/**
@note make sure to call SetTarget() after this if you want to aim at the same target
*/
void Camera::SetPosition(XMFLOAT3 position)
{
	mPosition = position;
	UpdatePitchYaw();
}

//! Sets the cameras direction.
void Camera::SetDirection(XMFLOAT3 direction)
{
	mTarget = mPosition + direction;
	UpdatePitchYaw();
}

//! Sets the point the camera will aim at.
void Camera::SetTarget(XMFLOAT3 target)
{
	mTarget = target;
	UpdatePitchYaw();
}

//! Updates the yaw and pitch members.
void Camera::UpdatePitchYaw()
{
	// Calculate new yaw and pitch.
	XMFLOAT3 direction = GetDirection();
	mPitch = asinf(direction.y);
	mYaw = atan2f(direction.x, direction.z);
}

void Camera::SetLookSensitivity(float sensitivity)
{
	mSensitivity = sensitivity/100.0f;
}

void Camera::SetMoveSpeed(float speed)
{
	mVelocity = speed;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return mView;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return mProj;
}

XMFLOAT3 Camera::GetPosition()
{
	return mPosition;
}

XMFLOAT3 Camera::GetTarget()
{
	return mTarget;
}

XMFLOAT3 Camera::GetRight()
{
	return mRight;
}

//! Retuns the cameras looking direction.
XMFLOAT3 Camera::GetDirection()
{
	XMVECTOR direction;
	direction = XMVector3Normalize(XMLoadFloat3(&(mTarget - mPosition)));
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, direction);

	return dir;
}

void Camera::SetYaw(float yaw)
{
	mYaw = yaw;
	UpdateViewMatrix();
}

void Camera::SetPitch(float pitch)
{
	mPitch = pitch;
	UpdateViewMatrix();
}