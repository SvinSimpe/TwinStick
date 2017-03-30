#pragma once

#include <memory>
#include "ActorCollection.h"

__interface ISystems
{
	virtual bool Update( float deltaTime, 
						 std::unique_ptr<ActorCollection>& actors,
						 size_t numActiveActors,
						 void* systemSpecificInput ) = 0;

};