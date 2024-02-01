#include "Wall.h"

void Wall::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Wall::draw()
{
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(20.0f);
	glBegin(GL_POINTS);
		glVertex3f(x, y, 0.0);
	glEnd();
	glPopMatrix();
}
