#pragma once

#include "ECollisionShape.h"


struct CollisionShape
{
	DirectX::XMFLOAT2 mPosition;
	DirectX::XMFLOAT2 mCenter;

	virtual float GetHalfExtent() const = 0;
	virtual float GetHalfExtentSq() const = 0;
	virtual const ECollisionShape GetCollisionShape() const = 0;
};