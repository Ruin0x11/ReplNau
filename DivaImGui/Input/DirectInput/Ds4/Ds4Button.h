#pragma once

namespace ReplNau::Input
{
	enum Direction
	{
		DIR_UP, 
		DIR_RIGHT, 
		DIR_DOWN, 
		DIR_LEFT
	};

	enum Ds4Button : int
	{
		DS4_SQUARE = 0,
		DS4_CROSS = 1,
		DS4_CIRCLE = 2,
		DS4_TRIANGLE = 3,
		
		DS4_L1 = 4,
		DS4_R1 = 5,

		DS4_L_TRIGGER = 6,
		DS4_R_TRIGGER = 7,

		DS4_SHARE = 8,
		DS4_OPTIONS = 9,

		DS4_L3 = 10,
		DS4_R3 = 11,

		DS4_PS = 12,
		DS4_TOUCH = 13,
	
		DS4_DPAD_UP,
		DS4_DPAD_RIGHT,
		DS4_DPAD_DOWN,
		DS4_DPAD_LEFT,

		DS4_L_STICK_UP,
		DS4_L_STICK_RIGHT,
		DS4_L_STICK_DOWN,
		DS4_L_STICK_LEFT,

		DS4_R_STICK_UP,
		DS4_R_STICK_RIGHT,
		DS4_R_STICK_DOWN,
		DS4_R_STICK_LEFT,

		DS4_BUTTON_MAX,
	};
}