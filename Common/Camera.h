#pragma once
#include "d3dUtil.h"
#include <xnamath.h>

class Graphics;

class Camera
{
public:
	Camera();
	~Camera();

	void update(float dt);
	void move();
	void rotate();
	void rotate(float pitch, float yaw);
	void drawDebug();
	void updateView();

	void setPosition(XMFLOAT3 position);
	void setTarget(XMFLOAT3 target);
	void setSensitivity(float sensitivity);
	void setSpeed(float speed);
	void setDirection(XMFLOAT3 direction);
	void setYaw(float yaw);
	void setPitch(float pitch);

	float getYaw();
	float getPitch();

	XMFLOAT4X4	getViewMatrix();
	XMFLOAT4X4	getProjectionMatrix();
	XMFLOAT3	getPosition();
	XMFLOAT3	getTarget();
	XMFLOAT3	getDirection();
	XMFLOAT3	getRight();
private:
	void UpdatePitchYaw();
	XMFLOAT4X4	mView;
	XMFLOAT4X4	mProj;
	XMFLOAT3	mPosition;
	XMFLOAT3 mTarget;
	XMFLOAT3 mUp;
	XMFLOAT3 mRight;

	float		mVelocity;
	float		mSensitivity;
	float		mHeightOffset;
	float		mYaw, mPitch;
};