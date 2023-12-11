#include <array>
#include <cmath>
#include "Global.hpp"
#include <SFML/Graphics.hpp>

struct Bullet
{
	bool dead;

	float real_x;
	float real_y;
	float step_x;
	float step_y;

	short x;
	short y;

	//We also use this struct in the Player class. But the player's bullets don't have a tail. So it may seem a bit redundant. But I'm too lazy to fix it.
	std::array<short, 3> previous_x;
	std::array<short, 3> previous_y;

	Bullet(float i_step_x, float i_step_y, short i_x, short i_y);

	void tick();

	sf::IntRect get_hitbox() const;
};