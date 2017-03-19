#pragma once

struct ActorCollection;

__interface ISystems
{
	virtual bool Update( float deltaTime, ActorCollection& actors ) = 0;

};