#include "MovementSystem.h"
#include "Utility.h"
#include "stdafx.h"

using namespace DirectX;

#define MOVEMENT_MASK ( EComponentType::Transform | EComponentType::Movement )

MovementSystem::MovementSystem()
{

}

MovementSystem::~MovementSystem()
{}

bool MovementSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActors, void * systemSpecificInput )
{
	for( size_t i = 0; i < numActiveActors; i++ )
	{
		if( actors->mIsActive[i] &&
			( actors->componentMasks[i] & MOVEMENT_MASK ) == MOVEMENT_MASK )
		{
			std::unique_ptr<TransformComponent>& transformComp		= actors->mTransformComponents[i];
			std::unique_ptr<MovementComponent>& moveComp			= actors->mMovementComponents[i];
			

			// If actor has a BehaviourComponent:
			// Affect MovementComponent with BehaviourComponent
			//if( ( actors->componentMasks[i] & EComponentType::Behaviour ) == EComponentType::Behaviour ) // Remove this!!!!!!!11
			//{
				/*	Already calculated in SteeringBehaviorSystem

					desired_velocity = normalize(target - position) * max_velocity
					steering = desired_velocity - velocity
				*/
				/*std::unique_ptr<SteeringBehaviourComponent>& steerComp	= actors->mSteeringBehaviorComponents[i];

				XMVECTOR newVelocity = DirectX::XMVectorTruncate( XMLoadFloat3( &moveComp->velocity ) +
																  XMLoadFloat3( &steerComp->steeringVector ) );
				XMStoreFloat3( &moveComp->velocity, newVelocity );

				XMStoreFloat3( &transformComp->location, newVelocity );



			}*/



		
			if( ( XMVectorGetX( XMVector3LengthEst( XMLoadFloat3( &moveComp->targetLocation ) -
									  XMLoadFloat3( &transformComp->location ) ) ) <= 5.0f ) ||
				IsVector3Zero( moveComp->velocity ) )
			{
				// Set new target
				moveComp->targetLocation = XMFLOAT3( RandomFloatInRange( -150.0f, 150.0f ),
													 0.0f,						   
													 RandomFloatInRange( -150.0f, 150.0f ) );
			}


			// Move actor towards target location
			XMStoreFloat3( &transformComp->location,
						   XMLoadFloat3( &transformComp->location ) + XMLoadFloat3( &moveComp->velocity ) * moveComp->speed * deltaTime );

			int k = 4;

		}
	}


	return true;

}
