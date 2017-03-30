#include "BehaviourSystem.h"

#define BEHAVIOUR_MASK ( EComponentType::Behaviour )

BehaviorSystem::BehaviorSystem()
{}

BehaviorSystem::~BehaviorSystem()
{}

bool BehaviorSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActors, void * systemSpecificInput )
{
	return false;
}
