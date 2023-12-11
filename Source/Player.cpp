#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/Enemy.hpp"
#include "Headers/Ufo.hpp"
#include "Headers/Player.hpp"

Player::Player() :
	explosion(EXPLOSION_ANIMATION_SPEED, BASE_SIZE, "Resources/Images/Explosion.png"),
	fire_number(0)
{
	reset();

	bullet_texture.loadFromFile("Resources/Images/PlayerBullet.png");
	default_spaceship_texture.loadFromFile("Resources/Images/Default_spaceship_animation.png");
	shield_spaceship_texture.loadFromFile("Resources/Images/Shield_spaceship_animation.png");
	fast_reload_spaceship_texture.loadFromFile("Resources/Images/Fast_reload_spaceship_animation.png");
	triple_bullet_spaceship_texture.loadFromFile("Resources/Images/Triple_bullet_spaceship_animation.png");
	mirror_spaceship_texture.loadFromFile("Resources/Images/Mirror_spaceship_animation.png");

	bullet_sprite.setTexture(bullet_texture);
}

bool Player::get_dead() const
{
	return dead;
}

bool Player::get_dead_animation_over() const
{
	return dead_animation_over;
}

Power_type Player::get_current_power() const
{
	return current_power;
}

unsigned short Player::get_power_timer() const
{
	return power_timer;
}

unsigned short Player::get_y() const
{
	return y;
}

void Player::die()
{
	dead = true;
}

void Player::draw(sf::RenderWindow& i_window)
{
	if (dead == false)
	{
		sprite.setPosition(x - 8, y);
		switch (current_power)
		{
		case Power_type::Nothing:
			sprite.setTexture(default_spaceship_texture);
			sprite.setTextureRect(sf::IntRect(PLAYER_SIZE * fire_number, 0, PLAYER_SIZE, PLAYER_SIZE));
			break;
		case Power_type::Shield:
			sprite.setTexture(shield_spaceship_texture);
			sprite.setTextureRect(sf::IntRect(PLAYER_SIZE * fire_number, 0, PLAYER_SIZE, PLAYER_SIZE));
			break;
		case Power_type::Fast_Reload:
			sprite.setTexture(fast_reload_spaceship_texture);
			sprite.setTextureRect(sf::IntRect(PLAYER_SIZE * fire_number, 0, PLAYER_SIZE, PLAYER_SIZE));
			break;
		case Power_type::Triple_bullets:
			sprite.setTexture(triple_bullet_spaceship_texture);
			sprite.setTextureRect(sf::IntRect(PLAYER_SIZE * fire_number, 0, PLAYER_SIZE , PLAYER_SIZE));
			break;
		case Power_type::Mirror:
			sprite.setTexture(mirror_spaceship_texture);
			sprite.setTextureRect(sf::IntRect(PLAYER_SIZE * fire_number, 0, PLAYER_SIZE, PLAYER_SIZE));
			break;
		default:
			throw "Unexpected POwerUp type";
			break;
		}

		fire_number ++;
		if (fire_number == 4) {  fire_number = 0;  }

		for (const Bullet& bullet : bullets)
		{
			bullet_sprite.setPosition(bullet.x, bullet.y);

			i_window.draw(bullet_sprite);
		}

		i_window.draw(sprite);



		if (shield_animation_over == false)
		{
			//Once we get hit while having a shield, the shield will be destroyed. We'll show a blue explosion.
			explosion.draw(x, y, i_window, sf::Color(0, 109, 255));
		}
	}
	else if (dead_animation_over == false)
	{
		explosion.draw(x, y, i_window, sf::Color(255, 36, 0));
	}
}

void Player::reset()
{
	dead = 0;
	dead_animation_over = false;
	shield_animation_over = true;
	rotate_animation_over = true;
	last_move = Move_direction::Stable;
	current_move = Move_direction::Stable;

	current_power = Power_type::Nothing;
	reload_timer = 0;

	power_timer = 0;
	x = 0.5f * (SCREEN_WIDTH - BASE_SIZE);
	y = SCREEN_HEIGHT - 2 * BASE_SIZE;

	bullets.clear();

	explosion.reset();
}

void Player::tick(std::mt19937_64& i_random_engine, std::vector<Bullet>& i_enemy_bullets, std::vector<Enemy>& i_enemies, Ufo& i_ufo)
{
	if (dead == false)
	{
		Power_type powerup_type;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) == true)
		{
			if (current_power == Power_type::Mirror)
			{
				x = std::min<int>(PLAYER_MOVE_SPEED + x, SCREEN_WIDTH - PLAYER_SIZE);

				last_move = current_move;
				current_move = Move_direction::Right;
			}
			else
			{
				x = std::max<int>(x - PLAYER_MOVE_SPEED, PLAYER_SIZE);

				last_move = current_move;
				current_move = Move_direction::Left;
			}
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == true)
		{
			if (current_power == Power_type::Mirror)
			{
				x = std::max<int>(x - PLAYER_MOVE_SPEED, PLAYER_SIZE);
								
				last_move = current_move;
				current_move = Move_direction::Left;
			}
			else
			{
				x = std::min<int>(PLAYER_MOVE_SPEED + x, SCREEN_WIDTH - PLAYER_SIZE);
				last_move = current_move;
				current_move = Move_direction::Right;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			last_move = current_move;
			current_move = Move_direction::Stable;
		}

		if (0 == reload_timer)
		{
			if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			{
				if (current_power == Power_type::Fast_Reload)
				{
					reload_timer = FAST_RELOAD_DURATION;
				}
				else
				{
					reload_timer = RELOAD_DURATION;
				}

				bullets.push_back(Bullet(0, -PLAYER_BULLET_SPEED, x, y));

				if (current_power == Power_type::Triple_bullets)
				{
					bullets.push_back(Bullet(0, -PLAYER_BULLET_SPEED, x - 0.5f * BASE_SIZE, y));
					bullets.push_back(Bullet(0, -PLAYER_BULLET_SPEED, x + 0.55f * BASE_SIZE, y));
				}
			}
		}
		else
		{
			reload_timer--;
		}

		for (Bullet& enemy_bullet : i_enemy_bullets)
		{
			if (1 == get_hitbox().intersects(enemy_bullet.get_hitbox()))
			{
				if (current_power == Power_type::Shield)
				{
					current_power = Power_type::Nothing;

					shield_animation_over = false;
				}
				else
				{
					dead = true;
				}

				enemy_bullet.dead = 1;

				break;
			}
		}

		powerup_type = i_ufo.check_powerup_collision(get_hitbox());

		if (powerup_type != Power_type::Nothing)
		{
			current_power = powerup_type;

			power_timer = POWERUP_DURATION;
		}

		if (power_timer == 0)
		{
			current_power = Power_type::Nothing;
		}
		else
		{
			power_timer--;
		}

		if (shield_animation_over == false)
		{
			shield_animation_over = explosion.tick();
		}
	}
	else if (dead_animation_over == false)
	{
		dead_animation_over = explosion.tick();
	}

	for (Bullet& bullet : bullets)
	{
		bullet.tick();
		
		if (bullet.dead == false)
		{
			if (i_ufo.check_bullet_collision(bullet.get_hitbox()) == true)
			{
				bullet.dead = true;
			}
		}
	}

	for (Enemy& enemy : i_enemies)
	{
		for (Bullet& bullet : bullets)
		{
			if (0 == bullet.dead && 0 < enemy.get_health() && 1 == enemy.get_hitbox().intersects(bullet.get_hitbox()))
			{
				bullet.dead = 1;

				enemy.hit();

				break;
			}
		}
	}

	bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& i_bullet)
	{
		return 1 == i_bullet.dead;
	}), bullets.end());
}

sf::IntRect Player::get_hitbox() const
{
	return sf::IntRect(x - 0.0f * PLAYER_SIZE, y + 0.4f * PLAYER_SIZE,  0.7f * PLAYER_SIZE, 0.6f * PLAYER_SIZE);
}