#pragma once

class Game
{
	bool has_focus;

	std::vector<Asteroid> asteroids;

	sf::Event event;

	sf::RenderWindow window;

	RandomManager random_manager;

	SpriteManager sprite_manager;

	Player player;

	Sprite background_sprite;
public:
	Game();

	bool is_open() const;

	void draw();
	void generate_asteroids();
	void handle_events();
	void update();
};