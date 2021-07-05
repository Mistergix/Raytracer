#include "color.h"

#include <cmath>

Color::Color() : r(0.0f), g(0.0f), b(0.0f) {}

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

Color& Color::operator =(const Color& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	return *this;
}

Color& Color::operator +=(const Color& other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	return *this;
}

Color& Color::operator *=(const Color& other)
{
	r *= other.r;
	g *= other.g;
	b *= other.b;
	return *this;
}

Color& Color::operator *=(float f)
{
	r *= f;
	g *= f;
	b *= f;
	return *this;
}