#include "PrimitiveFactory.h"
#include "d3dUtil.h"
#include "Runnable.h"
#include "D3DCore.h"
#include <D3DX11.h>
#include <vector>
#include "Primitive.h"
#include "Vertex.h"

PrimitiveFactory::PrimitiveFactory()
{

}
	
PrimitiveFactory::~PrimitiveFactory()
{
	// Release the buffers.
	for(auto iter = mPrimitiveMap.begin(); iter != mPrimitiveMap.end(); iter++) 
		(*iter).second.Cleanup();
}

//! Creates a box primitive.
/**
@note if a primitive of this type already is created then it returns a pointer to it (instancing)
*/
Primitive* PrimitiveFactory::CreateBox()
{
	// Already created a box primitive?
	if(mPrimitiveMap.find("box") != mPrimitiveMap.end())
		return &mPrimitiveMap["box"];

	Primitive primitive;

	// Create the vertices.
    Vertex vertices[] =
    {
		{XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White   },
		{XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black   },
		{XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Red     },
		{XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Colors::Green   },
		{XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Colors::Blue    },
		{XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Colors::Yellow  },
		{XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Colors::Cyan    },
		{XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Magenta }
    };

	// Set the primitives vertices.
    primitive.SetVertices(gGame->GetD3D()->GetDevice(), vertices, 8);

	// Create the index buffer
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3, 
		4, 3, 7
	};

	// Set the indices.
	primitive.SetIndices(gGame->GetD3D()->GetDevice(), indices, 36);

	// Add to the primitive map.
	mPrimitiveMap["box"] = primitive;

	// Return adress to newly added primitive.
	return &mPrimitiveMap["box"];
}

Primitive* PrimitiveFactory::CreateGrid(float width, float depth, UINT m, UINT n)
{
	if(mPrimitiveMap.find("grid") != mPrimitiveMap.end())
		return &mPrimitiveMap["grid"];

	UINT vertexCount = m*n;
	UINT faceCount   = (m-1)*(n-1)*2;

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n-1);
	float dz = depth / (m-1);

	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	Primitive primitive;

	// Create the vertices.
	vector<Vertex> vertices;
	vertices.resize(vertexCount);
	for(UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for(UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			vertices[i*n+j].Pos		= XMFLOAT3(x, 0.0f, z);
			vertices[i*n+j].Pos.y	= GetHeight(x, z);
			vertices[i*n+j].Normal	= GetHillNormal(x, z);

			// Stretch texture over grid.
			//meshData.Vertices[i*n+j].TexC.x = j*du;
			//meshData.Vertices[i*n+j].TexC.y = i*dv;
		}
	}
 
	primitive.SetVertices(gGame->GetD3D()->GetDevice(), &vertices[0], vertexCount);

	// Create the indices.
	vector<UINT> indices;
	indices.resize(faceCount*3);

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for(UINT i = 0; i < m-1; ++i)
	{
		for(UINT j = 0; j < n-1; ++j)
		{
			indices[k]   = i*n+j;
			indices[k+1] = i*n+j+1;
			indices[k+2] = (i+1)*n+j;

			indices[k+3] = (i+1)*n+j;
			indices[k+4] = i*n+j+1;
			indices[k+5] = (i+1)*n+j+1;

			k += 6; // next quad
		}
	}

	primitive.SetIndices(gGame->GetD3D()->GetDevice(), &indices[0], faceCount*3);

	mPrimitiveMap["grid"] = primitive;
	return &mPrimitiveMap["grid"];
}

float PrimitiveFactory::GetHeight(float x, float z)
{
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
}

XMFLOAT3 PrimitiveFactory::GetHillNormal(float x, float z)
{
	// n = (-df/dx, 1, -df/dz)
	XMFLOAT3 n(
		-0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
		1.0f,
		-0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));
	
	XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
	XMStoreFloat3(&n, unitNormal);

	return n;
}