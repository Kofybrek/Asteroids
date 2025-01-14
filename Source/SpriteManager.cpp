#include <chrono>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/SpriteManager.hpp"

SpriteManager::SpriteManager()
{
	unsigned short current_texture = 0;
	unsigned short total_textures = 0;

	for (const std::filesystem::directory_entry file : std::filesystem::directory_iterator("Resources/Images/"))
	{
		if (".sprites" == file.path().extension())
		{
			total_textures++;
		}
	}

	textures.resize(total_textures);

	for (const std::filesystem::directory_entry file : std::filesystem::directory_iterator("Resources/Images/"))
	{
		if (".sprites" == file.path().extension())
		{
			load_texture(current_texture, file.path().stem().string());

			current_texture++;
		}
	}
}

const sf::Texture* SpriteManager::get_sprite_texture(const std::string& i_sprite_name) const
{
	return sprites_data.at(i_sprite_name).texture;
}

unsigned short SpriteManager::get_sprite_total_frames(const std::string& i_sprite_name) const
{
	return sprites_data.at(i_sprite_name).total_frames;
}

void SpriteManager::load_texture(const unsigned short& i_texture_id, const std::string& i_file_name)
{
	std::ifstream data_file("Resources/Images/" + i_file_name + ".sprites");

	std::string line;
	std::string name;

	textures[i_texture_id].loadFromFile("Resources/Images/" + i_file_name + ".png");

	while (std::getline(data_file, line))
	{
		if (0 == line.size())
		{
			continue;
		}

		std::string key = line.substr(0, line.find(':'));
		std::string value = line.substr(2 + line.find(':'));

		if ("Name" == key)
		{
			name = value;

			sprites_data[name].texture = &textures[i_texture_id];
		}
		else if ("Origin x" == key)
		{
			sprites_data.at(name).origin.x = std::stoi(value);
		}
		else if ("Origin y" == key)
		{
			sprites_data.at(name).origin.y = std::stoi(value);
		}
		else if ("Sprite height" == key)
		{
			sprites_data.at(name).texture_box.height = std::stoi(value);
		}
		else if ("Sprite width" == key)
		{
			sprites_data.at(name).texture_box.width = std::stoi(value);
		}
		else if ("Texture x" == key)
		{
			sprites_data.at(name).texture_box.left = std::stoi(value);
		}
		else if ("Texture y" == key)
		{
			sprites_data.at(name).texture_box.top = std::stoi(value);
		}
		else if ("Total frames" == key)
		{
			sprites_data.at(name).total_frames = std::stoi(value);
		}
	}

	data_file.close();
}

sf::Rect<unsigned short> SpriteManager::get_sprite_texture_box(const std::string& i_sprite_name) const
{
	return sprites_data.at(i_sprite_name).texture_box;
}

sf::Vector2<short> SpriteManager::get_sprite_origin(const std::string& i_sprite_name) const
{
	return sprites_data.at(i_sprite_name).origin;
}