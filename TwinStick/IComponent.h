#pragma once

#include <memory>

#if _DEBUG
	#include <iostream>
#endif

#include "EComponentType.h"


__interface IComponent
{
	virtual const EComponentType GetType() const = 0;
	virtual const bool Set( const std::unique_ptr<IComponent>& newData ) = 0;

};