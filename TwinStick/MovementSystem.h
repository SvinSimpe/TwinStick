#pragma once

#include "ISystems.h"

class MovementSystem : public ISystems
{
	private:

	public:
		MovementSystem();
		~MovementSystem();


		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							 size_t numActiveActors, void * systemSpecificInput ) override;

};