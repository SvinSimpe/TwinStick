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
	if( !mSteeringBehaviourSystem->Update( deltaTime, mActors, mNumActiveActors, nullptr ) )
		return false;

	if( !mMovementSystem->Update( deltaTime, mActors, mNumActiveActors, nullptr ) )
		return false;

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
	mCameraSystem = std::make_unique<CameraSystem>( XMFLOAT3( 0.0f, 200.0f, -450.0f ) );
	if( !mCameraSystem )
		return false;

	mGraphicSystem = std::make_unique<GraphicSystem>();
	if( !mGraphicSystem->Initialize( mHWnd ) )
		return false;

	mMovementSystem = std::make_unique<MovementSystem>();
	if( !mMovementSystem )
		return false;

	mSteeringBehaviourSystem = std::make_unique<SteeringBehaviourSystem>();
	if( !mSteeringBehaviourSystem )
		return false;

	return true;

}

bool Engine::InitializeActors()
{
	try
	{
		mActors = std::make_unique<ActorCollection>();

		mActors->mIsActive.reserve( GameGlobals::MAX_ACTORS );
		mActors->mComponentMasks.reserve( GameGlobals::MAX_ACTORS );
		mActors->mActorType.reserve( GameGlobals::MAX_ACTORS );
		mActors->mTransformComponents.reserve( GameGlobals::MAX_ACTORS );
		mActors->mMeshComponents.reserve( GameGlobals::MAX_ACTORS );
		mActors->mHealthComponents.reserve( GameGlobals::MAX_ACTORS );
		mActors->mMovementComponents.reserve( GameGlobals::MAX_ACTORS );
		mActors->mSteeringBehaviorComponents.reserve( GameGlobals::MAX_ACTORS );
		mActors->mCollisionComponents.reserve( GameGlobals::MAX_ACTORS );


		for( size_t i = 0; i < GameGlobals::MAX_ACTORS; i++ )
		{
			mActors->mIsActive.push_back( false );
			mActors->mComponentMasks.push_back( 0 );
			mActors->mActorType.push_back( EActorType::None );
			mActors->mTransformComponents.push_back( std::make_unique<TransformComponent>() );
			mActors->mMeshComponents.push_back( std::make_unique<MeshComponent>() );
			mActors->mHealthComponents.push_back( std::make_unique<HealthComponent>() );
			mActors->mMovementComponents.push_back( std::make_unique<MovementComponent>() );
			mActors->mSteeringBehaviorComponents.push_back( std::make_unique<SteeringBehaviourComponent>() );
			mActors->mCollisionComponents.push_back( std::make_unique<CollisionComponent>() );

		}
	}
	catch( const std::exception& )
	{
		OutputDebugStringA( "Error: Failed to initialize actors!" );
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
			std::swap( mActors->mComponentMasks[i], mActors->mComponentMasks[mNumActiveActors] );
			std::swap( mActors->mActorType[i], mActors->mActorType[mNumActiveActors] );
			mActors->mTransformComponents[i].swap( mActors->mTransformComponents[mNumActiveActors] );
			mActors->mMeshComponents[i].swap( mActors->mMeshComponents[mNumActiveActors] );
			mActors->mHealthComponents[i].swap( mActors->mHealthComponents[mNumActiveActors] );
			mActors->mMovementComponents[i].swap( mActors->mMovementComponents[mNumActiveActors] );
			mActors->mSteeringBehaviorComponents[i].swap( mActors->mSteeringBehaviorComponents[mNumActiveActors] );
			mActors->mCollisionComponents[i].swap( mActors->mCollisionComponents[mNumActiveActors] );
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

	mGraphicSystem				= nullptr;
	mCameraSystem				= nullptr;
	mMovementSystem				= nullptr;
	mSteeringBehaviourSystem	= nullptr;
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
	QueryPerformanceFrequency( (LARGE_INTEGER*)&cntsPerSec );
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

			// Update Engine modules
			if( !Update( deltaTime ) ) 
				return -1;

			prevTimeStamp = currTimeStamp;
		}
	}

	return static_cast<int>( msg.wParam );

}

const bool Engine::RequestActor( std::vector<std::unique_ptr<IComponent>>& componentList )
{
	if( !componentList.empty() && mNumActiveActors < GameGlobals::MAX_ACTORS )
	{
		const size_t& i			= mNumActiveActors;
		mActors->mIsActive[i]	= true;
		mActors->mActorType[i] = ( i == 0 ) ? EActorType::Player : EActorType::Enemy;

		// Set Components
		for( auto& component : componentList )
		{
			switch( component->GetType() )
			{
				case EComponentType::Transform :
				{
					if( mActors->mTransformComponents[mNumActiveActors]->Set( component ) )
					{
						mActors->mComponentMasks[i] = static_cast<size_t>( 
							mActors->mComponentMasks[i] | EComponentType::Transform );
					}
					else
						OutputDebugString( "Error: Unable to set TransformComponent data" );

					break;
				}
				case EComponentType::Mesh :
				{
					if( mActors->mMeshComponents[mNumActiveActors]->Set( component ) )
					{
						mActors->mComponentMasks[i] = static_cast<size_t>( 
							mActors->mComponentMasks[i] | EComponentType::Mesh );
					}
					else
						OutputDebugString( "Error: Unable to set MeshComponent data" );

					break;
				}
				case EComponentType::Health :
				{
					if( mActors->mHealthComponents[mNumActiveActors]->Set( component ) )
					{
						mActors->mComponentMasks[i] = static_cast<size_t>( 
							mActors->mComponentMasks[i] | EComponentType::Health );
					}
					else
						OutputDebugString( "Error: Unable to set HealthComponent data" );

					break;
				}
				case EComponentType::Movement :
				{
					if( mActors->mMovementComponents[mNumActiveActors]->Set( component ) )
					{
						mActors->mComponentMasks[i] = static_cast<size_t>( 
							mActors->mComponentMasks[i] | EComponentType::Movement );
					}
					else
						OutputDebugString( "Error: Unable to set MovementComponent data" );

					break;
				}
				case EComponentType::SteeringBehaviour :
				{
					if( mActors->mSteeringBehaviorComponents[mNumActiveActors]->Set( component ) )
					{
						mActors->mComponentMasks[i] = static_cast<size_t>( 
							mActors->mComponentMasks[i] | EComponentType::SteeringBehaviour );
					}
					else
						OutputDebugString( "Error: Unable to set SteeringBehaviourComponent data" );

					break;
				}
				default:
					break;
			}
		}
		mNumActiveActors++;

	}

	return true;
}
