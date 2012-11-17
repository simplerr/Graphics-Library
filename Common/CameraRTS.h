#pragma once
#include "Camera.h"

namespace GLib
{
	//! A RTS camera.
	class CameraRTS : public Camera
	{
	public:
		CameraRTS();
		~CameraRTS();

		void Update(Input* pInput, float dt);
	private:
	};
}