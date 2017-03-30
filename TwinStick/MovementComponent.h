#pragma once

#include "IComponent.h"
#include <DirectXMath.h>
struct MovementComponent : public IComponent
{
	bool				isMoving;
	DirectX::XMFLOAT3	velocity;
	DirectX::XMFLOAT3	maxVelocity;
	DirectX::XMFLOAT3	acceleration;
	DirectX::XMFLOAT3	maxAcceleration;
	DirectX::XMFLOAT3	destination; //??


	virtual const EComponentType GetType() const
	{
		return EComponentType::Movement;

	}

	virtual const bool Set( const std::unique_ptr<IComponent>& newData )
	{


	}

};