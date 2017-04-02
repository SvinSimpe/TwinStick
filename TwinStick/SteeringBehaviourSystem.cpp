#include "SteeringBehaviourSystem.h"
#include <DirectXMath.h>
#include "Utility.h"

using namespace DirectX;

#define BEHAVIOUR_MASK ( EComponentType::Transform | EComponentType::Movement | EComponentType::SteeringBehaviour )


SteeringBehaviourSystem::SteeringBehaviourSystem()
{}

SteeringBehaviourSystem::~SteeringBehaviourSystem()
{}

bool SteeringBehaviourSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActors, void * systemSpecificInput )
{
	for( size_t i = 0; i < numActiveActors; i++ )
	{
		if( actors->mIsActive[i] &&
			( actors->componentMasks[i] & BEHAVIOUR_MASK ) == BEHAVIOUR_MASK )
		{
			std::unique_ptr<TransformComponent>& transformComp		= actors->mTransformComponents[i];
			std::unique_ptr<MovementComponent>& moveComp			= actors->mMovementComponents[i];
			std::unique_ptr<SteeringBehaviourComponent>& steerComp	= actors->mSteeringBehaviorComponents[i];

			switch( steerComp->state )
			{
				case ESteeringBehaviourState::Wander :
				{



					break;
				}
				case ESteeringBehaviourState::Seek :
				{
					// Calculate desired velocity ( target - location ) * maxVelocity
					XMStoreFloat3( &steerComp->desiredVelocity, XMVector3Normalize( XMLoadFloat3( &moveComp->targetLocation ) -
																					XMLoadFloat3( &transformComp->location ) ) *
																					XMLoadFloat3( &moveComp->maxVelocity ) );
					
					// Calculate steering vector ( desiredVelocity - velocity )
					XMStoreFloat3( &steerComp->steeringVector, XMLoadFloat3( &steerComp->desiredVelocity ) -
																XMLoadFloat3( &moveComp->velocity ) );

					// Make sure steeringVector magnitude doesn't exceed maxAcceleretion
					steerComp->steeringVector = Vector3Truncate( steerComp->steeringVector,
																 moveComp->maxAcceleration * -1.0f,
																 moveComp->maxAcceleration );
					//XMStoreFloat3( &steerComp->steeringVector, XMVectorTruncate( XMLoadFloat3( &steerComp->steeringVector ) ) );

					// Calculate acceleration (force) and apply to velocity
					XMStoreFloat3( &moveComp->acceleration, XMLoadFloat3( &steerComp->steeringVector ) / steerComp->mass );
					XMStoreFloat3( &moveComp->velocity, XMLoadFloat3( &moveComp->velocity ) +
														XMLoadFloat3( &moveComp->acceleration ) );

					int k = 4;

					break;
				}
				case ESteeringBehaviourState::Flee :
				{

					break;
				}
				case ESteeringBehaviourState::Avoid :
				{

					break;
				}
				case ESteeringBehaviourState::Arrive :
				{

					break;
				}
				default:
					break;
			}		
		}
	}

	return true;
}
