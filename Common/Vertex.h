#pragma once
#include <Windows.h>
#include <xnamath.h>

//! Basic vertex.
struct Vertex
{
	Vertex() {}
	Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float tz, float u, float v) 
		: Pos(px, py, pz), Normal(nx, ny, nz), Tex(u, v) {}

	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};

struct BillboardVertex
{
	BillboardVertex() {}
	BillboardVertex(XMFLOAT3 pos, XMFLOAT2 size) 
		: Pos(pos), Size(size) {}	

	XMFLOAT3 Pos;
	XMFLOAT2 Size;
};