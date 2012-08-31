#include "d3dUtil.h"

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