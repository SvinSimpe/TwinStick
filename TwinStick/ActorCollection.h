#pragma once

#include <vector>
#include <memory>

#include "TransformComponent.h"
#include "CameraComponent.h"

#define MAX_ACTORS 256

struct ActorCollection
{
	std::vector<bool>			mIsActive;
	std::vector<unsigned int>	componentMasks;

	std::vector<std::unique_ptr<TransformComponent>>	mTransformComponents;
	std::vector<std::unique_ptr<CameraComponent>>		mCameraComponents;

};