#pragma once

#include "CollisionShape.h"
#include <DirectXMath.h>

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