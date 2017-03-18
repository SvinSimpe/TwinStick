#pragma once

#include <Windows.h>

class Engine
{
	private:
		HINSTANCE	mHInstance;
		HWND		mHWnd;


		bool Update( float deltaTime );

	public:
		LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		
		Engine();
		~Engine();

		bool Initialize( HINSTANCE hInstance, int nCmdShow );
		int Run();

};