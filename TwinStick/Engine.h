#pragma once

#include <Windows.h>

#include <memory>
#include <vector>



#include "GraphicSystem.h"
#include "CameraSystem.h"
#include "MovementSystem.h"
#include "SteeringBehaviourSystem.h"
#include "CollisionSystem.h"


class Engine
{
	private:
		HINSTANCE	mHInstance;
		HWND		mHWnd;

		std::unique_ptr<ActorCollection>	mActors;
		unsigned int						mNumActiveActors;

		// Systems
		std::unique_ptr<GraphicSystem>				mGraphicSystem;
		std::unique_ptr<CameraSystem>				mCameraSystem;
		std::unique_ptr<MovementSystem>				mMovementSystem;
		std::unique_ptr<SteeringBehaviourSystem>	mSteeringBehaviourSystem;
		std::unique_ptr<CollisionSystem>			mCollisionSystem;

	private:
		bool Update( float deltaTime );
		bool InitializeSystems();
		bool InitializeActors();
		void CheckInactiveActors();



	public:
		LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		
		Engine();
		~Engine();

		bool Initialize( HINSTANCE hInstance, int nCmdShow );
		int Run();

		const bool RequestActor( std::vector<std::unique_ptr<IComponent>>& componentList );

};