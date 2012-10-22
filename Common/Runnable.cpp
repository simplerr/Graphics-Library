#include <sstream>
#include "Runnable.h"
#include "Graphics.h"
#include "Input.h"

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created.
	if(gGame != NULL )
		return gGame->MsgProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

//! Constructor. The Init() function handles the initialization.
Runnable::Runnable(HINSTANCE hInstance, std::string caption, int width, int height)
{
	// Set values.
	mGraphics		= nullptr;
	mhMainWindow	= nullptr;
	mCaption		= caption;
	mhInstance		= hInstance;
	mScreenWidth	= width;
	mScreenHeight	= height;
}

//! Cleans up and frees all pointers.
Runnable::~Runnable()
{
	delete mGraphics;
}

//! Initializes Win32 and Direct3D by calling Graphics::Init(...).
void Runnable::Init()
{
	// Init the Win32 components.
	InitWin32();

	// Init Direct3D.
	mGraphics = new Graphics();
	mGraphics->Init(GetScreenWidth(), GetScreenHeight(), GetHwnd(), false);
}

//! Initializes Win32 and creates the main window.
bool Runnable::InitWin32()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "D3DWndClassName";

	if(!RegisterClass(&wc)) {
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	RECT clientRect;
	clientRect.left = GetSystemMetrics(SM_CXSCREEN)/2 - mScreenWidth/2.0f;
	clientRect.right = GetSystemMetrics(SM_CXSCREEN)/2 + mScreenWidth/2.0f;
	clientRect.top = GetSystemMetrics(SM_CYSCREEN)/2 - mScreenHeight/2.0f;
	clientRect.bottom = GetSystemMetrics(SM_CYSCREEN)/2 + mScreenHeight/2.0f;

	AdjustWindowRect(&clientRect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	// Create the window with a custom size and make it centered
	// NOTE: WS_CLIPCHILDREN Makes the area under child windows not be displayed. (Useful when rendering DirectX and using windows controls).
	mhMainWindow = CreateWindow("D3DWndClassName", mCaption.c_str(), 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, GetSystemMetrics(SM_CXSCREEN)/2-(mScreenWidth/2),
		GetSystemMetrics(SM_CYSCREEN)/2-(mScreenHeight/2), width, height, 
		0, 0, mhInstance, 0); 

	if(!mhMainWindow) {
		MessageBox(0, "CreateWindow() failed.", 0, 0);
		PostQuitMessage(0);
	}

	// Show the newly created window.
	ShowWindow(mhMainWindow, SW_SHOW);
	UpdateWindow(mhMainWindow);

	// Return true since we succeeded.
	return true;
}

// The game loop. Loops until it catches a WM_QUIT message.
int Runnable::Run()
{
	MSG msg = {0};
	mTimer.Reset();

	// The loop that will run until the app closes.
	while(msg.message != WM_QUIT)
	{
		// If there are window messages then process them.
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE ))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Else do game stuff.
		else
		{
			// Calculate the delta time and call the abstract Update() and Draw() functions. 
			mTimer.Tick();
			gInput->Poll();
			CalculateFrameStats();
			Update(mTimer.DeltaTime());
			Draw(GetGraphics());
		}
    }

	return (int)msg.wParam;
}

// Handles all window messages.
LRESULT Runnable::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//! Toggle between fullscreen and windowed.
void Runnable::SwitchScreenMode()
{

}

//! Updates the caption with information about the FPS.
void Runnable::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream outs;   
		outs.precision(6);
		outs << mCaption << "    " << "FPS: " << fps << "    " << "Frame Time: " << mspf << " (ms)";
		SetWindowText(GetHwnd(), outs.str().c_str());
		
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

//! Returns the main HINSTANCE of the application.
HINSTANCE Runnable::GetInstance()
{
	return mhInstance;
}

//! Returns the	main HWND of the application.
HWND Runnable::GetHwnd()
{
	return mhMainWindow;
}

//! Sets the visibility of the window.
void Runnable::SetVisible(bool visible)
{
	if(visible)
		ShowWindow(mhMainWindow, SW_SHOW);
	else
		ShowWindow(mhMainWindow, SW_HIDE);
}

//! Returns the screen width.
int	Runnable::GetScreenWidth()
{
	return mScreenWidth;
}

//! Returns the screen height.
int	Runnable::GetScreenHeight()
{
	return mScreenHeight;
}

//! Returns the Graphics D3DCore instance.
D3DCore* Runnable::GetD3D()
{
	return mGraphics->GetD3D();
}

//! Returns the Graphics instance.
Graphics* Runnable::GetGraphics()
{
	return mGraphics;
}