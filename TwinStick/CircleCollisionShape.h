#pragma once

#include "CollisionShape.h"
#include <DirectXMath.h>

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