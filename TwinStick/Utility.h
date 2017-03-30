#pragma once

#include <DirectXMath.h>

namespace GameGlobals
{
	constexpr size_t MAX_ACTORS = 256;

}


namespace Resolution
{ 
	constexpr size_t SCREEN_WIDTH	= 1920;
	constexpr size_t SCREEN_HEIGHT	= 1080;
}

inline constexpr bool IsVector3Equal( const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2 ) noexcept
{
	return	v1.x == v2.x &&
			v1.y == v2.y &&
			v1.z == v2.z;

}

inline constexpr bool IsVector3Zero( const DirectX::XMFLOAT3& v ) noexcept
{
	return	v.x == 0.0f &&
			v.y == 0.0f &&
			v.z == 0.0f;

}
