#pragma once

#include "ISystems.h"

class SteeringBehaviourSystem : public ISystems
{

	private:

	public:
		SteeringBehaviourSystem();
		~SteeringBehaviourSystem();

		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActors, void* systemSpecificInput ) override;

};