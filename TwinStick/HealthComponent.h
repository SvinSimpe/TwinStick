#pragma once

#include "IComponent.h"

struct HealthComponent : public IComponent
{
	float	health;
	float	maxHealth;
	bool	isAlive;
	bool	isHealthVisible;

	HealthComponent( const float health = 100.0f, const float maxHealth = 100.0f,
					 const bool isAlive = true, const bool isHealthVisible = false )
	{
		this->health			= health;
		this->maxHealth			= maxHealth;
		this->isAlive			= isAlive;
		this->isHealthVisible	= isHealthVisible;

	}

	inline virtual const EComponentType GetType() const
	{
		return EComponentType::Health;

	}
	virtual const bool Set( const std::unique_ptr<IComponent>& newData )
	{

		try
		{
			HealthComponent& newHealth = dynamic_cast<HealthComponent&>( *newData );
			health			= newHealth.health;
			maxHealth		= newHealth.maxHealth;
			isAlive			= newHealth.isAlive;
			isHealthVisible = newHealth.isHealthVisible;
		}
		catch( const std::exception& exc )
		{
			std::cerr << "Error occurred: " << exc.what() << std::endl;
			return false;

		}

		return true;

	}

};