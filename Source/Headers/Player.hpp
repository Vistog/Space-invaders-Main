#pragma once

#include "Global.hpp"
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>

class Player
{
	bool dead;
	bool dead_animation_over;
	bool shield_animation_over;
	bool rotate_animation_over;
	Move_direction last_move;
	Move_direction current_move;

	Power_type current_power;
	unsigned char reload_timer;

	unsigned short power_timer;
	unsigned short x;
	unsigned short y;

	std::vector<Bullet> bullets;

	sf::Sprite bullet_sprite;
	sf::Sprite sprite;

	sf::Texture bullet_texture;
	sf::Texture texture;

	Animation explosion;
public:
	Player();

	bool get_dead() const;
	bool get_dead_animation_over() const;
	bool get_rotate_animation_over() const;

	Power_type get_current_power() const;

	unsigned short get_power_timer() const;
	unsigned short get_y() const;

	void die();
	void draw(sf::RenderWindow& i_window);
	void reset();
	void tick(std::mt19937_64& i_random_engine, std::vector<Bullet>& i_enemy_bullets, std::vector<Enemy>& i_enemies, Ufo& i_ufo);

	sf::IntRect get_hitbox() const;
};