#include "SteeringBehaviourSystem.h"
#include "Utility.h"
#include "stdafx.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace SteeringBehaviourConstants
{
	const float MIN_WANDER_TIME					= 0.5f;
	const float MAX_WANDER_TIME					= 3.0f;
	const float THREAT_DISTANCE_SQUARED			= 100.0f * 100.0f;
	const float PREFERED_SAFE_DISTANCE_SQUARED	= 120.0f * 120.0f;
	const float ARRIVAL_RADIUS_SQUARED			= 35.0f * 35.0f;

}

#define BEHAVIOUR_MASK ( EComponentType::Transform | EComponentType::Movement | EComponentType::SteeringBehaviour )

void SteeringBehaviourSystem::CalculateActorSteeringAndVelocity( std::unique_ptr<TransformComponent>& transformComp,
																 std::unique_ptr<MovementComponent>& moveComp,
																 std::unique_ptr<SteeringBehaviourComponent>& steerComp,
																 const float directionModifier ) const
{
	XMStoreFloat3( &steerComp->desiredVelocity, XMVector3Normalize( XMLoadFloat3( &moveComp->targetLocation ) -
																	XMLoadFloat3( &transformComp->location ) ) *
																	XMLoadFloat3( &moveComp->maxVelocity ) *
																	directionModifier );

	XMStoreFloat3( &steerComp->steeringVector, XMLoadFloat3( &steerComp->desiredVelocity ) -
												XMLoadFloat3( &moveComp->velocity ) );

	steerComp->steeringVector = Vector3Clamp( steerComp->steeringVector,
											  moveComp->maxAcceleration * -1.0f,
											  moveComp->maxAcceleration );

	if( steerComp->mass == 0.0f )
		steerComp->mass = 1.0f;

	XMStoreFloat3( &moveComp->acceleration, XMLoadFloat3( &steerComp->steeringVector ) / steerComp->mass );
	XMStoreFloat3( &moveComp->velocity, XMLoadFloat3( &moveComp->velocity ) +
										XMLoadFloat3( &moveComp->acceleration ) );

}

SteeringBehaviourSystem::SteeringBehaviourSystem()
{}

SteeringBehaviourSystem::~SteeringBehaviourSystem()
{}

bool SteeringBehaviourSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
									  size_t numActiveActors, void* systemSpecificInput )
{
	if( !actors )
		return false;

	for( size_t i = 0; i < numActiveActors; i++ )
	{
		if( actors->mIsActive[i] &&
			( actors->mComponentMasks[i] & BEHAVIOUR_MASK ) == BEHAVIOUR_MASK )
		{
			std::unique_ptr<TransformComponent>& transformComp		= actors->mTransformComponents[i];
			std::unique_ptr<MovementComponent>& moveComp			= actors->mMovementComponents[i];
			std::unique_ptr<SteeringBehaviourComponent>& steerComp	= actors->mSteeringBehaviorComponents[i];

			XMVECTOR& playerLocation = XMLoadFloat3( &actors->mTransformComponents[0]->location );

			switch( steerComp->state )
			{
				case ESteeringBehaviourState::Wander :
				{
					if( i != 0 )
					{ 
						if( XMVectorGetX( XMVector3LengthSq( playerLocation - XMLoadFloat3( &transformComp->location ) ) ) <=
							SteeringBehaviourConstants::THREAT_DISTANCE_SQUARED )
						{
							steerComp->state = ESteeringBehaviourState::Flee;
							moveComp->targetLocation = actors->mTransformComponents[0]->location;
						}

					}


					if( steerComp->wanderCooldown <= 0.0f )
					{
						steerComp->wanderCooldown = RandomFloatInRange( SteeringBehaviourConstants::MIN_WANDER_TIME,
																		SteeringBehaviourConstants::MAX_WANDER_TIME );
						moveComp->targetLocation = XMFLOAT3( RandomFloatInRange( GameGlobals::WorldBounds::X_MIN, GameGlobals::WorldBounds::X_MAX ),
															 0.0f,
															 RandomFloatInRange( GameGlobals::WorldBounds::Z_MIN, GameGlobals::WorldBounds::Z_MAX ) );

					}

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
					if( XMVectorGetX( XMVector3LengthSq( playerLocation - XMLoadFloat3( &transformComp->location ) ) ) >=
						SteeringBehaviourConstants::PREFERED_SAFE_DISTANCE_SQUARED )
					{
						steerComp->state = ESteeringBehaviourState::Wander;

					}
					
					CalculateActorSteeringAndVelocity( transformComp, moveComp, steerComp, -1.0f );
					break;
				}
				case ESteeringBehaviourState::Arrive :
				{
					float distanceToTargetSquared = XMVectorGetX( 
						XMVector3LengthSq( XMLoadFloat3( &moveComp->targetLocation ) -
											XMLoadFloat3( &transformComp->location ) ) );
					
					float arrivalForce = MapToRange( distanceToTargetSquared,
													 0.0f, SteeringBehaviourConstants::ARRIVAL_RADIUS_SQUARED,
													 0.0f, 1.0f );
											
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
