#pragma once

#include "ISystems.h"

class CameraSystem : public ISystems
{
	public:
		CameraSystem();
		~CameraSystem();

		bool Initialize();

		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors ) override;

};