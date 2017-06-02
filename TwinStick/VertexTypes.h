#pragma once
#include <DirectXMath.h>

struct Vertex12
{
	DirectX::XMFLOAT3 position;
};

struct Vertex32
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texCoord;
};
