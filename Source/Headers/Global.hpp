#pragma once

namespace gbl
{
	//It's actually ==== 3.141592653589793238.
	//                                    ^^^
	constexpr float PI = 3.141592653589793116f;
	//Why it's different? Because C++ stores float numbers in a weird way.

	namespace ASTEROID
	{
		//Big asteroid speed.
		constexpr float SPEED_0 = 0.25f;
		//Medium asteroid speed.
		constexpr float SPEED_1 = 0.5f;
		//Small asteroid speed.
		constexpr float SPEED_2 = 1;

		//When an asteroid is hit, it's gonna flash white before disappearing. This is the duration.
		constexpr unsigned char HIT_DURATION = 2;
		constexpr unsigned char INITIAL_NUMBER = 16;
	}

	namespace PLAYER
	{
		constexpr float ACCELERATION = 0.125f;
		//Death animation speed. For some reason, I forgot to write "animation".
		constexpr float DEATH_SPEED = 0.5f;
		constexpr float MAX_SPEED = 4;
		constexpr float MOVE_ANIMATION_SPEED = 0.5f;

		//The length of its trail (or tail).
		constexpr unsigned char BULLET_LENGTH = 4;
		constexpr unsigned char BULLET_SPEED = 4;
		constexpr unsigned char RELOAD_DURATION = 8;
		constexpr unsigned char ROTATION_SPEED = 4;
		//This is the zone where asteroids can't spawn so the player doesn't die immediately.
		constexpr unsigned char SAFE_ZONE_SIZE = 48;

		constexpr unsigned short BULLET_DURATION = 120;
	}

	namespace SCREEN
	{
		constexpr unsigned char RESIZE = 2;

		constexpr unsigned short HEIGHT = 360;
		constexpr unsigned short WIDTH = 640;

		//The game runs at 60 FPS.
		constexpr std::chrono::microseconds FRAME_DURATION(16667);
	}

	struct SpriteData
	{
		const sf::Texture* texture;

		unsigned short total_frames;

		sf::Rect<unsigned short> texture_box;

		sf::Vector2<short> origin;
	};
}