#pragma once
#include <cmath>
#include "entities.h"

class Map
{
	int width;
	int height;

public:
	Map(int width_size, int height_size) : width(width_size), height(height_size) {}

	bool isValidPosition(Position pos)
	{
		return pos.getX() >= 1 && pos.getX() <= width && pos.getY() >= 1 && pos.getY() <= height;
	}

	int getDistance(Position pos1, Position pos2)
	{
		return std::sqrt(std::pow( (pos1.getX() - pos2.getX()), 2) + std::pow( (pos1.getY() - pos2.getY()), 2) );
	}

	int getDistance(int x1, int y1, int x2, int y2)
	{
		Position pos1(x1, y1);
		Position pos2(x2, y2);
		
		return getDistance(pos1, pos2);
	}

	int getDistance(Entity ent1, Entity ent2)
	{
		return getDistance(ent1.getPos(), ent2.getPos());
	}

	int getDistance(Entity* ent1, Entity* ent2)
	{
		return getDistance(ent1->getPos(), ent2->getPos());
	}

	int getWidth() { return width; }
	int getHeight() { return height; }
};