#pragma once

#include <DirectXMath.h>
#include <ctime>

namespace GameGlobals
{
	constexpr size_t MAX_ACTORS = 2048;
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
// Treat each vector component as int
inline constexpr bool IsVector3EqualInt( const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2 ) noexcept
{
	return	static_cast<int>(v1.x) == static_cast<int>(v2.x) && 
		static_cast<int>(v1.y) == static_cast<int>(v2.y) && 
		static_cast<int>(v1.z) == static_cast<int>(v2.z);

}

inline constexpr bool IsVector3Zero( const DirectX::XMFLOAT3& v ) noexcept
{
	return	v.x == 0.0f &&
			v.y == 0.0f &&
			v.z == 0.0f;

}


inline const float RandomFloatInRange( const float min, const float max )
{

	return ( min + static_cast<float>( rand() % static_cast<int>(
		( max) - ( min ) + 1.0f ) ) );

}