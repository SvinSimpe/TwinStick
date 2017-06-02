#include "MovementSystem.h"
#include "Utility.h"
#include "stdafx.h"
#include <string>

using namespace DirectX;

#define MOVEMENT_MASK ( EComponentType::Transform | EComponentType::Movement )

MovementSystem::MovementSystem()
{}

MovementSystem::~MovementSystem()
{}

bool MovementSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							 size_t numActiveActors, void * systemSpecificInput )
{
	if( !actors )
		return false;

	for( size_t i = 0; i < numActiveActors; i++ )
	{
		if( actors->mIsActive[i] &&
			( actors->mComponentMasks[i] & MOVEMENT_MASK ) == MOVEMENT_MASK )
		{
			std::unique_ptr<TransformComponent>& transformComp		= actors->mTransformComponents[i];
			std::unique_ptr<MovementComponent>& moveComp			= actors->mMovementComponents[i];
		
			XMStoreFloat3( &transformComp->location,
						   XMLoadFloat3( &transformComp->location ) + XMLoadFloat3( &moveComp->velocity ) * moveComp->speed * deltaTime );

			transformComp->rotation.y = atan2f( moveComp->velocity.x, moveComp->velocity.z );

			

			if( actors->mCollisionComponents[i]->mOverlap )
			{ 
				actors->mMeshComponents[i]->color = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
			}
			else
			{ 			
				actors->mMeshComponents[i]->color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

				if( i == 0 )
					actors->mMeshComponents[i]->color = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
			}
			

			actors->mCollisionComponents[i]->mOverlap = false;

		}
	}

	return true;

}
