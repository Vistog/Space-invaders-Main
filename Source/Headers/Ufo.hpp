#pragma once

#include "Powerup.hpp"
#include "Global.hpp"

class Ufo
{
	bool dead;
	bool dead_animation_over;

	short explosion_x;
	short x;

	unsigned short timer;
	unsigned short y;

	std::uniform_int_distribution<unsigned short> powerup_distribution;
	//The UFO can appear after 12 seconds. Or 16 seconds. Or 14.652 seconds. We don't know for sure.
	std::uniform_int_distribution<unsigned short> timer_distribution;

	std::vector<Animation> powerup_animations;

	//Yes, the UFO is responsible for the powerups too.
	std::vector<Powerup>& powerups;

	Animation animation;
	Animation explosion;
	std::mt19937_64& random_engine;
public:
	Ufo(std::mt19937_64& i_random_engine, std::vector<Powerup>& main_powerups);

	bool check_bullet_collision(const sf::IntRect& i_bullet_hitbox);

	Power_type check_powerup_collision(const sf::IntRect& i_player_hitbox);

	void draw(sf::RenderWindow& i_window);
	void reset(bool i_dead);
	void tick();

	sf::IntRect get_hitbox() const;
};