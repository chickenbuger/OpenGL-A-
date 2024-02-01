#include "Tile.h"

void Tile::reset()
{
	Parent = nullptr;
	f = 0.0f;
	g = 0.0f;
	h = 0.0f;
	InOpen = false;
	InClosed = false;
	Block = false;
}
