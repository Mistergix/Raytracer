#include "Image.h"
#include <algorithm>

Image::Image(int width, int height) : width(width), height(height)
{
}

int Image::GetWidth() const
{
	return width;
}

int Image::GetHeight() const
{
	return height;
}

void Image::DrawPixel(int x, int y, Color c, GLubyte(*texture)[SCR_WIDTH][4])
{
	texture[y][x][0] = pow((std::min(255.0f, c.r) / 255.0f), 1.0f / 2.2f) * 255.99f;
	texture[y][x][1] = pow((std::min(255.0f, c.g) / 255.0f), 1.0f / 2.2f) * 255.99f;
	texture[y][x][2] = pow((std::min(255.0f, c.b) / 255.0f), 1.0f / 2.2f) * 255.99f;
	texture[y][x][3] = 255;
}
