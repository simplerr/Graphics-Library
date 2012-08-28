#include "Camera.h"
#include "Input.h"
#include "Graphics.h"
#include "Runnable.h"

Camera::Camera()
{
	// Set the sensitivity and speed
	setSensitivity(2.0f);
	setSpeed(0.005f);

	// Default position and target
	mPosition	= XMFLOAT3(9, 6, 0);
	mTarget		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	mUp			= XMFLOAT3(0.0f, 1.0f, 0.0f);		// Weird up vector

	// Calculate the new direction.
	UpdatePitchYaw();
	
	// Build the projection matrix
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PI * 0.25, (float)gGame->GetScreenWidth()/(float)gGame->GetScreenHeight(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, proj);

	updateView();
}

Camera::~Camera()
{

}

void Camera::update(float dt)
{
	// Rotate and move
	move();
	rotate();

	updateView();
	//buildWorldFrustumPlanes();
}

void Camera::updateView()
{
	// Update the right vector.
	XMVECTOR up = XMLoadFloat3(&mUp);
	XMVECTOR dir = XMLoadFloat3(&getDirection());
	XMVECTOR right = XMVector3Cross(up, dir);
	right = XMVector3Normalize(right);
	XMStoreFloat3(&mRight, right);

	// Update the view matrix
	XMVECTOR pos = XMLoadFloat3(&mPosition);
	XMVECTOR target = XMLoadFloat3(&mTarget);
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(pos, target, up));
}

void Camera::move()
{
	// Get the look direction
	XMVECTOR direction;
	direction = XMLoadFloat3(&getDirection());

	// Fast scroll zooming
	mPosition += direction * gInput->mouseDz() / 8;
	mTarget += direction * gInput->mouseDz() / 8;

	// Check for keypresses
	if(gInput->keyDown('W')) {
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) + direction*mVelocity);
		XMStoreFloat3(&mTarget, XMLoadFloat3(&mTarget) + direction*mVelocity);
	}
	else if(gInput->keyDown('S')) {
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) - direction*mVelocity);
		XMStoreFloat3(&mTarget, XMLoadFloat3(&mTarget) - direction*mVelocity);
	}

	if(gInput->keyDown('A')) {
		XMVECTOR right = XMLoadFloat3(&getRight());
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) - right*mVelocity);
		XMStoreFloat3(&mTarget, XMLoadFloat3(&mTarget) - right*mVelocity);
	}
	else if(gInput->keyDown('D')) {
		XMVECTOR right = XMLoadFloat3(&getRight());
		XMStoreFloat3(&mPosition, XMLoadFloat3(&mPosition) + right*mVelocity);
		XMStoreFloat3(&mTarget, XMLoadFloat3(&mTarget) + right*mVelocity);
	}
}

void Camera::rotate()
{
	// Increase the pitch and yaw angles.
	mPitch += gInput->mouseDy() / (-13.0f / mSensitivity);
	mYaw += gInput->mouseDx() / (13.0f / mSensitivity);

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

void Camera::rotate(float pitch, float yaw)
{
	// Increase the pitch and yaw angles.
	mPitch += pitch;
	mYaw += yaw;

	// Limit to PI/2 radians.
	if(mPitch > 0)
		mPitch = min(mPitch, 1.54f);
	else
		mPitch = max(mPitch, -1.54f);

	// Calculate the new direction.
	XMVECTOR direction;
	float r = cosf(mPitch);
	direction = XMVectorSet(sinf(mPitch), r * cosf(mYaw), r * sinf(mYaw), 0);

	// Set the new target.
	XMStoreFloat3(&mTarget, XMLoadFloat3(&mPosition) + direction);
}

void Camera::drawDebug()
{
	/*char buffer[256];
	sprintf(buffer, "Up: (%f, %f, %f)\nTarget: (%f, %f, %f)Height: %f", mUp.x, mUp.y, mUp.z, mTarget.x, mTarget.y, mTarget.z, height);
	gGraphics->drawText(buffer, 20, 200, BLUE);
	sprintf(buffer, "[Direction] x: %f, y: %f, z: %f", getDirection().x, getDirection().y, getDirection().z);
	gGraphics->drawText(buffer, 20, 240, BLUE);
	sprintf(buffer, "[dirXZ] x: %f, y: %f, z: %f", dirXZ.x, dirXZ.y, dirXZ.z);
	gGraphics->drawText(buffer, 20, 280, BLUE);
	sprintf(buffer, "Angle: %f", angle);
	gGraphics->drawText(buffer, 20, 320, WHITE);
	sprintf(buffer, "Yaw: %f Pitch: %f", mYaw, mPitch);
	gGraphics->drawText(buffer, 20, 360, GREEN);*/
}

void Camera::setPosition(XMFLOAT3 position)
{
	XMFLOAT3 direction = mTarget - mPosition;
	mPosition = position;
	mTarget = mPosition + direction;
	UpdatePitchYaw();
}

void Camera::setDirection(XMFLOAT3 direction)
{
	mTarget = mPosition + direction;
	UpdatePitchYaw();
}

void Camera::setTarget(XMFLOAT3 target)
{
	mTarget = target;
	XMFLOAT3 direction = mTarget - mPosition;
	UpdatePitchYaw();
}

void Camera::UpdatePitchYaw()
{
	// Calculate new yaw and pitch.
	XMFLOAT3 direction = getDirection();
	mPitch = asinf(direction.y);
	float r = cosf(mPitch);
	mYaw = asinf(direction.x / r);
}

void Camera::setSensitivity(float sensitivity)
{
	mSensitivity = sensitivity/100.0f;
}

void Camera::setSpeed(float speed)
{
	mVelocity = speed;
}

XMFLOAT4X4 Camera::getViewMatrix()
{
	return mView;
}

XMFLOAT4X4 Camera::getProjectionMatrix()
{
	return mProj;
}

XMFLOAT3 Camera::getPosition()
{
	return mPosition;
}

XMFLOAT3 Camera::getTarget()
{
	return mTarget;
}

XMFLOAT3 Camera::getRight()
{
	return mRight;
}

XMFLOAT3 Camera::getDirection()
{
	XMVECTOR direction;
	direction = XMVector3Normalize(XMLoadFloat3(&(mTarget - mPosition)));
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, direction);

	return dir;
}

void Camera::setYaw(float yaw)
{
	mYaw = yaw;
	updateView();
}

void Camera::setPitch(float pitch)
{
	mPitch = pitch;
	updateView();
}

float Camera::getYaw()
{
	return mYaw;
}

float Camera::getPitch()
{
	return mPitch;
}