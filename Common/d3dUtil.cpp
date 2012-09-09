#include "d3dUtil.h"
#include <xnamath.h>

// Returns the inverse transpose.
XMMATRIX InverseTranspose(CXMMATRIX M)
{
	// Inverse-transpose is just applied to normals.  So zero out 
	// translation row so that it doesn't get into our inverse-transpose
	// calculation--we don't want the inverse-transpose of the translation.
	XMMATRIX A = M;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));
}

XMFLOAT3 operator+(const XMFLOAT3 a, const XMFLOAT3 b)
{
	XMVECTOR first = XMLoadFloat3(&a);
	XMVECTOR second = XMLoadFloat3(&b);

	XMFLOAT3 result;
	XMStoreFloat3(&result, first + second);
	return result;
}

XMFLOAT3 operator-(const XMFLOAT3 a, const XMFLOAT3 b)
{
	XMVECTOR first = XMLoadFloat3(&a);
	XMVECTOR second = XMLoadFloat3(&b);

	XMFLOAT3 result;
	XMStoreFloat3(&result, first - second);
	return result;
}

XMFLOAT3 operator+=(const XMFLOAT3 a, const XMVECTOR b)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&a) + b);
	return result;
}

XMFLOAT3 operator-=(const XMFLOAT3 a, const XMVECTOR b)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMLoadFloat3(&a) - b);
	return result;
}

XMFLOAT4 operator*(const XMFLOAT4 a, const float b)
{
	return XMFLOAT4(a.x * b, a.y * b, a.z * b, a.w * b);
}

XMFLOAT2 operator+(const XMFLOAT2 a, const XMFLOAT2 b)
{
	XMVECTOR first = XMLoadFloat2(&a);
	XMVECTOR second = XMLoadFloat2(&b);

	XMFLOAT2 result;
	XMStoreFloat2(&result, first + second);
	return result;
}

XMFLOAT2 operator-(const XMFLOAT2 a, const XMFLOAT2 b)
{
	XMVECTOR first = XMLoadFloat2(&a);
	XMVECTOR second = XMLoadFloat2(&b);

	XMFLOAT2 result;
	XMStoreFloat2(&result, first - second);
	return result;
}

//-----------------------------------------------------------------------------
// Transform an axis aligned box by an angle preserving transform.
//-----------------------------------------------------------------------------
VOID TransformAxisAlignedBoxCustom(XNA::AxisAlignedBox* pOut, const XNA::AxisAlignedBox* pIn, FXMVECTOR Scale, FXMVECTOR Rotation, FXMVECTOR Translation )
{
    XMASSERT( pOut );
    XMASSERT( pIn );

    static XMVECTOR Offset[8] =
    {
        { -1.0f, -1.0f, -1.0f, 0.0f },
        { -1.0f, -1.0f,  1.0f, 0.0f },
        { -1.0f,  1.0f, -1.0f, 0.0f },
        { -1.0f,  1.0f,  1.0f, 0.0f },
        {  1.0f, -1.0f, -1.0f, 0.0f },
        {  1.0f, -1.0f,  1.0f, 0.0f },
        {  1.0f,  1.0f, -1.0f, 0.0f },
        {  1.0f,  1.0f,  1.0f, 0.0f }
    };

    // Load center and extents.
    XMVECTOR Center = XMLoadFloat3( &pIn->Center );
    XMVECTOR Extents = XMLoadFloat3( &pIn->Extents );

    XMVECTOR VectorScale = Scale;//XMVectorReplicate( Scale );

    // Compute and transform the corners and find new min/max bounds.
    XMVECTOR Corner = Center + Extents * Offset[0];
    Corner = XMVector3Rotate( Corner * VectorScale, Rotation ) + Translation;

    XMVECTOR Min, Max;
    Min = Max = Corner;

    for( INT i = 1; i < 8; i++ )
    {
        Corner = Center + Extents * Offset[i];
        Corner = XMVector3Rotate( Corner * VectorScale, Rotation ) + Translation;

        Min = XMVectorMin( Min, Corner );
        Max = XMVectorMax( Max, Corner );
    }

    // Store center and extents.
    XMStoreFloat3( &pOut->Center, ( Min + Max ) * 0.5f );
    XMStoreFloat3( &pOut->Extents, ( Max - Min ) * 0.5f );

    return;
}