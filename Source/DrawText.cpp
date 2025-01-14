#include <chrono>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/Sprite.hpp"

void draw_text(const bool& i_center_x, const bool& i_center_y, const short& i_x, const short& i_y, const std::string& i_font_sprite_name, const std::string& i_text, const SpriteManager& i_sprite_manager, sf::RenderWindow& i_window, const sf::Color& i_color)
{
	short character_x;
	short character_y;

	unsigned short character_height;
	unsigned short character_width;

	Sprite font_sprite(i_font_sprite_name, i_sprite_manager);
	font_sprite.set_color(i_color);

	character_height = font_sprite.get_crop_box().height;
	character_width = font_sprite.get_crop_box().width;

	if (1 == i_center_x)
	{
		character_x = static_cast<short>(round(i_x - 0.5f * character_width * i_text.substr(0, i_text.find_first_of('\n')).size()));
	}
	else
	{
		character_x = i_x;
	}

	if (1 == i_center_y)
	{
		character_y = static_cast<short>(round(i_y - 0.5f * character_height * (1 + std::count(i_text.begin(), i_text.end(), '\n'))));
	}
	else
	{
		character_y = i_y;
	}

	for (unsigned short a = 0; a < i_text.size(); a++)
	{
		if ('\n' == i_text[a])
		{
			character_y += character_height;

			if (1 == i_center_x)
			{
				character_x = static_cast<short>(round(i_x - 0.5f * character_width * i_text.substr(1 + a, i_text.find_first_of('\n', 1 + a) - 1 - a).size()));
			}
			else
			{
				character_x = i_x;
			}

			continue;
		}

		//We're using ASCII so 32nd character in ASCII is the first character in our font sprite.
		font_sprite.set_current_frame(static_cast<float>(i_text[a] - 32));
		font_sprite.set_position(character_x, character_y);
		font_sprite.draw(i_window);

		character_x += character_width;
	}
}