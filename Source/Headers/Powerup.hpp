#pragma once

#include <SFML/Graphics.hpp>
#include "Global.hpp"


struct Powerup
{
	bool dead;

	short x;
	short y;

	Power_type type;

	Powerup(short i_x, short i_y, Power_type i_type);
	sf::IntRect get_hitbox() const;
	void tick();
};