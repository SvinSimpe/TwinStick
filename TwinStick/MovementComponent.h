#pragma once

#include "IComponent.h"
#include <DirectXMath.h>
#include "Utility.h"

struct MovementComponent : public IComponent
{
	bool				isMoving			= false;
	float				speed				= 0.0f;
	DirectX::XMFLOAT3	velocity			= DirectX::XMFLOAT3();
	DirectX::XMFLOAT3	maxVelocity			= DirectX::XMFLOAT3();
	DirectX::XMFLOAT3	acceleration		= DirectX::XMFLOAT3();
	DirectX::XMFLOAT3	maxAcceleration		= DirectX::XMFLOAT3();
	DirectX::XMFLOAT3	targetLocation		= DirectX::XMFLOAT3();
	DirectX::XMFLOAT3	targetRotation		= DirectX::XMFLOAT3();

	

	MovementComponent( bool isMoving = false,
					   float speed = 0.0f,
					   DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(),
					   DirectX::XMFLOAT3 maxVelocity = DirectX::XMFLOAT3(),
					   DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(),
					   DirectX::XMFLOAT3 maxAcceleration = DirectX::XMFLOAT3(),
					   DirectX::XMFLOAT3 targetLocation = DirectX::XMFLOAT3(),
					   DirectX::XMFLOAT3 targetRotation = DirectX::XMFLOAT3() )
	{
		this->isMoving			= isMoving;
		this->speed				= speed;
		this->velocity			= velocity;
		this->maxVelocity		= maxVelocity;
		this->acceleration		= acceleration;
		this->maxAcceleration	= maxAcceleration;
		this->targetLocation	= targetLocation;
		this->targetRotation	= targetRotation;

	}

	virtual const EComponentType GetType() const
	{
		return EComponentType::Movement;

	}

	virtual const bool Set( const std::unique_ptr<IComponent>& newData )
	{
		try
		{
			MovementComponent& newMovement = dynamic_cast<MovementComponent&>( *newData );
			isMoving		= newMovement.isMoving;
			speed			= newMovement.speed;
			velocity		= newMovement.velocity;
			maxVelocity		= newMovement.maxVelocity;
			acceleration	= newMovement.acceleration;
			maxAcceleration	= newMovement.maxAcceleration;
			targetLocation	= newMovement.targetLocation;
			targetRotation	= newMovement.targetRotation;
			
		}
		catch( const std::exception& exc )
		{
			#if _DEBUG
				OutputDebugStringA( "Error: " );
				OutputDebugStringA( exc.what() );
			#endif
			return false;

		}

		return true;
	}

};