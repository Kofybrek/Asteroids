#include <array>
#include <chrono>
#include <random>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/RandomManager.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/DrawText.hpp"
#include "Headers/Sprite.hpp"
#include "Headers/Asteroid.hpp"
#include "Headers/Player.hpp"
#include "Headers/Game.hpp"

Game::Game() :
	window(sf::VideoMode(gbl::SCREEN::RESIZE * gbl::SCREEN::WIDTH, gbl::SCREEN::RESIZE * gbl::SCREEN::HEIGHT), "Asteroids", sf::Style::Close),
	player(sprite_manager),
	background_sprite("BACKGROUND", sprite_manager)
{
	has_focus = window.hasFocus();

	window.setView(sf::View(sf::FloatRect(0, 0, gbl::SCREEN::WIDTH, gbl::SCREEN::HEIGHT)));

	generate_asteroids();
}

bool Game::is_open() const
{
	return window.isOpen();
}

void Game::draw()
{
	if (1 == has_focus)
	{
		window.clear();

		background_sprite.draw(window);

		if (1 == asteroids.empty())
		{
			draw_text(1, 1, 0.5f * gbl::SCREEN::WIDTH, 0.5f * gbl::SCREEN::HEIGHT, "FONT_16", "Yay! You won!\n...What? Were you expecting something\nbetter? Were you expecting some kind\nof a reward? A payment maybe?\nTHIS IS A VIDEO GAME, YOU MORON!\nThe only reward you're gonna get is\nlosing that precious time you'll never\nget back!\nYeah, that's right. You just wasted\nyour time destroying some pixels on\nthe screen.", sprite_manager, window);
		}
		else if (0 == player.is_death_over())
		{
			for (Asteroid& asteroid : asteroids)
			{
				asteroid.draw(window);
			}

			player.draw(window);
		}
		else
		{
			draw_text(1, 1, 0.5f * gbl::SCREEN::WIDTH, 0.5f * gbl::SCREEN::HEIGHT, "FONT_16", "GAME OVER!\nYou pathetic loser couldn't even\ndestroy some asteroids. No wonder\nyou have no friends. I wouldn't\nwanna be your friend either. All you\ndo is play this silly game instead\nof doing something useful. And even\nthen you're still losing. You're a\nwaste of space!", sprite_manager, window);
		}

		window.display();
	}
}

void Game::generate_asteroids()
{
	asteroids.clear();

	for (unsigned char a = 0; a < gbl::ASTEROID::INITIAL_NUMBER; a++)
	{
		unsigned short random_x;
		unsigned short random_y;

		//Making sure that the asteroids don't spawn on the safe zone so they don't kill the player immediately.
		do
		{
			random_x = random_manager.get_random(gbl::SCREEN::WIDTH - 1, 0);
			random_y = random_manager.get_random(gbl::SCREEN::HEIGHT - 1, 0);
		}
		while (random_x > 0.5f * gbl::SCREEN::WIDTH - gbl::PLAYER::SAFE_ZONE_SIZE &&
			   random_x < gbl::PLAYER::SAFE_ZONE_SIZE + 0.5f * gbl::SCREEN::WIDTH &&
			   random_y > 0.5f * gbl::SCREEN::HEIGHT - gbl::PLAYER::SAFE_ZONE_SIZE &&
			   random_y < gbl::PLAYER::SAFE_ZONE_SIZE + 0.5f * gbl::SCREEN::HEIGHT);

		asteroids.push_back(Asteroid(random_x, random_y, 0, random_manager.get_random(359, 0), random_manager, sprite_manager));
	}
}

void Game::handle_events()
{
	while (1 == window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
			{
				window.close();
			}
		}
	}
}

void Game::update()
{
	has_focus = window.hasFocus();

	if (1 == has_focus)
	{
		if (0 < asteroids.size() && 0 == player.is_death_over())
		{
			for (Asteroid& asteroid : asteroids)
			{
				asteroid.update();
			}

			player.update();
			player.check_collisions(asteroids);

			for (unsigned char a = 0; a < asteroids.size(); a++)
			{
				if (1 == asteroids[a].is_dead())
				{
					if (2 > asteroids[a].get_size())
					{
						//Adding 2 smaller asteroids when an asteroid is destroyed.
						for (unsigned char b = 0; b < 2; b++)
						{
							asteroids.push_back(Asteroid(asteroids[a].get_x(), asteroids[a].get_y(), 1 + asteroids[a].get_size(), random_manager.get_random(359, 0), random_manager, sprite_manager));
						}
					}

					asteroids.erase(a + asteroids.begin());

					a--;
				}
			}
		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			generate_asteroids();

			player.reset();
		}
	}
}