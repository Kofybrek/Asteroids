#include <chrono>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/Sprite.hpp"

Sprite::Sprite(const std::string& i_sprite_name, const SpriteManager& i_sprite_manager, const bool& i_mirror_horizontal, const bool& i_mirror_vertical, const bool& i_ping_pong, const float& i_animation_speed, const float& i_current_frame) :
	ping_pong(i_ping_pong),
	is_texture_rect_updated(1),
	true_total_frames(i_sprite_manager.get_sprite_total_frames(i_sprite_name)),
	name(i_sprite_name),
	sprite_manager(&i_sprite_manager)
{
	if (0 == ping_pong)
	{
		total_frames = true_total_frames;
	}
	else
	{
		//For ping-pong animation, we're gonna assume that our sprite has twice as many frames.
		total_frames = 2 * (true_total_frames - 1);
	}

	set_animation_speed(i_animation_speed);
	set_crop_margin(0, 0, 0, 0);
	set_current_frame(i_current_frame);
	set_mirror_horizontal(i_mirror_horizontal);
	set_mirror_vertical(i_mirror_vertical);

	sprite.setOrigin(i_sprite_manager.get_sprite_origin(name).x, i_sprite_manager.get_sprite_origin(name).y);
	sprite.setTexture(*i_sprite_manager.get_sprite_texture(name));
}

bool Sprite::animate(const char& i_animation_end)
{
	//This is kinda hard to explain.
	//When the animation is going backwards (0 > animation_speed) we're gonna check to see if the animation is over on the left side (first frame).
	bool left_end = 0;
	//When the animation is going forward (0 < animation_speed) we're gonna check to see if the animation is over on the right side (last frame).
	bool right_end = 0;

	if (0 == ping_pong)
	{
		left_end = 0 > animation_speed + current_frame;
		right_end = total_frames <= animation_speed + current_frame;
	}
	else
	{
		left_end = 0 > animation_speed + current_frame || total_frames <= animation_speed + current_frame;
		right_end = current_frame < true_total_frames && true_total_frames <= animation_speed + current_frame;

		if (0 == right_end)
		{
			right_end = current_frame >= true_total_frames && true_total_frames > animation_speed + current_frame;
		}
	}

	set_current_frame(animation_speed + current_frame);

	switch (i_animation_end)
	{
		case -1:
		{
			return left_end;
		}
		case 0:
		{
			return left_end || right_end;
		}
		case 1:
		{
			return right_end;
		}
	}

	return 0;
}

bool Sprite::get_mirror_horizontal() const
{
	return mirror_horizontal;
}

bool Sprite::get_mirror_vertical() const
{
	return mirror_vertical;
}

bool Sprite::get_ping_pong() const
{
	return ping_pong;
}

float Sprite::get_animation_speed() const
{
	return animation_speed;
}

float Sprite::get_current_frame() const
{
	return current_frame;
}

float Sprite::get_rotation() const
{
	return sprite.getRotation();
}

unsigned short Sprite::get_frame(const float& i_frame) const
{
	unsigned short frame = static_cast<unsigned short>(floor(i_frame));

	if (1 == ping_pong && frame >= true_total_frames)
	{
		return 2 * (true_total_frames - 1) - frame;
	}
	
	return frame;
}

unsigned short Sprite::get_total_frames() const
{
	return total_frames;
}

unsigned short Sprite::get_true_total_frames() const
{
	return true_total_frames;
}

void Sprite::draw(sf::RenderWindow& i_window)
{
	if (1 == is_texture_rect_updated)
	{
		sf::Rect<short> texture_box(sprite_manager->get_sprite_texture_box(name));
		
		is_texture_rect_updated = 0;

		crop_box.left = std::clamp<unsigned short>(crop_box.left, 0, sprite_manager->get_sprite_texture_box(name).width - 1);
		crop_box.top = std::clamp<unsigned short>(crop_box.top, 0, sprite_manager->get_sprite_texture_box(name).height - 1);
		crop_box.height = std::clamp<unsigned short>(crop_box.height, 0, sprite_manager->get_sprite_texture_box(name).height - crop_box.top);
		crop_box.width = std::clamp<unsigned short>(crop_box.width, 0, sprite_manager->get_sprite_texture_box(name).width - crop_box.left);

		texture_box.left += texture_box.width * (mirror_horizontal + get_frame(current_frame));
		texture_box.top += mirror_vertical * texture_box.height;

		if (0 == mirror_horizontal)
		{
			texture_box.left += crop_box.left;
			texture_box.width = crop_box.width;
		}
		else
		{
			texture_box.left -= crop_box.left;
			texture_box.width = -crop_box.width;
		}

		if (0 == mirror_vertical)
		{
			texture_box.height = crop_box.height;
			texture_box.top += crop_box.top;
		}
		else
		{
			texture_box.height = -crop_box.height;
			texture_box.top -= crop_box.top;
		}

		sprite.setTextureRect(sf::IntRect(texture_box));
	}
	
	i_window.draw(sprite);
}

void Sprite::set_animation_speed(const float& i_animation_speed)
{
	animation_speed = static_cast<float>(std::fmod(i_animation_speed, total_frames));
}

void Sprite::set_color(const sf::Color& i_color)
{
	sprite.setColor(i_color);
}

void Sprite::set_crop_box(const unsigned short& i_height, const unsigned short& i_width, const unsigned short& i_x, const unsigned short& i_y)
{
	sf::Rect<unsigned short> previous_crop_box = crop_box;

	crop_box.height = i_height;
	crop_box.left = i_x;
	crop_box.top = i_y;
	crop_box.width = i_width;

	if (crop_box != previous_crop_box)
	{
		is_texture_rect_updated = 1;
	}
}

void Sprite::set_crop_coords(const unsigned short& i_x_0, const unsigned short& i_x_1, const unsigned short& i_y_0, const unsigned short& i_y_1)
{
	sf::Rect<unsigned short> previous_crop_box = crop_box;

	crop_box.height = i_y_1 - i_y_0;
	crop_box.left = i_x_0;
	crop_box.top = i_y_0;
	crop_box.width = i_x_1 - i_x_0;

	if (crop_box != previous_crop_box)
	{
		is_texture_rect_updated = 1;
	}
}

void Sprite::set_crop_margin(const unsigned short& i_bottom, const unsigned short& i_left, const unsigned short& i_right, const unsigned short& i_top)
{
	sf::Rect<unsigned short> previous_crop_box = crop_box;

	crop_box.height = sprite_manager->get_sprite_texture_box(name).height - i_bottom - i_top;
	crop_box.left = i_left;
	crop_box.top = i_top;
	crop_box.width = sprite_manager->get_sprite_texture_box(name).width - i_left - i_right;

	if (crop_box != previous_crop_box)
	{
		is_texture_rect_updated = 1;
	}
}

void Sprite::set_current_frame(const float& i_current_frame)
{
	float previous_frame = current_frame;

	current_frame = static_cast<float>(fmod(total_frames + fmod(i_current_frame, total_frames), total_frames));

	if (get_frame(current_frame) != get_frame(previous_frame))
	{
		is_texture_rect_updated = 1;
	}
}

void Sprite::set_mirror_horizontal(const bool& i_mirror_horizontal)
{
	if (i_mirror_horizontal != mirror_horizontal)
	{
		is_texture_rect_updated = 1;
	}

	mirror_horizontal = i_mirror_horizontal;
}

void Sprite::set_mirror_vertical(const bool& i_mirror_vertical)
{
	if (i_mirror_vertical != mirror_vertical)
	{
		is_texture_rect_updated = 1;
	}

	mirror_vertical = i_mirror_vertical;
}

void Sprite::set_position(const short& i_x, const short& i_y)
{
	sprite.setPosition(i_x, i_y);
}

void Sprite::set_rotation(const float& i_angle)
{
	sprite.setRotation(i_angle);
}

void Sprite::set_scale(const float& i_factor_x, const float& i_factor_y)
{
	sprite.setScale(i_factor_x, i_factor_y);
}

std::string Sprite::get_name() const
{
	return name;
}

sf::Color Sprite::get_color() const
{
	return sprite.getColor();
}

sf::Rect<unsigned short> Sprite::get_crop_box() const
{
	return crop_box;
}

sf::Vector2f Sprite::get_scale() const
{
	return sprite.getScale();
}

sf::Vector2<short> Sprite::get_position() const
{
	return sf::Vector2<short>(sprite.getPosition());
}