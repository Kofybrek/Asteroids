#pragma once

class Asteroid
{
	bool dead;

	float speed;
	float sprite_rotation;
	float sprite_rotation_speed;
	float x;
	float y;

	//When an asteroid is hit by a bullet, it's gonna flash before disappearing
	unsigned char hit_timer;
	//0 - Large
	//1 - Medium
	//2 - Small
	unsigned char size;

	unsigned short direction;

	Sprite sprite;
public:
	Asteroid(const float& i_x, const float& i_y, const unsigned char& i_size, const unsigned short& i_direction, RandomManager& i_random_manager, const SpriteManager& i_sprite_manager);

	bool is_dead() const;

	float get_radius() const;
	float get_x() const;
	float get_y() const;

	unsigned char get_size() const;

	void draw(sf::RenderWindow& i_window);
	void hit();
	void update();
};