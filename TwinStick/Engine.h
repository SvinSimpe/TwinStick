#pragma once

#include <Windows.h>
#include "GraphicSystem.h"
#include <memory>

struct ActorCollection
{

};

class Engine
{
	private:
		HINSTANCE	mHInstance;
		HWND		mHWnd;

		ActorCollection mActors;

		// Systems
		std::unique_ptr<GraphicSystem> mGraphicSystem;

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