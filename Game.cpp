#include <crtdbg.h>
#include <assert.h>
#include "Game.h"
#include "Common\D3DCore.h"
#include "Common\d3dUtil.h"
#include "Common\Graphics.h"

// Set globals to nullptrs
Runnable* gGame = nullptr;

//! The program starts here.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Game game(hInstance, "Graphics Library", 800, 600);
	gGame = &game;

	game.Init();

	return gGame->Run();
}

Game::Game(HINSTANCE hInstance, string caption, int width, int height)
	: Runnable(hInstance, caption, width, height)
{

}
	
Game::~Game()
{

}

void Game::Init()
{
	// Important to run Systems Init() function.
	Runnable::Init();
}
	
void Game::Update(float dt)
{

}
	
void Game::Draw(Graphics* pGraphics)
{
	pGraphics->ClearScene(Colors::Cyan);

	pGraphics->Present();
}

LRESULT Game::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{

		};
	};

	return Runnable::MsgProc(hwnd, msg, wParam, lParam);
}