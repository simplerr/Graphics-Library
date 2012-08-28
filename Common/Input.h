#pragma once
#include <Windows.h>
#include <dinput.h>

#include <xnamath.h>
#include "d3dUtil.h"

//! Wrapper for input with keyboard and mouse.
class Input
{
public:
	Input();
	~Input();

	void update(float dt);
	void draw();
	void poll();

	bool keyPressed(int key);
	bool keyDown(int key);
	bool keyReleased(int key);

	void getWorldPickingRay(D3DXVECTOR3& originW, D3DXVECTOR3& dirW);

	XMFLOAT3 mousePosition();
	float	mouseDx();
	float	mouseDy();
	float	mouseDz();
	void	setMousePosition(XMFLOAT3 pos);

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	unsigned char mLastKeyState[256];
	unsigned char mKeyState[256];

	IDirectInput8*       mDInput;
	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;
	DIMOUSESTATE2        mLastMouseState;

	XMFLOAT3 mMousePosition;
	float mDx, mDy;
};

// Global
extern Input* gInput;