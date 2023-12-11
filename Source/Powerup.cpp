#include <chrono>

#include "Headers/Powerup.hpp"

Powerup::Powerup(short i_x, short i_y, Power_type i_type) :
		dead(0),
		x(i_x),
		y(i_y),
		type(i_type)
{
		
}

sf::IntRect Powerup::get_hitbox() const
{
    return sf::IntRect(x + 0.25f * BASE_SIZE, y + 0.25f * BASE_SIZE, 0.5f * BASE_SIZE, 0.5f * BASE_SIZE);
}

void Powerup::tick() {
    	y += POWERUP_SPEED;

		if (SCREEN_HEIGHT <= y)
		{
			dead = true;
		}
}