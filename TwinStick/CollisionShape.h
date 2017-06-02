#pragma once

#define Min(a,b)  (((a) < (b)) ? (a) : (b))
#define Max(a,b)  (((a) > (b)) ? (a) : (b))

#include "ECollisionShape.h"
#include "Utility.h"
#include <DirectXMath.h>
using namespace DirectX;

struct CollisionShape
{
	XMFLOAT2 mPosition;
	XMFLOAT2 mCenter;

	virtual float GetHalfExtent() const = 0;
	virtual float GetHalfExtentSq() const = 0;
	virtual const ECollisionShape GetCollisionShape() const = 0;
};

struct CircleCollisionShape : public CollisionShape
{
	float	mRadius;
	float	mRadiusSq;

	CircleCollisionShape(){}

	CircleCollisionShape( DirectX::XMFLOAT2 center, float radius )
	{
		mCenter		= center;
		mPosition	= mCenter;
		mRadius		= radius;
		mRadiusSq	= mRadius * mRadius;
	};

	virtual float GetHalfExtent() const
	{
		return mRadius;
	}

	virtual float GetHalfExtentSq() const
	{
		return mRadiusSq;
	}

	virtual const ECollisionShape GetCollisionShape() const
	{
		return ECollisionShape::Circle;

	}

};

struct BoxCollisionShape : public CollisionShape
{
	float	mWidth;
	float	mHeight;

	BoxCollisionShape(){}

	BoxCollisionShape( DirectX::XMFLOAT2 position, float width, float height )
	{
		mPosition	= position;
		mWidth		= width;
		mHeight		= height;
		mCenter		= DirectX::XMFLOAT2( mPosition.x + mWidth / 2.0f,
										 mPosition.y - mHeight / 2.0f );
	};

	virtual float GetHalfExtent() const
	{
		return mWidth;
	}

	virtual float GetHalfExtentSq() const
	{
		return mWidth * mWidth;
	}

	virtual const ECollisionShape GetCollisionShape() const
	{
		return ECollisionShape::Box;

	}

};



namespace Intersection
{
	inline bool AABBVsAABB( const BoxCollisionShape& box1, 
							const BoxCollisionShape& box2 ) noexcept
	{
		return ( box1.mPosition.x < box2.mPosition.x + box2.mWidth &&
				 box1.mPosition.x + box1.mWidth > box2.mPosition.x &&
				 box1.mPosition.y < box2.mPosition.y - box2.mHeight &&
				 box1.mPosition.y - box1.mHeight > box2.mPosition.y );

	}

	inline bool CircleVsCircle( const CircleCollisionShape& circle1, 
								const CircleCollisionShape& circle2 ) noexcept
	{
		return ( XMVectorGetX( XMVector2LengthSq( XMLoadFloat2( &circle1.mCenter ) - 
												  XMLoadFloat2( &circle2.mCenter ) ) ) <
				 circle1.GetHalfExtentSq() + circle2.GetHalfExtentSq() );
	}

	//inline bool CircleVsAABB( const CircleCollisionShape& circle, 
	//						  const BoxCollisionShape& box ) noexcept
	//{
	//	XMVECTOR centerToCenter = XMVector2Length( XMLoadFloat2( &circle.mCenter ) -
	//											   XMLoadFloat2( &box.mCenter ) );
	//	XMVECTOR boxHalfExtents = XMVectorSet( box.mWidth * 0.5f, box.mHeight * 0.5f,
	//										   0.0f, 0.0f );
	//	XMVECTOR closestPoint = XMVectorClamp( centerToCenter, -boxHalfExtents, boxHalfExtents );

	//	float d =   XMVectorGetX( closestPoint - XMLoadFloat2( &circle.mCenter ) ) - circle.GetHalfExtent();
	//	return circle.GetHalfExtent() < d;
	//	//return circle.GetHalfExtent() < XMVectorGetX( closestPoint - XMLoadFloat2( &circle.mCenter ) );

	//}

	inline bool Point( const XMFLOAT2& point, 
					   const BoxCollisionShape& box ) noexcept
	{
		return ( point.x < box.mPosition.x + box.mWidth &&
				 point.x > box.mPosition.x &&
				 point.y > box.mPosition.y - box.mHeight &&
				 point.y < box.mPosition.y );

	}

	inline bool CircleVsAABB( const CircleCollisionShape& circle, 
							  const BoxCollisionShape& box ) noexcept
	{
		//float deltaX = circle.mPosition.x - Max( box.mPosition.x, Min( circle.mPosition.x, box.mPosition.x + box.mWidth ) );
		//float deltaY = circle.mPosition.y - Max( box.mPosition.y, Min( circle.mPosition.y, box.mPosition.y + box.mHeight ) );
		//
		//return ( deltaX * deltaX + deltaY * deltaY ) < circle.GetHalfExtentSq();

		bool overlap = Point( circle.mCenter, box );
		return overlap;
	}

	
}
