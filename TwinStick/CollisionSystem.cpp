#include "CollisionSystem.h"
#include <string>

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

		collision->mCollisionShape->mPosition = XMFLOAT2( transform->location.x,
														transform->location.z );

	}


	mQuadTree->Insert( *collision.get() );
}

void CollisionSystem::CheckCollision( std::unique_ptr<ActorCollection>& actors, size_t numActiveActors )
{
	for( size_t i = 0; i < numActiveActors; i++ )
	{
		if( actors->mIsActive[i] &&
			( actors->mComponentMasks[i] & COLLISION_MASK ) == COLLISION_MASK )
		{
			CollisionComponent& comp = *actors->mCollisionComponents[i];
			std::vector<CollisionComponent*> possibleOverlaps;

			mQuadTree->GetOverlaps( possibleOverlaps, comp );

			for( auto& node : possibleOverlaps )
			{
				mNumChecks++;

				if( Intersection::CircleVsCircle( dynamic_cast<CircleCollisionShape&>( *comp.mCollisionShape ),
												  dynamic_cast<CircleCollisionShape&>( *node->mCollisionShape ) ) )
				{
					node->mOverlap = true;
					comp.mOverlap = true;
					int OurVariable =  ( ( rand() % 20 ) + 20 );
					std::string s = std::to_string( OurVariable );
					#if _DEBUG
						//OutputDebugStringA( "Overlap      " );
						//OutputDebugStringA( s.c_str() );
						//OutputDebugStringA( "\n" );
					#endif
					
				}
			}
		}
	}



	int k = 4;
}


CollisionSystem::CollisionSystem()
{
	float width		= abs( GameGlobals::WorldBounds::X_MIN - GameGlobals::WorldBounds::X_MAX );
	float height	= abs( GameGlobals::WorldBounds::Z_MIN - GameGlobals::WorldBounds::Z_MAX );

	mQuadTree = std::make_unique<QuadTree>( 1, BoxCollisionShape( XMFLOAT2( GameGlobals::WorldBounds::X_MIN,
																			GameGlobals::WorldBounds::Z_MAX ),
																  width,
																  height ) );

	mNumChecks = 0;
}

CollisionSystem::~CollisionSystem()
{}

void CollisionSystem::GetQuadTreeVertices( std::vector<XMFLOAT2>& vertices )
{
	mQuadTree->GetDebugVertices( vertices );
}

bool CollisionSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							  size_t numActiveActors, void * systemSpecificInput )
{
	if( !actors )
		return false;

	mQuadTree->Clear();

	for( size_t i = 0; i < numActiveActors; i++ )
	{
		if( actors->mIsActive[i] &&
			( actors->mComponentMasks[i] & COLLISION_MASK ) == COLLISION_MASK )
		{
			UpdateCollisionComponents( actors->mTransformComponents[i],
									   actors->mCollisionComponents[i] );

		}
	}

	CheckCollision( actors, numActiveActors );

	

	std::string s = std::to_string( mNumChecks );
	#if _DEBUG
		OutputDebugStringA( s.c_str() );
		OutputDebugStringA( "\n" );
	#endif
	mNumChecks = 0;

	return true;

}

