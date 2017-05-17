#pragma once

#include "ECollisionShape.h"
#include <DirectXMath.h>

struct CollisionShape
{
	DirectX::XMFLOAT2 mPosition;
	DirectX::XMFLOAT2 mCenter;

	virtual float GetHalfExtent() const = 0;
	virtual float GetHalfExtentSq() const = 0;
	virtual const ECollisionShape GetCollisionShape() const = 0;
};

struct CircleCollisionShape : public CollisionShape
{
	float	mRadius;
	float	mRadiusSq;

	CircleCollisionShape( DirectX::XMFLOAT2 center, float radius )
	{
		mCenter		= center;
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