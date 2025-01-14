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

Asteroid::Asteroid(const float& i_x, const float& i_y, const unsigned char& i_size, const unsigned short& i_direction, RandomManager& i_random_manager, const SpriteManager& i_sprite_manager) :
	dead(0),
	sprite_rotation(i_random_manager.get_random(359, 0)),
	x(i_x),
	y(i_y),
	hit_timer(1 + gbl::ASTEROID::HIT_DURATION),
	size(i_size),
	direction(i_direction),
	sprite("ASTEROID_" + std::to_string(static_cast<unsigned short>(i_size)), i_sprite_manager)
{
	switch (size)
	{
		case 0:
		{
			speed = gbl::ASTEROID::SPEED_0;

			break;
		}
		case 1:
		{
			speed = gbl::ASTEROID::SPEED_1;

			break;
		}
		case 2:
		{
			speed = gbl::ASTEROID::SPEED_2;
		}
	}

	sprite_rotation_speed = speed * i_random_manager.get_random(4.f, 0.f);
}

bool Asteroid::is_dead() const
{
	return dead;
}

float Asteroid::get_radius() const
{
	return pow(2, 5 - size);
}

float Asteroid::get_x() const
{
	return x;
}

float Asteroid::get_y() const
{
	return y;
}

unsigned char Asteroid::get_size() const
{
	return size;
}

void Asteroid::draw(sf::RenderWindow& i_window)
{
	sprite.set_rotation(sprite_rotation);

	for (char a = -1; a < 2; a++)
	{
		for (char b = -1; b < 2; b++)
		{
			//We could use shaders for this.
			//But this is simpler.
			sprite.set_current_frame(hit_timer <= gbl::ASTEROID::HIT_DURATION);
			sprite.set_position(round(x) + a * gbl::SCREEN::WIDTH, round(y) + b * gbl::SCREEN::HEIGHT);
			sprite.draw(i_window);
		}
	}
}

void Asteroid::hit()
{
	if (hit_timer == 1 + gbl::ASTEROID::HIT_DURATION)
	{
		hit_timer--;
	}
}

void Asteroid::update()
{
	if (hit_timer <= gbl::ASTEROID::HIT_DURATION)
	{
		if (0 < hit_timer)
		{
			hit_timer--;
		}
		else
		{
			dead = 1;
		}
	}

	sprite_rotation += sprite_rotation_speed;
	//We're using fmod() to add wrapping (when an object goes outside the screen, it appears at the opposite end).
	x = fmod(gbl::SCREEN::WIDTH + x + speed * cos(deg_to_rad(direction)), gbl::SCREEN::WIDTH);
	y = fmod(gbl::SCREEN::HEIGHT + y + speed * sin(deg_to_rad(direction)), gbl::SCREEN::HEIGHT);
}