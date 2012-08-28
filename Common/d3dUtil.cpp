#include "d3dUtil.h"

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