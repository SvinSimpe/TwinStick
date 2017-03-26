#pragma once

#include <memory>

struct ActorCollection;

__interface ISystems
{
	virtual bool Update( float deltaTime, 
						 std::unique_ptr<ActorCollection>& actors,
						 size_t numActiveActor,
						 void* systemSpecificInput ) = 0;

};