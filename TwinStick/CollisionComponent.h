#pragma once

#include "IComponent.h"
#include "CircleCollisionShape.h"	

struct CollisionComponent : public IComponent
{
	CollisionShape* mCollisionShape;

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
			mCollisionShape = newCollision.mCollisionShape;

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