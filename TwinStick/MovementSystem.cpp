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
		
			if( ( XMVectorGetX( XMVector3LengthEst( XMLoadFloat3( &moveComp->targetLocation ) -
									  XMLoadFloat3( &transformComp->location ) ) ) <= 5.0f ) ||
											IsVector3Zero( moveComp->velocity ) )
			{
				// Set new target
				moveComp->targetLocation = XMFLOAT3( RandomFloatInRange( -250.0f, 250.0f ),
													 0.0f,						   
													 RandomFloatInRange( -150.0f, 150.0f ) );

			}

			// Move actor towards target location
			XMStoreFloat3( &transformComp->location,
						   XMLoadFloat3( &transformComp->location ) + XMLoadFloat3( &moveComp->velocity ) * moveComp->speed * deltaTime );

			// Set rotation
			transformComp->rotation.y = atan2f( moveComp->velocity.x, moveComp->velocity.z );

		}
	}


	return true;

}
