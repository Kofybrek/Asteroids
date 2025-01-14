#include <chrono>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/MathFunctions.hpp"
#include "Headers/Global.hpp"

//SFML 3 has the Angle class so I won't be needing this anymore.
//I still have to switch to it though.

float deg_difference(const float& i_angle_0, const float& i_angle_1)
{
	return 180 - abs(abs(get_degrees(i_angle_0) - get_degrees(i_angle_1)) - 180);
}

float deg_to_rad(const float& i_degrees)
{
	return gbl::PI * get_degrees(i_degrees) / 180;
}

float get_degrees(const float& i_degrees)
{
	return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
}

float get_distance(const float& i_finish_x, const float& i_finish_y, const float& i_start_x, const float& i_start_y)
{
	return sqrt(pow(i_finish_x - i_start_x, 2) + pow(i_finish_y - i_start_y, 2));
}

float get_radians(const float& i_radians)
{
	return static_cast<float>(fmod(2 * gbl::PI + fmod(i_radians, 2 * gbl::PI), 2 * gbl::PI));
}

float rad_difference(const float& i_angle_0, const float& i_angle_1)
{
	return gbl::PI - abs(abs(get_radians(i_angle_0) - get_radians(i_angle_1)) - gbl::PI);
}

float rad_to_deg(const float& i_radians)
{
	return 180 * get_radians(i_radians) / gbl::PI;
}