#pragma once

#include <Windows.h>
#include <string>

using namespace std;

class D3DCore;
class Effect;
class EffectManager;
struct D3DXCOLOR;

/**
	The user interface to the graphical components.
	Contains a D3DCore instance.
	This class and it's members handles everything that has to do with Direct3D.
*/
class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Init(int clientWidth, int clientHeight, HWND hwnd, bool fullscreen = false);
	Effect* LoadEffect(string filename);
	void ClearScene();
	void Present();

	void BuildVertexLayout();

	D3DCore* GetD3D();
private:
	D3DCore* mD3DCore;
	EffectManager* mEffectManager;
};	// Class