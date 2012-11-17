#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include "GameTimer.h"

using namespace std;

//! Graphics Library namespace.
namespace GLib
{
	// Forward declarations.
	class Graphics;
	class D3DCore;
	class Input;

	//!
	//	Initializes the Win32 window.
	//	This class should be inherited in all your applications.
	//	Contains the game loop and a Graphics instance.
	//	Declared as a global for convenience.
	//!
	class Runnable
	{
	public:
		Runnable(HINSTANCE hInstance, string caption, int width, int height);
		virtual ~Runnable();

		// None-framwork methods.
		int		Run();
		bool	InitWin32();
		void	SwitchScreenMode();
		void	CalculateFrameStats();

		// Framework methods.
		virtual void Init();
		virtual void Update(Input* pInput, float dt) = 0;
		virtual void Draw(Graphics* pGraphics) = 0;
		virtual void OnResize(int width, int height) = 0;
		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Getters and setters.
		void SetVisible(bool visible);
		void SetFpsCap(float cap);
		void SetFullscreen(bool fullscreen);
	
		HINSTANCE	GetInstance();
		HWND		GetHwnd();	
		int			GetClientWidth();
		int			GetClientHeight();
		int			GetFullscreenWidth();
		int			GetFullscreenHeight();

		D3DCore*	GetD3D();
		Graphics*	GetGraphics();
		Input*		GetInput();
	private:
		Graphics*	mGraphics;
		Input*		mInput;
		GameTimer	mTimer;
		string		mCaption;
		HINSTANCE	mhInstance;
		HWND		mhMainWindow;
		bool		mFullscreen;
		int			mWindowedWidth;
		int			mWindowedHeight;
		float		mFpsCap;

	};	// Class

	// Global
	extern Runnable* GlobalApp;
}