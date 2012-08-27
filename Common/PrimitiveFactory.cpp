#include "PrimitiveFactory.h"
#include "d3dUtil.h"
#include "Runnable.h"
#include "D3DCore.h"
#include <D3DX11.h>
#include "../Primitive.h"
#include "../Vertex.h"

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

	Primitive primitive;

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