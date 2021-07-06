#include "color.h"

#include <cmath>

Color::Color() : r(0.0f), g(0.0f), b(0.0f) {}

Color::Color(float r1, float g1, float b1) {
	if (r1 < 0) {
		r = 0;
	}
	else if (r1 > 255) {
		r = 255;
	}
	else {
		r = r1;
	}

	if (g1 < 0) {
		g = 0;
	}
	else if (g1 > 255) {
		g = 255;
	}
	else {
		g = g1;
	}

	if (b1 < 0) {
		b = 0;
	}
	else if (b1 > 255) {
		b = 255;
	}
	else {
		b = b1;
	}
}

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