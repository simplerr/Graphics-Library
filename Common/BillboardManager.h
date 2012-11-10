#pragma once

#include <D3DX11.h>
#include <vector>
#include "Vertex.h"
using namespace std;

// Graphics Library namespace.
namespace GLib
{
	class Billboard;
	class Graphics;
	class Texture2D;

	/**
		Contain a vertex buffer with points representing billboards.
	*/
	class BillboardManager
	{
	public:
		BillboardManager(Graphics* pGraphics, string texture);
		~BillboardManager();

		void AddBillboard(Graphics* pGraphics, BillboardVertex* billboard);
		void RemoveBillbaord(BillboardVertex* billboard);
		void BuildVertexBuffer(ID3D11Device* device);
		void SetRebuild(bool rebuild);

		ID3D11Buffer*	GetVertexBuffer();
		Texture2D*		GetTexture();
		int				GetNumBillboards();
		bool			GetRebuild();
	private:
		vector<BillboardVertex*>	mBillboardList;
		ID3D11Buffer*		mVB;
		Texture2D*			mTexture;
		bool				mRebuild;
	};
}