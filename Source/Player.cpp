#include <array>
#include <chrono>
#include <random>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/MathFunctions.hpp"
#include "Headers/RandomManager.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/Sprite.hpp"
#include "Headers/Asteroid.hpp"
#include "Headers/Player.hpp"

//Normally, I put each class in separate files. But I got lazy.

Bullet::Bullet(const float& i_x, const float& i_y, const unsigned short& i_direction, const SpriteManager& i_sprite_manager) :
	death_timer(gbl::PLAYER::BULLET_DURATION),
	direction(i_direction),
	sprite("BULLET", i_sprite_manager)
{
	x.fill(i_x);
	y.fill(i_y);
}

bool Bullet::is_dead() const
{
	return 0 == death_timer;
}

float Bullet::get_x() const
{
	return x[0];
}

float Bullet::get_y() const
{
	return y[0];
}

void Bullet::die()
{
	death_timer = 0;
}

void Bullet::draw(sf::RenderWindow& i_window)
{
	for (char a = gbl::PLAYER::BULLET_LENGTH - 1; 0 <= a; a--)
	{
		sprite.set_current_frame(a);
		sprite.set_position(round(x[a]), round(y[a]));
		sprite.draw(i_window);
	}
}

void Bullet::update()
{
	for (unsigned char a = gbl::PLAYER::BULLET_LENGTH - 1; 0 < a; a--)
	{
		x[a] = x[a - 1];
		y[a] = y[a - 1];
	}

	//In case you don't want the wrapping for bullets.
	//x += gbl::PLAYER::BULLET_SPEED * cos(deg_to_rad(direction));
	//y += gbl::PLAYER::BULLET_SPEED * sin(deg_to_rad(direction));

	//We're using fmod() to add wrapping (when an object goes outside the screen, it appears at the opposite end).
	x[0] = fmod(gbl::SCREEN::WIDTH + x[0] + gbl::PLAYER::BULLET_SPEED * cos(deg_to_rad(direction)), gbl::SCREEN::WIDTH);
	y[0] = fmod(gbl::SCREEN::HEIGHT + y[0] + gbl::PLAYER::BULLET_SPEED * sin(deg_to_rad(direction)), gbl::SCREEN::HEIGHT);

	if (0 < death_timer)
	{
		death_timer--;
	}
}

Player::Player(const SpriteManager& i_sprite_manager) :
	dead(0),
	death_animation_over(0),
	moving(0),
	speed(0),
	x(0.5f * gbl::SCREEN::WIDTH),
	y(0.5f * gbl::SCREEN::HEIGHT),
	reload_timer(0),
	direction(0),
	death_sprite("PLAYER_DEATH", i_sprite_manager, 0, 0, 0, gbl::PLAYER::DEATH_SPEED),
	idle_sprite("PLAYER_IDLE", i_sprite_manager),
	move_sprite("PLAYER_MOVE", i_sprite_manager, 0, 0, 0, gbl::PLAYER::MOVE_ANIMATION_SPEED),
	sprite_manager(&i_sprite_manager)
{
	
}

bool Player::is_death_over() const
{
	return death_animation_over;
}

void Player::check_collisions(std::vector<Asteroid>& i_asteroids)
{
	for (Asteroid& asteroid : i_asteroids)
	{
		if (1 == asteroid.is_dead())
		{
			continue;
		}

		for (Bullet& bullet : bullets)
		{
			if (1 == bullet.is_dead())
			{
				continue;
			}

			for (char a = -1; a < 2; a++)
			{
				for (char b = -1; b < 2; b++)
				{
					if (2 + asteroid.get_radius() > get_distance(bullet.get_x(), bullet.get_y(), asteroid.get_x() + a * gbl::SCREEN::WIDTH, asteroid.get_y() + b * gbl::SCREEN::HEIGHT))
					{
						asteroid.hit();
						bullet.die();
					}
				}
			}
		}

		if (1 == dead)
		{
			continue;
		}

		for (char a = -1; a < 2; a++)
		{
			for (char b = -1; b < 2; b++)
			{
				if (8 + asteroid.get_radius() > get_distance(x, y, asteroid.get_x() + a * gbl::SCREEN::WIDTH, asteroid.get_y() + b * gbl::SCREEN::HEIGHT))
				{
					dead = 1;
				}
			}
		}
	}
}

void Player::draw(sf::RenderWindow& i_window)
{
	for (Bullet& bullet : bullets)
	{
		bullet.draw(i_window);
	}

	for (char a = -1; a < 2; a++)
	{
		for (char b = -1; b < 2; b++)
		{
			if (0 == dead)
			{
				if (0 == moving)
				{
					idle_sprite.set_position(round(x) + a * gbl::SCREEN::WIDTH, round(y) + b * gbl::SCREEN::HEIGHT);
					idle_sprite.set_rotation(direction);
					idle_sprite.draw(i_window);
				}
				else
				{
					move_sprite.set_position(round(x) + a * gbl::SCREEN::WIDTH, round(y) + b * gbl::SCREEN::HEIGHT);
					move_sprite.set_rotation(direction);
					move_sprite.draw(i_window);
				}
			}
			else
			{
				death_sprite.set_position(round(x) + a * gbl::SCREEN::WIDTH, round(y) + b * gbl::SCREEN::HEIGHT);
				death_sprite.draw(i_window);
			}
		}
	}
}

void Player::reset()
{
	dead = 0;
	death_animation_over = 0;
	moving = 0;

	speed = 0;
	x = 0.5f * gbl::SCREEN::WIDTH;
	y = 0.5f * gbl::SCREEN::HEIGHT;

	reload_timer = 0;

	direction = 0;

	bullets.clear();
}

void Player::update()
{
	for (Bullet& bullet : bullets)
	{
		bullet.update();
	}

	for (unsigned char a = 0; a < bullets.size(); a++)
	{
		if (1 == bullets[a].is_dead())
		{
			bullets.erase(a + bullets.begin());

			a--;
		}
	}

	if (0 == dead)
	{
		moving = 0;

		move_sprite.animate();

		if (0 < reload_timer)
		{
			reload_timer--;
		}

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			moving = 1;

			direction = get_degrees(direction - gbl::PLAYER::ROTATION_SPEED);
		}

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			moving = 1;

			direction = get_degrees(gbl::PLAYER::ROTATION_SPEED + direction);
		}

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			moving = 1;

			speed = std::min(gbl::PLAYER::MAX_SPEED, gbl::PLAYER::ACCELERATION + speed);
		}
		else
		{
			speed = std::max(0.f, speed - gbl::PLAYER::ACCELERATION);
		}

		if (0 == reload_timer && 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			reload_timer = gbl::PLAYER::RELOAD_DURATION;

			bullets.push_back(Bullet(x, y, direction, *sprite_manager));
		}

		//We're using fmod() to add wrapping (when an object goes outside the screen, it appears at the opposite end).
		x = fmod(gbl::SCREEN::WIDTH + x + speed * cos(deg_to_rad(direction)), gbl::SCREEN::WIDTH);
		y = fmod(gbl::SCREEN::HEIGHT + y + speed * sin(deg_to_rad(direction)), gbl::SCREEN::HEIGHT);
	}
	else if (0 == death_animation_over)
	{
		death_animation_over = death_sprite.animate();
	}
}