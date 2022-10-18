#pragma once


class Utility
{
public:
	static int signum(int n)
	{
		if (n > 0)
			return 1;

		else if (n < 0)
			return -1;

		return 0;
	}
	static int limit(int min, int max, int n)
	{
		if (n < min)
			return min;

		else if (n > max)
			return max;

		return n;
	}
};

class Keyboard
{
public:
	static const int ENTER_KEY = 13;
	static const int ESC_KEY = 27;

	static int handle_keyboard()
	{
		while (true)
		{
			if (_kbhit())
			{
				int key = _getch();

				if (key == ENTER_KEY || key == ESC_KEY)
					return key;
			}
		}
	}
};