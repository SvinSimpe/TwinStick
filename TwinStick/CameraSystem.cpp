#include "CameraSystem.h"
#include "ActorCollection.h"
#include "Utility.h"
using namespace DirectX;

#define CAMERA_MASK ( EComponentType::Transform )


//CameraSystem::CameraSystem()
//{
//	mFocusPoint		= XMFLOAT3();
//	mUpVector		= XMFLOAT3();
//	mRightVector	= XMFLOAT3();
//	mLookVector		= XMFLOAT3();
//
//	mViewMatrix			= XMFLOAT4X4();
//	mProjectionMatrix	= XMFLOAT4X4();
//
//	mFollowedActorID	= -1;
//	mIsFollowingActor	= false;
//	
//	mMinDrawDistance	= 0.0f;
//	mMaxDrawDistance	= 0.0f;
//
//}

CameraSystem::CameraSystem( const XMFLOAT3 cameraLocation, const XMFLOAT3 focusPoint, const XMFLOAT3 upVector,
							const XMFLOAT3 rightVector, const float minDrawDistance, const float maxDrawDistance, const float fieldOfView )
{
	mCameraLocation	= cameraLocation;
	mFocusPoint		= focusPoint;
	mUpVector		= upVector;
	mRightVector	= rightVector;
	
	XMVECTOR tempLookVector = XMLoadFloat3( &mFocusPoint ) - XMLoadFloat3( &mCameraLocation );
	XMStoreFloat3( &mLookVector, tempLookVector );

	mMinDrawDistance	= minDrawDistance;
	mMaxDrawDistance	= maxDrawDistance;
	mFieldOfView		= fieldOfView;

	mFollowedActorID	= -1;
	mIsFollowingActor	= false;


	XMMATRIX tempViewMatrix = XMMatrixLookAtLH( XMLoadFloat3( &mCameraLocation ), 
												XMLoadFloat3( &mFocusPoint ),
												XMLoadFloat3( &mUpVector ) );
	XMStoreFloat4x4( &mViewMatrix, tempViewMatrix );


	XMMATRIX tempProjectionMatrix = XMMatrixPerspectiveFovLH( mFieldOfView,
		(float)Resolution::SCREEN_WIDTH / (float)Resolution::SCREEN_HEIGHT,
															  mMinDrawDistance, mMaxDrawDistance );
	XMStoreFloat4x4( &mProjectionMatrix, tempProjectionMatrix );

}

CameraSystem::~CameraSystem()
{}

bool CameraSystem::Initialize( const float minDrawDistance, const float maxDrawDistance, const float fieldOfView )
{
	mMinDrawDistance	= minDrawDistance;
	mMaxDrawDistance	= maxDrawDistance;
	mFieldOfView		= fieldOfView;

	XMMATRIX tempProjectionMatrix = XMMatrixPerspectiveFovLH( mFieldOfView,
		(float)Resolution::SCREEN_WIDTH / (float)Resolution::SCREEN_HEIGHT,
															  mMinDrawDistance, mMaxDrawDistance );
	XMStoreFloat4x4( &mProjectionMatrix, tempProjectionMatrix );

	return true;

}

void CameraSystem::FollowActor( const size_t actorID, const bool followActor ) noexcept
{
	mFollowedActorID	= actorID;
	mIsFollowingActor	= followActor;

}

void CameraSystem::SetFocusPoint( const XMFLOAT3& newFocusPoint ) noexcept
{
	mFocusPoint = newFocusPoint;

}

void CameraSystem::SetDistanceToFollowedActor( const float distanceToActor ) noexcept
{
	mDistanceToFollowedActor = distanceToActor;

}

bool CameraSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
						   size_t numActiveActor, void* systemSpecificInput )
{
	XMVECTOR E = XMLoadFloat3( &mCameraLocation );
	XMVECTOR R = XMLoadFloat3( &mRightVector );
	XMVECTOR U = XMLoadFloat3( &mUpVector );
	XMVECTOR L = XMVectorZero();

	if( mIsFollowingActor )
	{
		if( actors->mIsActive[mFollowedActorID] &&
			( actors->componentMasks[mFollowedActorID] & CAMERA_MASK ) == CAMERA_MASK )
		{
			std::unique_ptr<TransformComponent>& actorTransform = 
				actors->mTransformComponents[mFollowedActorID];
			
			// Update CameraLocation
			XMVECTOR newCameraLocation = XMLoadFloat3( &actorTransform->location ) -
				XMLoadFloat3( &mLookVector ) * mDistanceToFollowedActor;
			XMStoreFloat3( &mCameraLocation, newCameraLocation );
			E = XMLoadFloat3( &mCameraLocation );

			L = XMLoadFloat3( &actorTransform->location  ) -
				XMLoadFloat3( &mCameraLocation );

		}
	}
	else
		L = XMLoadFloat3( &mFocusPoint ) - XMLoadFloat3( &mCameraLocation );


	L = XMVector3Normalize( L );

	// Compute new corrected Up Vector
	U = XMVector3Normalize( XMVector3Cross( L, R ) );

	// Compute new corrected Right Vector
	R = XMVector3Cross( U, L );

	// Fill View Matrix entries
	float x = -XMVectorGetX( XMVector3Dot( E, R ) );
	float y = -XMVectorGetX( XMVector3Dot( E, U ) );
	float z = -XMVectorGetX( XMVector3Dot( E, L ) );

	XMStoreFloat3( &mRightVector, R );
	XMStoreFloat3( &mUpVector, U );
	XMStoreFloat3( &mLookVector, L );

	mViewMatrix( 0, 0 ) = mRightVector.x;
	mViewMatrix( 1, 0 ) = mRightVector.y;
	mViewMatrix( 2, 0 ) = mRightVector.z;
	mViewMatrix( 3, 0 ) = x;

	mViewMatrix( 0, 1 ) = mUpVector.x;
	mViewMatrix( 1, 1 ) = mUpVector.y;
	mViewMatrix( 2, 1 ) = mUpVector.z;
	mViewMatrix( 3, 1 ) = y;

	mViewMatrix( 0, 2 ) = mLookVector.x;
	mViewMatrix( 1, 2 ) = mLookVector.y;
	mViewMatrix( 2, 2 ) = mLookVector.z;
	mViewMatrix( 3, 2 ) = z;

	mViewMatrix( 0, 3 ) = 0.0f;
	mViewMatrix( 1, 3 ) = 0.0f;
	mViewMatrix( 2, 3 ) = 0.0f;
	mViewMatrix( 3, 3 ) = 1.0f;
		
	return true;

}