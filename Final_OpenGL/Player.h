#pragma once

#include <windows.h>
#include <gl/GL.h>

class Player
{
public:
	Player();
	Player(float x, float y, float z);
	~Player() {}
public:
	float x, y, z;

public:
	void set(float x, float y, float z);
	void draw();
};

