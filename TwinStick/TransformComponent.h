#pragma once

#include "IComponent.h"
#include <DirectXMath.h>

struct TransformComponent : public IComponent
{
	DirectX::XMFLOAT3	location	= DirectX::XMFLOAT3();
	DirectX::XMFLOAT3	rotation	= DirectX::XMFLOAT3();
	DirectX::XMFLOAT3	scale		= DirectX::XMFLOAT3();
	bool		isDirty		= false;

	TransformComponent( DirectX::XMFLOAT3 initialLocation = DirectX::XMFLOAT3(),
						DirectX::XMFLOAT3 initialRotation = DirectX::XMFLOAT3(),
						DirectX::XMFLOAT3 initialScale = DirectX::XMFLOAT3( 1.0f, 1.0f, 1.0f ) )
	{
		location	= initialLocation;
		rotation	= initialRotation;
		scale		= initialScale;
		isDirty		= true;

	}



	// Inherited via IComponent
	virtual const EComponentType GetType() const override
	{
		return EComponentType::Transform;

	}

	virtual const bool Set( const std::unique_ptr<IComponent>& newData )
	{
		try
		{
			TransformComponent& newTransform = dynamic_cast<TransformComponent&>( *newData );

			location	= newTransform.location;
			rotation	= newTransform.rotation;
			scale		= newTransform.scale;
			isDirty		= true;			

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