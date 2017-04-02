#pragma once

#include <vector>
#include <memory>

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "SteeringBehaviourComponent.h"

struct ActorCollection
{
	std::vector<bool>	mIsActive;
	std::vector<size_t>	componentMasks;

	std::vector<std::unique_ptr<TransformComponent>>			mTransformComponents;
	std::vector<std::unique_ptr<MeshComponent>>					mMeshComponents;
	std::vector<std::unique_ptr<HealthComponent>>				mHealthComponents;
	std::vector<std::unique_ptr<MovementComponent>>				mMovementComponents;
	std::vector<std::unique_ptr<SteeringBehaviourComponent>>	mSteeringBehaviorComponents;
};