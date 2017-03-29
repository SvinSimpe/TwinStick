#pragma once

#include "IComponent.h"
#include "EMeshType.h"

struct MeshComponent : public IComponent
{
	bool				isVisible;
	EMeshType			meshType;
	DirectX::XMFLOAT4	color;

	MeshComponent( bool newVisibility = false,
				   const EMeshType newMeshType = EMeshType::Cube,
				   DirectX::XMFLOAT4 newColor = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) )
	{
		isVisible	= newVisibility;
		meshType	= newMeshType;
		color		= newColor;
	}

	virtual const EComponentType GetType() const
	{
		return EComponentType::Mesh;

	}

	virtual const bool Set( const std::unique_ptr<IComponent>& newData )
	{
		try
		{
			MeshComponent& newMesh = dynamic_cast<MeshComponent&>( *newData );
			isVisible	= newMesh.isVisible;
			meshType	= newMesh.meshType;
			color		= newMesh.color;

		}
		catch( const std::exception& exc )
		{
			std::cerr << "Error occurred: " << exc.what() << std::endl;
			return false;

		}

		return true;

	}

};