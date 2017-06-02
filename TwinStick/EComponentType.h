#pragma once

#include <memory>

enum class EComponentType : std::uint32_t
{
	None				= 0,
	Transform			= 1 << 0,
	Movement			= 1 << 1,
	Mesh				= 1 << 2,
	SteeringBehaviour	= 1 << 3,
	Collision			= 1 << 4,
	Health				= 1 << 5

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