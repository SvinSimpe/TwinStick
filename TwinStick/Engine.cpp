#include "Engine.h"
#include "Utility.h"

Engine* gEngine = 0;

LRESULT CALLBACK MainWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return gEngine->WndProc( hwnd, msg, wParam, lParam );

}


bool Engine::Update( float deltaTime )
{
	CheckInactiveActors();


	// Update systems



	// Update last
	if( !mCameraSystem->Update( deltaTime, mActors, mNumActiveActors, nullptr ) )
		return false;

	FrameData newFrameData = { mCameraSystem->GetViewMatrixTranspose(),
								mCameraSystem->GetProjectionMatrixTranspose(),
								mCameraSystem->GetCameraLocation() };

	if( !mGraphicSystem->Update( deltaTime, mActors, mNumActiveActors, &newFrameData ) )
		return false;

	return true;

}

bool Engine::InitializeSystems()
{
	mCameraSystem = std::make_unique<CameraSystem>( XMFLOAT3( 20.0f, -20.0f, -20.0f ) );
	if( !mCameraSystem )
		return false;

	mGraphicSystem = std::make_unique<GraphicSystem>();
	if( !mGraphicSystem->Initialize( mHWnd ) )
		return false;

	return true;

}

bool Engine::InitializeActors()
{
	try
	{
		mActors = std::make_unique<ActorCollection>();

		for( size_t i = 0; i < GameGlobals::MAX_ACTORS; i++ )
		{
			mActors->mIsActive.push_back( false );
			mActors->componentMasks.push_back( 0 );
			mActors->mTransformComponents.push_back( std::make_unique<TransformComponent>() );
			mActors->mMeshComponents.push_back( std::make_unique<MeshComponent>() );
			mActors->mHealthComponents.push_back( std::make_unique<HealthComponent>() );

		}

		mActors->mIsActive.resize( GameGlobals::MAX_ACTORS );
		mActors->componentMasks.resize( GameGlobals::MAX_ACTORS );
		mActors->mTransformComponents.resize( GameGlobals::MAX_ACTORS );
		mActors->mMeshComponents.resize( GameGlobals::MAX_ACTORS );
		mActors->mHealthComponents.resize( GameGlobals::MAX_ACTORS );
	}
	catch( const std::exception& )
	{
		return false;
	}

	return true;

}

void Engine::CheckInactiveActors()
{
	for( size_t i = 0; i < mNumActiveActors; i++ )
	{
		if( !mActors->mIsActive[i] )
		{
			std::swap( mActors->mIsActive[i], mActors->mIsActive[mNumActiveActors] );
			std::swap( mActors->componentMasks[i], mActors->componentMasks[mNumActiveActors] );
			mActors->mTransformComponents[i].swap( mActors->mTransformComponents[mNumActiveActors] );

			mNumActiveActors--;

		}
	}

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

	mGraphicSystem	= nullptr;
	mCameraSystem	= nullptr;
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

	if( !InitializeActors() )
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

const bool Engine::RequestActor( std::vector<std::unique_ptr<IComponent>>& componentList )
{
	if( !componentList.empty() )
	{
		if( mNumActiveActors < GameGlobals::MAX_ACTORS )
		{
			const size_t& i = mNumActiveActors;
			mActors->mIsActive[i] = true;

			// Set Components
			for( auto& component : componentList )
			{
				switch( component->GetType() )
				{
					case EComponentType::Transform :
					{
						if( mActors->mTransformComponents[mNumActiveActors]->Set( component ) )
						{
							mActors->componentMasks[i] = static_cast<size_t>( 
								mActors->componentMasks[i] | EComponentType::Transform );
						}
						else
							OutputDebugString( "Error: Unable to set TransformComponent data" );

						break;
					}
					case EComponentType::Mesh :
					{
						if( mActors->mMeshComponents[mNumActiveActors]->Set( component ) )
						{
							mActors->componentMasks[i] = static_cast<size_t>( 
								mActors->componentMasks[i] | EComponentType::Mesh );
						}
						else
							OutputDebugString( "Error: Unable to set MeshComponent data" );

						break;
					}
					case EComponentType::Health :
					{
						if( mActors->mHealthComponents[mNumActiveActors]->Set( component ) )
						{
							mActors->componentMasks[i] = static_cast<size_t>( 
								mActors->componentMasks[i] | EComponentType::Health );
						}
						else
							OutputDebugString( "Error: Unable to set HealthComponent data" );

						break;
					}
					default:
						break;
				}
			}
			mNumActiveActors++;

		}
	}

	return true;
}
