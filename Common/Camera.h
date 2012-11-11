#pragma once
#include "d3dUtil.h"

using namespace XNA;

//! Graphics Library namespace.
namespace GLib
{
	// Forward declarations.
	class Graphics;
	class Input;

	//!
	//	Contains the view and projection matrices.
	//	Simulates a first person camera.
	//!
	class Camera
	{
	public:
		Camera();
		~Camera();

		void Update(Input* pInput, float dt);
		void Move(Input* pInput);
		void Rotate(Input* pInput);
		void DrawDebug();
		void UpdateViewMatrix();

		void SetPosition(XMFLOAT3 position);
		void SetTarget(XMFLOAT3 target);
		void SetLookSensitivity(float sensitivity);
		void SetMoveSpeed(float speed);
		void SetDirection(XMFLOAT3 direction);
		void SetYaw(float yaw);
		void SetPitch(float pitch);

		XMFLOAT4X4	GetViewMatrix();
		XMFLOAT4X4	GetProjectionMatrix();
		XMFLOAT3	GetPosition();
		XMFLOAT3	GetTarget();
		XMFLOAT3	GetDirection();
		XMFLOAT3	GetRight();
		Frustum		GetFrustum();
		Ray			GetWorldPickingRay();
	private:
		void UpdatePitchYaw();
		XMFLOAT4X4	mView;
		XMFLOAT4X4	mProj;
		XMFLOAT3	mPosition;
		XMFLOAT3	mTarget;
		XMFLOAT3	mUp;
		XMFLOAT3	mRight;
		Frustum		mFrustum;

		float		mVelocity;
		float		mSensitivity;
		float		mHeightOffset;
		float		mYaw, mPitch;
	};
}