#pragma once

#include <vector>
#include <memory>

#include "TransformComponent.h"
#include "CameraComponent.h"

struct ActorCollection
{
	std::vector<bool>	mIsActive;
	std::vector<size_t>	componentMasks;

	std::vector<std::unique_ptr<TransformComponent>>	mTransformComponents;
	std::vector<std::unique_ptr<CameraComponent>>		mCameraComponents;

};