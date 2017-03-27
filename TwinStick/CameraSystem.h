#pragma once

#include "ISystems.h"
#include <DirectXMath.h>


class CameraSystem : public ISystems
{
	private:
		DirectX::XMFLOAT3 mCameraLocation;
		DirectX::XMFLOAT3 mFocusPoint;
		DirectX::XMFLOAT3 mUpVector;
		DirectX::XMFLOAT3 mRightVector;
		DirectX::XMFLOAT3 mLookVector;

		DirectX::XMFLOAT4X4 mViewMatrix;
		DirectX::XMFLOAT4X4 mProjectionMatrix;
		

		size_t	mFollowedActorID;
		bool	mIsFollowingActor;
		float	mDistanceToFollowedActor;

		float	mMaxDrawDistance;
		float	mMinDrawDistance;
		float	mFieldOfView;

	public:
		//CameraSystem();
		CameraSystem( const DirectX::XMFLOAT3 cameraLocation = DirectX::XMFLOAT3(),
					  const DirectX::XMFLOAT3 focusPoint = DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f ), 
					  const DirectX::XMFLOAT3 upVector = DirectX::XMFLOAT3( 0.0f, 1.0f, 1.0f ),
					  const DirectX::XMFLOAT3 rightVector = DirectX::XMFLOAT3( 1.0f, 0.0f, 0.0f ),
					  const float minDrawDistance = 0.5f,
					  const float maxDrawDistance = 5000.0f,
					  const float fieldOfView = 0.75f );
		~CameraSystem();
			
		bool Initialize( const float minDrawDistance = 0.5f, const float maxDrawDistance = 5000.0f, const float fieldOfView = 0.75 );
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

		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							 size_t numActiveActor, void* systemSpecificInput ) override;

};