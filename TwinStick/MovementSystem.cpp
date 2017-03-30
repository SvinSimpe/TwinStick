#include "MovementSystem.h"

#define MOVEMENT_MASK ( EComponentType::Transform | EComponentType::Movement )

MovementSystem::MovementSystem()
{}

MovementSystem::~MovementSystem()
{}

bool MovementSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActors, void * systemSpecificInput )
{
	for( size_t i = 0; i < numActiveActors; i++ )
	{
		if( actors->mIsActive[i] &&
			( actors->componentMasks[i] & MOVEMENT_MASK ) == MOVEMENT_MASK )
		{


		}
	}


	return true;

}
