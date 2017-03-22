#pragma once

#include <memory>

enum class EBufferType : std::uint32_t
{
	Vertex,
	Frame,
	Instance,
	NumBufferTypes

};

//using T = std::underlying_type_t<EBufferType>;
//
//inline EBufferType operator|( EBufferType lhs, EBufferType rhs )
//{
//	return static_cast<EBufferType>( static_cast<T>( lhs ) | static_cast<T>( rhs ) );
//
//}
//
//inline EBufferType operator|( size_t lhs, EBufferType rhs )
//{
//	return static_cast<EBufferType>( static_cast<T>( lhs ) | static_cast<T>( rhs ) );
//
//}
//
//
//inline EBufferType operator&( EBufferType lhs, EBufferType rhs )
//{
//	return static_cast<EBufferType>( static_cast<T>(lhs) & static_cast<T>( rhs ) );
//
//}
//
//inline EBufferType operator&( size_t lhs, EBufferType rhs )
//{
//	return static_cast<EBufferType>( static_cast<T>(lhs) & static_cast<T>( rhs ) );
//
//}