#include "CameraSystem.h"
#include "ActorCollection.h"
#include "Utility.h"

#define CAMERA_MASK ( EComponentType::Transform | EComponentType::Camera )


CameraSystem::CameraSystem()
{}

CameraSystem::~CameraSystem()
{}

bool CameraSystem::Initialize()
{
	return true;
}

bool CameraSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActor )
{
	for( size_t i = 0; i < numActiveActor; i++ )
	{
		if( actors->mIsActive[i] && ( actors->componentMasks[i] & CAMERA_MASK ) == CAMERA_MASK )
		{
			std::unique_ptr<CameraComponent>& camComp = actors->mCameraComponents[i];

			XMVECTOR L = XMVectorZero();
			if( IsVector3Zero( camComp->mLookVector ) )
			{ 
				L = XMLoadFloat3( &actors->mTransformComponents[i]->location ) -
					XMLoadFloat3( &camComp->mFocusPoint );
			}
			else
				L = XMLoadFloat3( &camComp->mLookVector );


			// Update View Matrix
			XMVECTOR R = XMLoadFloat3( &camComp->mRightVector );
			XMVECTOR U = XMLoadFloat3( &camComp->mUpVector );
			XMVECTOR E = XMLoadFloat3( &actors->mTransformComponents[i]->location );

			L = XMVector3Normalize( L );

			// Compute new corrected Up Vector
			U = XMVector3Normalize( XMVector3Cross( L, R ) );

			// Compute new corrected Right Vector
			R = XMVector3Cross( U, L );

			// Fill View Matrix entries
			float x = -XMVectorGetX( XMVector3Dot( E, R ) );
			float y = -XMVectorGetX( XMVector3Dot( E, U ) );
			float z = -XMVectorGetX( XMVector3Dot( E, L ) );

			XMStoreFloat3( &camComp->mRightVector, R );
			XMStoreFloat3( &camComp->mUpVector, U );
			XMStoreFloat3( &camComp->mLookVector, L );

			camComp->mViewMatrix( 0, 0 ) = camComp->mRightVector.x;
			camComp->mViewMatrix( 1, 0 ) = camComp->mRightVector.y;
			camComp->mViewMatrix( 2, 0 ) = camComp->mRightVector.z;
			camComp->mViewMatrix( 3, 0 ) = x;

			camComp->mViewMatrix( 0, 1 ) = camComp->mUpVector.x;
			camComp->mViewMatrix( 1, 1 ) = camComp->mUpVector.y;
			camComp->mViewMatrix( 2, 1 ) = camComp->mUpVector.z;
			camComp->mViewMatrix( 3, 1 ) = y;

			camComp->mViewMatrix( 0, 2 ) = camComp->mLookVector.x;
			camComp->mViewMatrix( 1, 2 ) = camComp->mLookVector.y;
			camComp->mViewMatrix( 2, 2 ) = camComp->mLookVector.z;
			camComp->mViewMatrix( 3, 2 ) = z;

			camComp->mViewMatrix( 0, 3 ) = 0.0f;
			camComp->mViewMatrix( 1, 3 ) = 0.0f;
			camComp->mViewMatrix( 2, 3 ) = 0.0f;
			camComp->mViewMatrix( 3, 3 ) = 1.0f;

		}
	}

	return true;

}
