#pragma once

#include "IComponent.h"
#include "CollisionShape.h"	

struct CollisionComponent : public IComponent
{
	std::unique_ptr<CollisionShape> mCollisionShape;
	bool							mOverlap;

	// Inherited via IComponent
	virtual const EComponentType GetType() const override
	{
		return EComponentType::Collision;

	}

	virtual const bool Set( const std::unique_ptr<IComponent>& newData )
	{
		try
		{
			CollisionComponent& newCollision = dynamic_cast<CollisionComponent&>( *newData );
			mCollisionShape = std::move( newCollision.mCollisionShape);
			mOverlap		= newCollision.mOverlap;

		}
		catch( const std::exception& exc )
		{
			#if _DEBUG
			OutputDebugStringA( "Error: " );
			OutputDebugStringA( exc.what() );
			#endif
			return false;

		}


		return true;

	}
};