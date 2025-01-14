#include <array>
#include <chrono>
#include <random>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/RandomManager.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/Sprite.hpp"
#include "Headers/Asteroid.hpp"
#include "Headers/Player.hpp"
#include "Headers/Game.hpp"

//I got sick, by the way.
//I keep coughing and sneezing.
//I don't know why I'm saying this to you.
//Maybe I want you to feel sorry for me.
//Or maybe I'm just an attention seeker.

int main()
{
	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	Game game;

	game.draw();

	previous_time = std::chrono::steady_clock::now();

	while (1 == game.is_open())
	{
		//Making our game framerate-independant.
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

		lag += delta_time;

		previous_time += delta_time;

		while (gbl::SCREEN::FRAME_DURATION <= lag)
		{
			lag -= gbl::SCREEN::FRAME_DURATION;

			game.handle_events();
			game.update();

			if (gbl::SCREEN::FRAME_DURATION > lag)
			{
				game.draw();
			}
		}
	}
}