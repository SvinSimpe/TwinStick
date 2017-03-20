#pragma once

#include <memory>

struct ActorCollection;

__interface ISystems
{
	virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors ) = 0;

};