#pragma once

#include "ISystems.h"

class BehaviorSystem : public ISystems
{
	private:

	public:

		BehaviorSystem();
		~BehaviorSystem();

	// Inherited via ISystems
	virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActors, void * systemSpecificInput ) override;

};