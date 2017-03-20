#pragma once

#include "ISystems.h"

class CameraSystem : public ISystems
{

	CameraSystem();
	~CameraSystem();


	// Inherited via ISystems
	virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors ) override;

};