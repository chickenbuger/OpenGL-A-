#include "Player.h"

Player::Player()
{
	x = 20.0f;
	y = 20.0f;
	z = 0.0f;
}

Player::Player(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Player::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Player::draw()
{
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(20.0f);
	glBegin(GL_POINTS);
		glVertex3f(x, y, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(x + 1, y + 1, 0.0);
		glVertex3f(x + 1, y - 1, 0.0);
		glVertex3f(x - 1, y - 1, 0.0);
		glVertex3f(x - 1, y + 1, 0.0);
	glEnd();
	glPopMatrix();
}
