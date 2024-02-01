#pragma once

#include <windows.h>
#include <gl/GL.h>

class Enemy
{
public:
	Enemy();
	~Enemy() {}
public:
	float x, y, z;
public:
	void set(float x, float y, float z);
	void draw();
};

