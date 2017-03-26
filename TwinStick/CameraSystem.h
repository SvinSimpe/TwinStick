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

	public:
		CameraSystem();
		~CameraSystem();

		bool Initialize();
		void FollowActor( const size_t actorID = 0, const bool followActor = true ) noexcept;
		void SetFocusPoint( const DirectX::XMFLOAT3& newFocusPoint = DirectX::XMFLOAT3() ) noexcept;


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