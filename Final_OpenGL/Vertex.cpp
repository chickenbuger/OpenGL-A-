#include "Vertex.h"

Vertex::Vertex()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vertex::Vertex(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vertex::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float* Vertex::get()
{
	float value[3] = { x,y,z };
	return value;
}

void Vertex::print()
{
	printf("x %5.2f, y %5.2f, z %5.2f \n", x, y, z);
}