#pragma once

#include "IComponent.h"
#include "ESteeringBehaviourState.h"
#include <DirectXMath.h>

struct SteeringBehaviourComponent : public IComponent
{
	float					mass				= 0.0f;
	DirectX::XMFLOAT3		desiredVelocity		= DirectX::XMFLOAT3();
	DirectX::XMFLOAT3		steeringVector		= DirectX::XMFLOAT3();
	ESteeringBehaviourState	state				= ESteeringBehaviourState::Wander;
	float					wanderCooldown		= 0.0f;

	SteeringBehaviourComponent( const float mass = 1.0f,
								DirectX::XMFLOAT3 desiredVelocity = DirectX::XMFLOAT3(),
								DirectX::XMFLOAT3 steeringVector = DirectX::XMFLOAT3(),
								ESteeringBehaviourState state = ESteeringBehaviourState::Seek,
								const float wanderCooldown = 1.0f)
	{
		this->mass				= mass;
		this->desiredVelocity	= desiredVelocity;
		this->steeringVector	= steeringVector;
		this->state				= state;
		this->wanderCooldown	= wanderCooldown;

	}

	virtual const EComponentType GetType() const
	{
		return EComponentType::SteeringBehaviour;

	}
	virtual const bool Set( const std::unique_ptr<IComponent>& newData )
	{
		try
		{
			SteeringBehaviourComponent& newSteeringBehaviour = dynamic_cast<SteeringBehaviourComponent&>( *newData );
			this->mass				= newSteeringBehaviour.mass; 
			this->desiredVelocity	= newSteeringBehaviour.desiredVelocity;
			this->steeringVector	= newSteeringBehaviour.steeringVector;
			this->state				= newSteeringBehaviour.state;
			this->wanderCooldown	= newSteeringBehaviour.wanderCooldown;

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