#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
{}

CollisionSystem::~CollisionSystem()
{}

bool CollisionSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							  size_t numActiveActors, void * systemSpecificInput )
{
	if( !actors )
		return false;

	return true;
}

