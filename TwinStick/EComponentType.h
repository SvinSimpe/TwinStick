#pragma once

#include <memory>

enum class EComponentType : std::uint32_t
{
	None		= 0,
	Transform	= 1 << 0,
	Movement	= 1 << 1,
	Collider	= 1 << 2,
	Render		= 1 << 3,
	Camera		= 1 << 4,
	Mesh		= 1 << 5

};

using T = std::underlying_type_t<EComponentType>;

inline EComponentType operator|( EComponentType lhs, EComponentType rhs )
{
	return static_cast<EComponentType>( static_cast<T>( lhs ) | static_cast<T>( rhs ) );

}

inline EComponentType operator|( size_t lhs, EComponentType rhs )
{
	return static_cast<EComponentType>( static_cast<T>( lhs ) | static_cast<T>( rhs ) );

}


inline EComponentType operator&( EComponentType lhs, EComponentType rhs )
{
	return static_cast<EComponentType>( static_cast<T>(lhs) & static_cast<T>( rhs ) );

}

inline EComponentType operator&( size_t lhs, EComponentType rhs )
{
	return static_cast<EComponentType>( static_cast<T>(lhs) & static_cast<T>( rhs ) );

}