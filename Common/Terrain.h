#pragma once

#include <vector>
#include "d3dUtil.h"
#include "Vertex.h"

class Primitive;
class Graphics;
class PrimitiveFactory;

//! Stores the init information for a terrain.
struct InitInfo
	{
		string HeightMapFilename;
		string LayerMapFilename0;
		string LayerMapFilename1;
		string LayerMapFilename2;
		string LayerMapFilename3;
		string LayerMapFilename4;
		string BlendMapFilename;
		UINT HeightmapWidth;
		UINT HeightmapHeight;
		float HeightScale;
		float CellSpacing;
	};

/**
	This class represents the basic terrain and gets loaded from a heightmap.
	It contains a blend map and support up to 4 different textures.
*/
class Terrain
{
public:
	Terrain();
	~Terrain();

	void Init(ID3D11Device* device, ID3D11DeviceContext* context, PrimitiveFactory* pPrimitiveFactory, const InitInfo& initInfo);
	void Draw(Graphics* pGraphics);

	InitInfo	GetInfo();
	Primitive*	GetPrimitive();
	float		GetWidth();
	float		GetDepth();
	float		GetHeight(float x, float z);
	void		SetHeigt(float x, float z, float height);
	void		BuildHeightmapSRV(ID3D11Device* device);
	void		BuildBlendMapSRV(ID3D11Device* device);
	void		Smooth();
	void		Smooth(XMFLOAT3 origin, int radius);
	XMFLOAT3	GetIntersectPoint(Ray ray);

	void		SetBlend(XMFLOAT3 pos, float modifier, int layer);
private:
	void		LoadHeightmap();
	void		LoadBlendMap();
	bool		InBounds(int i, int j);
	float		Average(int i, int j);
	XMFLOAT3	BinarySearch(Ray ray);
	Ray			LinearSearch(Ray ray);
private:
	ID3D11ShaderResourceView* mLayerTextureArraySRV;
	ID3D11ShaderResourceView* mHeightMapSRV;
	ID3D11ShaderResourceView* mBlendMapSRV;
	Primitive*		mPrimitive;
	vector<float>	mHeightMap;
	vector<XMFLOAT4>mBlendMap;
	InitInfo		mInfo;
	XMFLOAT4X4		mWorldMatrix;
};