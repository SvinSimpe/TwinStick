#include "Engine.h"
#include "Utility.h"

Engine* gEngine = 0;

LRESULT CALLBACK MainWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return gEngine->WndProc( hwnd, msg, wParam, lParam );

}


bool Engine::Update( float deltaTime )
{
	// Update Systems
	//if( !mCameraSystem->Update( deltaTime, mActors ) )
	//	return false;

	if( !mGraphicSystem->Update( deltaTime, mActors ) )
		return false;

	return true;

}

bool Engine::InitializeSystems()
{

	mGraphicSystem = std::make_unique<GraphicSystem>();
	if( !mGraphicSystem->Initialize( mHWnd ) )
		return false;

	mCameraSystem = std::make_unique<CameraSystem>();
	if( !mCameraSystem->Initialize() )
		return false;

	return true;

}

LRESULT CALLBACK Engine::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( msg )
	{
		case WM_PAINT :
		{ 
			hdc = BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
			break;
		}
		case WM_DESTROY :
		{
			PostQuitMessage( 0 );
			break;
		}

		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	
	return 0;

}

Engine::Engine()
{
	mHInstance	= nullptr;
	mHWnd		= nullptr;

	mActors				= nullptr;
	mNumActiveActors	= 0;

	mGraphicSystem = nullptr;
	
	size_t width = Resolution::SCREEN_WIDTH;
}


Engine::~Engine()
{}

bool Engine::Initialize( HINSTANCE hInstance, int nCmdShow )
{
	mHInstance = hInstance;

	// Register Window Class
	WNDCLASSEX wc;
	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= mHInstance;
	wc.hIcon			= 0;
	wc.hCursor			= LoadCursor( NULL, IDC_HAND );
	wc.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "TwinStick Y Y  (^<^)";
	wc.hIconSm			= 0;

	if( !RegisterClassEx( &wc ) )
		return false;


	// Adjust & Create Window
	RECT rc = { 0, 0, Resolution::SCREEN_WIDTH, Resolution::SCREEN_HEIGHT };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if( !( mHWnd = CreateWindowA( "TwinStick Y Y  (^<^)",
								  "TwinStick Y Y  (^<^)",
								  WS_OVERLAPPEDWINDOW,
								  CW_USEDEFAULT,
								  CW_USEDEFAULT,
								  rc.right - rc.left,
								  rc.bottom - rc.top,
								  NULL,
								  NULL,
								  mHInstance,
								  NULL ) ) )
	{	
		return false;
	}

	ShowWindow( mHWnd, nCmdShow );
	ShowCursor( TRUE );


	if( !InitializeSystems() )
		return false;

	return true;
}

int Engine::Run()
{
	__int64 cntsPerSec = 0;
	
	//QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>( &cntsPerSec ) );
	QueryPerformanceFrequency( (LARGE_INTEGER*)&cntsPerSec );
	//float secsPerCnt = 1.0f / (float)cntsPerSec;
	float secsPerCnt = 1.0f / static_cast<float>( cntsPerSec );

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter( (LARGE_INTEGER*)&prevTimeStamp );

	MSG msg = { 0 };

	while ( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter( (LARGE_INTEGER*)&currTimeStamp );
			float deltaTime = ( currTimeStamp - prevTimeStamp ) * secsPerCnt;

			// Update & Render
			if( !Update( deltaTime ) ) 
				return -1;
			//Render();

			prevTimeStamp = currTimeStamp;
		}
	}

	return static_cast<int>( msg.wParam );

}
