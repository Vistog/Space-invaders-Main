#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/Ufo.hpp"

Ufo::Ufo(std::mt19937_64& i_random_engine, std::vector<Powerup>& main_powerups) :
	y(BASE_SIZE),
	powerup_distribution(1, POWERUP_TYPES),
	timer_distribution(UFO_TIMER_MIN, UFO_TIMER_MAX),
	animation(UFO_ANIMATION_SPEED, 2 * BASE_SIZE, "Resources/Images/Ufo.png"),
	explosion(EXPLOSION_ANIMATION_SPEED, 2 * BASE_SIZE, "Resources/Images/ExplosionBig.png"),
	powerups(main_powerups),
	random_engine(i_random_engine)
{
	reset(1);

	for (unsigned char a = 0; a < POWERUP_TYPES; a++)
	{
		powerup_animations.push_back(Animation(POWERUP_ANIMATION_SPEED, BASE_SIZE, "Resources/Images/Powerup" + std::to_string(static_cast<unsigned short>(a)) + ".png"));
	}
}

bool Ufo::check_bullet_collision(const sf::IntRect& i_bullet_hitbox)
{
	if (dead == false)
	{
		if (get_hitbox().intersects(i_bullet_hitbox) == true)
		{
			dead = 1;

			explosion_x = x;

			powerups.push_back(Powerup(x + 0.5f * BASE_SIZE, y, static_cast<Power_type>(powerup_distribution(random_engine))));

			return true;
		}
	}
	
	return 0;
}

Power_type Ufo::check_powerup_collision(const sf::IntRect& i_player_hitbox)
{
	for (Powerup& powerup : powerups)
	{
		if (powerup.dead == false && powerup.get_hitbox().intersects(i_player_hitbox) == true)
		{
			powerup.dead = true;
			return powerup.type;
		}
	}
	
	return Power_type::Nothing;
}

void Ufo::draw(sf::RenderWindow& i_window)
{
	if (0 == dead)
	{
		animation.draw(x, y, i_window);
	}

	if (0 == dead_animation_over)
	{
		explosion.draw(explosion_x, y - 0.5f * BASE_SIZE, i_window, sf::Color(255, 36, 0));
	}

	for (Powerup& powerup : powerups)
	{
		powerup_animations[static_cast<int>(powerup.type) - 1].draw(powerup.x, powerup.y, i_window);
	}
}

void Ufo::reset(bool i_dead)
{
	dead = i_dead;
	dead_animation_over = 0;

	explosion_x = SCREEN_WIDTH;
	x = SCREEN_WIDTH;

	timer = timer_distribution(random_engine);

	powerups.clear();

	animation.reset();
	explosion.reset();
}

void Ufo::tick()
{
	if (dead == false)
	{
		x -= UFO_MOVE_SPEED;

		//As soon as the UFO leaves the screen, it gets destroyed. But no powerups will appear.
		if (x <= -2 * BASE_SIZE)
		{
			dead = true;
		}

		animation.tick();
	}
	else
	{
		if (dead_animation_over == false)
		{
			dead_animation_over = explosion.tick();
		}

		if (timer == 0)
		{
			reset(0);
		}
		else
		{
			timer--;
		}
	}

	for (Animation& powerup_animation : powerup_animations)
	{
		powerup_animation.tick();
	}

	powerups.erase(remove_if(powerups.begin(), powerups.end(), [](const Powerup& i_powerup)
	{
		return 1 == i_powerup.dead;
	}), powerups.end());
}

sf::IntRect Ufo::get_hitbox() const
{
	return sf::IntRect(x, y, 2 * BASE_SIZE, BASE_SIZE);
}