#pragma once

#include "ISystems.h"
#include "QuadTree.h"

class CollisionSystem : public ISystems
{
	private:
		QuadTree mQuadTree;

	public:
		CollisionSystem();
		~CollisionSystem();


		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
						 size_t numActiveActors, void * systemSpecificInput ) override;

};