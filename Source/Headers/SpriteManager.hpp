#pragma once

class SpriteManager
{
	std::unordered_map<std::string, gbl::SpriteData> sprites_data;

	std::vector<sf::Texture> textures;
public:
	SpriteManager();

	const sf::Texture* get_sprite_texture(const std::string& i_sprite_name) const;

	unsigned short get_sprite_total_frames(const std::string& i_sprite_name) const;

	void load_texture(const unsigned short& i_texture_id, const std::string& i_file_name);

	sf::Rect<unsigned short> get_sprite_texture_box(const std::string& i_sprite_name) const;

	sf::Vector2<short> get_sprite_origin(const std::string& i_sprite_name) const;
};