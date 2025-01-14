#pragma once

class Bullet
{
	std::array<float, gbl::PLAYER::BULLET_LENGTH> x;
	std::array<float, gbl::PLAYER::BULLET_LENGTH> y;

	unsigned short death_timer;
	unsigned short direction;

	Sprite sprite;
public:
	Bullet(const float& i_x, const float& i_y, const unsigned short& i_direction, const SpriteManager& i_sprite_manager);

	bool is_dead() const;

	float get_x() const;
	float get_y() const;

	void die();
	void draw(sf::RenderWindow& i_window);
	void update();
};

class Player
{
	bool dead;
	bool death_animation_over;
	bool moving;

	float speed;
	float x;
	float y;

	unsigned char reload_timer;

	unsigned short direction;

	std::vector<Bullet> bullets;

	Sprite death_sprite;
	Sprite idle_sprite;
	Sprite move_sprite;

	const SpriteManager* sprite_manager;
public:
	Player(const SpriteManager& i_sprite_manager);

	bool is_death_over() const;

	void check_collisions(std::vector<Asteroid>& i_asteroids);
	void draw(sf::RenderWindow& i_window);
	void reset();
	void update();
};