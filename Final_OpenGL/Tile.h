#pragma once
#include <iostream>
#include <vector>

class Tile;
class TileNode
{
public:
	std::vector<Tile*> Edge;
};

class Tile
{
public:
	Tile() { To = new TileNode; }
	~Tile() { delete To; }
public:
	Tile* Parent;
	TileNode* To; 
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float f = 0.0f, g = 0.0f, h = 0.0f;
	int id = 0;

	bool InOpen = false;
	bool InClosed = false;
	bool Block = false;
public:
	void print() { std::cout << "x " << x << " y " << y << " z " << z << std::endl; }
	void reset();
public:
	Tile& operator=(const Tile& t) {}

	friend bool operator == (const Tile& lhs, const Tile& rhs)
	{
		if (lhs.x != rhs.x)
		{
			return false;
		}
		if (lhs.y != rhs.y)
		{
			return false;
		}
		if (lhs.z != rhs.z)
		{
			return false;
		}
		return true;
	}

	bool operator != (const Tile& lhs)
	{
		return !(*this == lhs);
	}
};
