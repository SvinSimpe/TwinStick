#pragma once

#include <DirectXMath.h>


struct InstanceData
{
	DirectX::XMFLOAT4X4	worldMatrix;
	DirectX::XMFLOAT4	color;
};

struct FrameData
{
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMFLOAT4   eyePosition;
};