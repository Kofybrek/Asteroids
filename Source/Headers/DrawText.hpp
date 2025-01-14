#pragma once

void draw_text(const bool& i_center_x, const bool& i_center_y, const short& i_x, const short& i_y, const std::string& i_font_sprite_name, const std::string& i_text, const SpriteManager& i_sprite_manager, sf::RenderWindow& i_window, const sf::Color& i_color = sf::Color(255, 255, 255));