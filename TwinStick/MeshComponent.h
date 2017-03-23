#pragma once

#include "IComponent.h"
#include "EMeshType.h"

struct MeshComponent : public IComponent
{
	EMeshType	meshType;
	bool		isVisible;

	MeshComponent( const EMeshType newMeshType = EMeshType::Cube,
				   bool newVisibility = false )
	{
		meshType	= newMeshType;
		isVisible	= newVisibility;

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
			meshType	= newMesh.meshType;
			isVisible	= newMesh.isVisible;

		}
		catch( const std::exception& exc )
		{
			std::cerr << "Error occurred: " << exc.what() << std::endl;
			return false;

		}

		return true;

	}

};