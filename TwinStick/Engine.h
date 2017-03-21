#pragma once

#include <Windows.h>

#include <memory>
#include <vector>

#include "ActorCollection.h"

#include "GraphicSystem.h"
#include "CameraSystem.h"



class Engine
{
	private:
		HINSTANCE	mHInstance;
		HWND		mHWnd;

		std::unique_ptr<ActorCollection> mActors;
		unsigned int mNumActiveActors;

		// Systems
		std::unique_ptr<GraphicSystem> mGraphicSystem;
		std::unique_ptr<CameraSystem> mCameraSystem;

	private:
		bool Update( float deltaTime );
		bool InitializeSystems();


	public:
		LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		
		Engine();
		~Engine();

		bool Initialize( HINSTANCE hInstance, int nCmdShow );
		int Run();

};