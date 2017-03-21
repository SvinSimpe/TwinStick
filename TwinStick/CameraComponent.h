#pragma once

#include "IComponent.h"
#include <DirectXMath.h>
using namespace DirectX;

struct CameraComponent : public IComponent
{

	XMFLOAT3 mFocusPoint	= XMFLOAT3();
	XMFLOAT3 mUpVector		= XMFLOAT3();
	XMFLOAT3 mRightVector	= XMFLOAT3();
	XMFLOAT3 mLookVector	= XMFLOAT3();

	XMFLOAT4X4	mViewMatrix			= XMFLOAT4X4();
	XMFLOAT4X4	mProjectionMatrix	= XMFLOAT4X4();

	CameraComponent( XMFLOAT3 focusPoint = XMFLOAT3() )
	{
		mFocusPoint = focusPoint;

	}

	// Inherited via IComponent
	virtual const EComponentType GetType() const override
	{
		return EComponentType::Camera;

	}

	virtual const bool Set( const std::unique_ptr<IComponent>& newData )
	{
		try
		{
			CameraComponent& newCamera = dynamic_cast<CameraComponent&>( *newData );

			mFocusPoint		= newCamera.mFocusPoint;
			mUpVector		= newCamera.mUpVector;
			mRightVector	= newCamera.mRightVector;
			mLookVector		= newCamera.mLookVector;

		}
		catch( const std::exception& exc )
		{
			std::cerr << "Error occurred: " << exc.what() << std::endl;
			return false;

		}


		return true;

	}

};