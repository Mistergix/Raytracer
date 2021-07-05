#pragma once
struct Color
{
	float r, g, b;

	Color();
	Color(float r, float g, float b);

	Color& operator =(const Color& other);
	Color& operator +=(const Color& other);
	Color& operator *=(const Color& other);
	Color& operator *=(float f);
};

inline Color operator *(const Color& a, const Color& b)
{
	return Color(a.r * b.r, a.g * b.g, a.b * b.b);
}

inline Color operator *(float f, const Color& color)
{
	return Color(f * color.r, f * color.g, f * color.b);
}

inline Color operator *(const Color& color, float f)
{
	return Color(color.r * f, color.g * f, color.b * f);
}

inline Color operator +(const Color& a, const Color& b)
{
	return Color(a.r + b.r, a.g + b.g, a.b + b.b);
}