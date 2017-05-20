#pragma once

#include "ISystems.h"
#include "QuadTree.h"

class CollisionSystem : public ISystems
{
	private:
		std::unique_ptr<QuadTree> mQuadTree;
		size_t	mNumChecks;

	private:
		void UpdateCollisionComponents( std::unique_ptr<TransformComponent>& transform,
										std::unique_ptr<CollisionComponent>& collision );
		void CheckCollision( std::unique_ptr<ActorCollection>& actors, size_t numActiveActors );

	public:
		CollisionSystem();
		~CollisionSystem();


		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
						 size_t numActiveActors, void * systemSpecificInput ) override;

};