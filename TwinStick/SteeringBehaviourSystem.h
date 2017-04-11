#pragma once

#include "ISystems.h"

class SteeringBehaviourSystem : public ISystems
{

	private:

		void CalculateActorSteeringAndVelocity( std::unique_ptr<TransformComponent>& transformComp,
												std::unique_ptr<MovementComponent>& moveComp,
												std::unique_ptr<SteeringBehaviourComponent>& steerComp,
												const float directionModifier = 1.0f ) const;

	public:
		SteeringBehaviourSystem();
		~SteeringBehaviourSystem();

		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							 size_t numActiveActors, void* systemSpecificInput ) override;

};