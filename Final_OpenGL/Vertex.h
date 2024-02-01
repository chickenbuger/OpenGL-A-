#pragma once
#include <stdio.h>

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z); 
	~Vertex() {}
public:
	float x, y, z; // z°ª 0 °íÁ¤

public:
	void set(float x, float y, float z);
	
	float* get();

	void print();
};

