#include "SteeringBehaviourSystem.h"
//#include <DirectXMath.h>
#include "Utility.h"
#include "stdafx.h"

using namespace DirectX;

namespace SteeringBehaviourConstants
{
	const float MIN_WANDER_TIME = 0.1f;
	const float MAX_WANDER_TIME = 5.0f;

}


#define BEHAVIOUR_MASK ( EComponentType::Transform | EComponentType::Movement | EComponentType::SteeringBehaviour )

void SteeringBehaviourSystem::CalculateActorSteeringAndVelocity( std::unique_ptr<TransformComponent>& transformComp,
																 std::unique_ptr<MovementComponent>& moveComp,
																 std::unique_ptr<SteeringBehaviourComponent>& steerComp,
																 const float directionModifier )
{
	// Calculate desired velocity ( target - location ) * maxVelocity
	XMStoreFloat3( &steerComp->desiredVelocity, XMVector3Normalize( XMLoadFloat3( &moveComp->targetLocation ) -
																	XMLoadFloat3( &transformComp->location ) ) *
																	XMLoadFloat3( &moveComp->maxVelocity ) *
																	directionModifier );

	// Calculate steering vector ( desiredVelocity - velocity )
	XMStoreFloat3( &steerComp->steeringVector, XMLoadFloat3( &steerComp->desiredVelocity ) -
												XMLoadFloat3( &moveComp->velocity ) );

	// Make sure steeringVector magnitude doesn't exceed maxAcceleration
	steerComp->steeringVector = Vector3Truncate( steerComp->steeringVector,
												 moveComp->maxAcceleration * -1.0f,
												 moveComp->maxAcceleration );

	// Calculate acceleration (force) and apply to velocity
	XMStoreFloat3( &moveComp->acceleration, XMLoadFloat3( &steerComp->steeringVector ) / steerComp->mass );
	XMStoreFloat3( &moveComp->velocity, XMLoadFloat3( &moveComp->velocity ) +
										XMLoadFloat3( &moveComp->acceleration ) );
}

SteeringBehaviourSystem::SteeringBehaviourSystem()
{}

SteeringBehaviourSystem::~SteeringBehaviourSystem()
{}

bool SteeringBehaviourSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActors, void* systemSpecificInput )
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
					if( steerComp->wanderCooldown <= 0.0f )
					{
						steerComp->wanderCooldown = RandomFloatInRange( SteeringBehaviourConstants::MIN_WANDER_TIME,
																		SteeringBehaviourConstants::MAX_WANDER_TIME );
						moveComp->targetLocation = XMFLOAT3( RandomFloatInRange( GameGlobals::WorldBounds::WIDTH_MIN, GameGlobals::WorldBounds::WIDTH_MAX ),
															 0.0f,
															 RandomFloatInRange( GameGlobals::WorldBounds::DEPTH_MIN, GameGlobals::WorldBounds::DEPTH_MAX ) );

					}
					else
						steerComp->wanderCooldown -= deltaTime;

					CalculateActorSteeringAndVelocity( transformComp, moveComp, steerComp );

					break;
				}
				case ESteeringBehaviourState::Seek :
				{
					CalculateActorSteeringAndVelocity( transformComp, moveComp, steerComp );
					break;
				}
				case ESteeringBehaviourState::Flee :
				{
					CalculateActorSteeringAndVelocity( transformComp, moveComp, steerComp, -1.0f );

					break;
				}
				case ESteeringBehaviourState::Avoid :
				{

					break;
				}
				case ESteeringBehaviourState::Arrive :
				{

					float distanceToTarget = XMVectorGetX( XMVector3LengthEst( XMLoadFloat3( &moveComp->targetLocation ) -
																			   XMLoadFloat3( &transformComp->location ) ) );
					
					// Put this test where the actor state is set
					float arrivalRadius = 35.0f;
					float arrivalForce = MapToRange( distanceToTarget, 0.0f, arrivalRadius, 0.0f, 1.0f );
											
					CalculateActorSteeringAndVelocity( transformComp, moveComp, steerComp, arrivalForce );

					break;
				}
				default:
					break;
			}		
		}
	}

	return true;
}
