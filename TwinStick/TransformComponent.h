#pragma once

#include "IComponent.h"
#include <DirectXMath.h>
using namespace DirectX;
struct TransformComponent : public IComponent
{
	XMFLOAT3	location	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	rotation	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	scale		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	bool		isDirty		= false;

	TransformComponent( XMFLOAT3 initialLocation = XMFLOAT3( 0.0f, 0.0f, 0.0f ),
						XMFLOAT3 initialRotation = XMFLOAT3( 0.0f, 0.0f, 0.0f ),
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