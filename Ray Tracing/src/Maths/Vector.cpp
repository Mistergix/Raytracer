#include "Vector.h"

float Vector::getX()
{
	return m_x;
}

float Vector::getY()
{
	return m_y;
}

Vector::Vector(float x, float y)
{
	m_x = x;
	m_y = y;
}

Vector::Vector()
{
	m_x = m_y = 0.0f;
}
