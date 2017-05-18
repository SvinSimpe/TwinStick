#include "CollisionSystem.h"

#define COLLISION_MASK ( EComponentType::Transform | EComponentType::Collision )




void CollisionSystem::UpdateCollisionComponents( std::unique_ptr<TransformComponent>& transform,
												 std::unique_ptr<CollisionComponent>& collision )
{
	if( collision->mCollisionShape->GetCollisionShape() == ECollisionShape::Box )
	{
		collision->mCollisionShape->mCenter = XMFLOAT2( transform->location.x,
														transform->location.z );

		collision->mCollisionShape->mPosition = XMFLOAT2( transform->location.x -
														  collision->mCollisionShape->GetHalfExtent(),
														  transform->location.z +
														  collision->mCollisionShape->GetHalfExtent() );

	}
	else if( collision->mCollisionShape->GetCollisionShape() == ECollisionShape::Circle )
	{
		collision->mCollisionShape->mCenter = XMFLOAT2( transform->location.x,
														transform->location.z );

	}


	mQuadTree.Insert( *collision.get() );
}

void CollisionSystem::CheckCollision()
{
	const std::list<CollisionComponent*>& nodeList = mQuadTree.GetNodeList();

	for( auto& node : nodeList )
	{
		std::vector<CollisionComponent*> overlaps;
		mQuadTree.GetOverlaps( overlaps, *node );

		if( overlaps.empty() )
			node->mOverlap = false;
		else
		{ 
			for( auto& overlap : overlaps )
				overlap->mOverlap = true;

		}
	}
}


CollisionSystem::CollisionSystem()
{}

CollisionSystem::~CollisionSystem()
{}

bool CollisionSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							  size_t numActiveActors, void * systemSpecificInput )
{
	if( !actors )
		return false;

	for( size_t i = 0; i < numActiveActors; i++ )
	{
		if( actors->mIsActive[i] &&
			( actors->mComponentMasks[i] & COLLISION_MASK ) == COLLISION_MASK )
		{
			UpdateCollisionComponents( actors->mTransformComponents[i],
									   actors->mCollisionComponents[i] );

		}
	}

	CheckCollision();

	return true;

}

