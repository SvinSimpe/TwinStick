#pragma once

#include "IComponent.h"
#include "ECollisionShape.h"
#include <DirectXMath.h>

struct CollisionComponent : public IComponent
{
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

	virtual const ECollisionShape GetCollisionShape() const = 0;
};