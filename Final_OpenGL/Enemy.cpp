#include "Enemy.h"

Enemy::Enemy()
{
	x = 30.f;
	y = 10.f;
	z = 0.0f;
}

void Enemy::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Enemy::draw()
{
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(20.0f);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(x + 1, y + 1, 0.0);
	glVertex3f(x + 1, y - 1, 0.0);
	glVertex3f(x - 1, y - 1, 0.0);
	glVertex3f(x - 1, y + 1, 0.0);
	glEnd();
	glPopMatrix();
}
