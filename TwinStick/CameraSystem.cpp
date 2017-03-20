#include "CameraSystem.h"
#include "ActorCollection.h"


#define CAMERA_MASK ( EComponentType::Transform | EComponentType::Camera )



CameraSystem::CameraSystem()
{}

CameraSystem::~CameraSystem()
{}

bool CameraSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors )
{
	for( const auto& mask : actors->componentMasks )
	{
		if( ( mask & CAMERA_MASK ) == CAMERA_MASK )
		{
			// Update Actor Camera Component

		}
	}

	return true;
}
