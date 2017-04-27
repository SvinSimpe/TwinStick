#pragma once

#include <DirectXMath.h>
#include <ctime>

namespace GameGlobals
{
	constexpr size_t MAX_ACTORS = 2048;

	namespace WorldBounds
	{
		constexpr float X_MIN = -500.0f;
		constexpr float X_MAX = 500.0f;
		constexpr float Z_MIN = -200.0f;
		constexpr float Z_MAX = 500.0f;;

	}

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

inline constexpr DirectX::XMFLOAT3 Vector3Truncate( const DirectX::XMFLOAT3& v, const float max ) noexcept
{
	return DirectX::XMFLOAT3( ( v.x < max ) ? v.x : max, 
								( v.y < max ) ? v.y : max, 
								( v.z < max ) ? v.z : max );

}

inline constexpr DirectX::XMFLOAT3 Vector3Clamp( const DirectX::XMFLOAT3& v, const float min, const float max ) noexcept
{
	return DirectX::XMFLOAT3( ( v.x < 0.0f ) ? /*neg*/ ( ( v.x > min ) ? v.x : min ) : /*pos*/ ( ( v.x < max ) ? v.x : max ), 
								( v.y < 0.0f ) ? /*neg*/ ( ( v.y > min ) ? v.y : min ) : /*pos*/ ( ( v.y < max ) ? v.y : max ), 
								( v.z < 0.0f ) ? /*neg*/ ( ( v.z > min ) ? v.z : min ) : /*pos*/ ( ( v.z < max ) ? v.z : max ) );

}


inline const float RandomFloatInRange( const float min, const float max )  noexcept
{

	return ( min + static_cast<float>( rand() % static_cast<int>(
		( max) - ( min ) + 1.0f ) ) );

}

// Returns 'value' mapped from one range into another
inline const float MapToRange( const float value, const float inRangeMin, const float inRangeMax,
							   const float outRangeMin, const float outRangeMax )  noexcept
{
	const float inputRange	= inRangeMax - inRangeMin;
	const float outputRange	= outRangeMax - outRangeMin;

	return ( value - inRangeMin ) * outputRange / inputRange + outRangeMin;

}

