#pragma once

class Sprite
{
	bool mirror_horizontal;
	bool mirror_vertical;
	//Does the animation go back and forth (like ping-pong)?
	bool ping_pong;
	bool is_texture_rect_updated;

	float animation_speed;
	float current_frame;

	//This 2 will have different values if the animation is the ping-pong type.
	unsigned short total_frames;
	unsigned short true_total_frames;

	std::string name;

	sf::Rect<unsigned short> crop_box;

	sf::Sprite sprite;

	const SpriteManager* sprite_manager;
public:
	Sprite(const std::string& i_sprite_name, const SpriteManager& i_sprite_manager, const bool& i_mirror_horizontal = 0, const bool& i_mirror_vertical = 0, const bool& i_ping_pong = 0, const float& i_animation_speed = 1, const float& i_current_frame = 0);
	
	bool animate(const char& i_animation_end = 0);
	bool get_mirror_horizontal() const;
	bool get_mirror_vertical() const;
	bool get_ping_pong() const;

	float get_animation_speed() const;
	float get_current_frame() const;
	float get_rotation() const;

	unsigned short get_frame(const float& i_frame) const;
	unsigned short get_total_frames() const;
	unsigned short get_true_total_frames() const;

	void draw(sf::RenderWindow& i_window);
	void set_animation_speed(const float& i_animation_speed);
	void set_color(const sf::Color& i_color);
	void set_crop_box(const unsigned short& i_height, const unsigned short& i_width, const unsigned short& i_x, const unsigned short& i_y);
	void set_crop_coords(const unsigned short& i_x_0, const unsigned short& i_x_1, const unsigned short& i_y_0, const unsigned short& i_y_1);
	void set_crop_margin(const unsigned short& i_bottom, const unsigned short& i_left, const unsigned short& i_right, const unsigned short& i_top);
	void set_current_frame(const float& i_current_frame);
	void set_mirror_horizontal(const bool& i_mirror_horizontal);
	void set_mirror_vertical(const bool& i_mirror_vertical);
	void set_position(const short& i_x, const short& i_y);
	void set_rotation(const float& i_angle);
	void set_scale(const float& i_factor_x, const float& i_factor_y);

	std::string get_name() const;

	sf::Color get_color() const;

	sf::Rect<unsigned short> get_crop_box() const;

	sf::Vector2f get_scale() const;
	sf::Vector2<short> get_position() const;
};