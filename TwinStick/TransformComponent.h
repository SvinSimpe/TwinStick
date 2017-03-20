#pragma once

#include "IComponent.h"
#include <DirectXMath.h>
using namespace DirectX;
struct TransformComponent : public IComponent
{
	XMFLOAT3	location	= XMFLOAT3();
	XMFLOAT3	rotation	= XMFLOAT3();
	XMFLOAT3	scale		= XMFLOAT3();
	bool		isDirty		= false;

	TransformComponent( XMFLOAT3 initialLocation = XMFLOAT3(),
						XMFLOAT3 initialRotation = XMFLOAT3(),
						XMFLOAT3 initialScale = XMFLOAT3( 1.0f, 1.0f, 1.0f ) )
	{
		location	= initialLocation;
		rotation	= initialRotation;
		scale		= initialScale;
		isDirty		= true;

	}



	// Inherited via IComponent
	virtual const EComponentType GetType() const override
	{
		return EComponentType::Transform;

	}

};