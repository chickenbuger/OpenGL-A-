#pragma once

#include <windows.h>
#include <gl/GL.h>

class Wall
{
public:
	Wall() { x = 0; y = 0; z = 0; }
	~Wall() {}
public:
	float x, y, z;
	float id;

public:
	void set(float x, float y, float z);
	void draw();
};

