#include "CameraSystem.h"
#include "ActorCollection.h"
#include "Utility.h"
#include "stdafx.h"
#include <sstream>

using namespace DirectX;

#define CAMERA_MASK ( EComponentType::Transform )

void CameraSystem::Rotate( float deltaTime, float angle  )
{
	mRotationY += deltaTime  * 5000.0f;// *0.01f;
	float radian = 10.01f;//ConvertToRadians( atanf( deltaTime ) ) * 5.0f;

	std::ostringstream ss;
	ss << radian;
	OutputDebugStringA( ss.str().c_str() );
	OutputDebugStringA( "\n" );


	XMMATRIX rotation		= XMMatrixRotationY( XMConvertToRadians( angle * deltaTime ) );

	// Rotate Camera Axes at origin
	XMVECTOR rotatedLook	=  XMVector3TransformCoord( XMLoadFloat3( &mLookVector ), rotation );
	XMVECTOR rotatedUp		=  XMVector3TransformCoord( XMLoadFloat3( &mUpVector ), rotation );
	XMVECTOR rotatedRight	=  XMVector3TransformCoord( XMLoadFloat3( &mRightVector ), rotation );


	// Translate look to find new Camera Location
	XMMATRIX translation				= XMMatrixTranslationFromVector( -rotatedLook * mDistanceToFollowedActor );
	XMVECTOR rotatedAndTranslatedLook	=  XMVector3TransformCoord( rotatedLook, translation );


	XMStoreFloat3( &mCameraLocation, rotatedAndTranslatedLook );
	XMStoreFloat3( &mLookVector, rotatedLook );
	XMStoreFloat3( &mUpVector, rotatedUp );
	XMStoreFloat3( &mRightVector, rotatedRight );

}

CameraSystem::CameraSystem( const XMFLOAT3 cameraLocation, const XMFLOAT3 focusPoint,
							const float minDrawDistance, const float maxDrawDistance, const float fieldOfView )
{
	mCameraLocation	= cameraLocation;
	mFocusPoint		= focusPoint;

	XMVECTOR look = XMLoadFloat3( &mFocusPoint ) - XMLoadFloat3( &mCameraLocation );
	XMStoreFloat3( &mLookVector, XMVector3Normalize( look ) );
	XMStoreFloat3( &mRightVector, XMVector3Cross( XMLoadFloat3( &mLookVector ),
												  XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) ) );
	XMStoreFloat3( &mUpVector, XMVector3Cross( XMLoadFloat3( &mLookVector ),
											   XMLoadFloat3( &mRightVector ) ) );


	
	mFollowedActorID			= -1;
	mIsFollowingActor			= false;
	mDistanceToFollowedActor	= XMVectorGetX( XMVector3Length( look ) );

	mMinCullDistance	= minDrawDistance;
	mMaxCullDistance	= maxDrawDistance;
	mFieldOfView		= fieldOfView;

	XMStoreFloat4x4( &mViewMatrix, XMMatrixLookAtLH( XMLoadFloat3( &mCameraLocation ), 
												XMLoadFloat3( &mFocusPoint ),
												XMLoadFloat3( &mUpVector ) ) );

	XMStoreFloat4x4( &mProjectionMatrix, XMMatrixPerspectiveFovLH( mFieldOfView, 
															  static_cast<float>(Resolution::SCREEN_WIDTH) / 
															  static_cast<float>(Resolution::SCREEN_HEIGHT),
															  mMinCullDistance, mMaxCullDistance ) );



	mRotationY = 0.0f;

}

CameraSystem::~CameraSystem()
{}

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

	Rotate( deltaTime, -50.0f );

	XMVECTOR E = XMLoadFloat3( &mCameraLocation );
	XMVECTOR R = XMLoadFloat3( &mRightVector );
	XMVECTOR U = XMLoadFloat3( &mUpVector );
	XMVECTOR L = XMLoadFloat3( &mLookVector );

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