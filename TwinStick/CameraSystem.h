#pragma once

#include "ISystems.h"
#include <DirectXMath.h>


class CameraSystem : public ISystems
{
	private:
		DirectX::XMFLOAT3 mCameraLocation;
		DirectX::XMFLOAT3 mFocusPoint;
		DirectX::XMFLOAT3 mLookVector;
		DirectX::XMFLOAT3 mUpVector;
		DirectX::XMFLOAT3 mRightVector;
		

		DirectX::XMFLOAT4X4 mViewMatrix;
		DirectX::XMFLOAT4X4 mProjectionMatrix;
		

		size_t	mFollowedActorID;
		bool	mIsFollowingActor;
		float	mDistanceToFollowedActor;

		float	mMaxCullDistance;
		float	mMinCullDistance;
		float	mFieldOfView;

	private:
		float	mRotationY;
		void Rotate( float deltaTime, float angle );

	public:
		//CameraSystem();
		CameraSystem( const DirectX::XMFLOAT3 cameraLocation = DirectX::XMFLOAT3( 10.0f, 10.0f, -10.0f ),
					  const DirectX::XMFLOAT3 focusPoint = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ), 
					  const float minCullDistance = 0.5f,
					  const float maxCullDistance = 5000.0f,
					  const float fieldOfView = 0.9f );
		~CameraSystem();
			
		void FollowActor( const size_t actorID = 0, const bool followActor = true ) noexcept;
		void SetFocusPoint( const DirectX::XMFLOAT3& newFocusPoint = DirectX::XMFLOAT3() ) noexcept;
		void SetDistanceToFollowedActor( const float distanceToActor ) noexcept;
		
		inline DirectX::XMFLOAT4 GetCameraLocation() const noexcept
		{
			return DirectX::XMFLOAT4( mCameraLocation.x,
									  mCameraLocation.y,
									  mCameraLocation.z,
									  1.0f );

		}

		inline DirectX::XMFLOAT4X4 GetViewMatrix() const noexcept
		{
			return mViewMatrix;

		}

		inline DirectX::XMFLOAT4X4 GetProjectionMatrix() const noexcept
		{
			return mProjectionMatrix;

		}

		inline DirectX::XMFLOAT4X4 GetViewMatrixTranspose() const noexcept
		{
			DirectX::XMMATRIX temp = XMMatrixTranspose( XMLoadFloat4x4( &mViewMatrix ) );
			DirectX::XMFLOAT4X4 viewTransposed;
			XMStoreFloat4x4( &viewTransposed, temp );

			return viewTransposed;

		}

		inline DirectX::XMFLOAT4X4 GetProjectionMatrixTranspose() const noexcept
		{
			DirectX::XMMATRIX temp = XMMatrixTranspose( XMLoadFloat4x4( &mProjectionMatrix ) );
			DirectX::XMFLOAT4X4 projectionTransposed;
			XMStoreFloat4x4( &projectionTransposed, temp );

			return projectionTransposed;

		}

		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							 size_t numActiveActors, void* systemSpecificInput ) override;

};