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