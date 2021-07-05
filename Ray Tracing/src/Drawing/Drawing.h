#pragma once
#include "../Maths/Maths.h"
#include "../Maths/Polygon.h"
#include <GLFW\glfw3.h>

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

struct Color {
	int r, g, b, a;
};

class Drawing {
private:
	int m_width, m_height;
public:
	Drawing(int w, int h);
	void DrawPixel(int x, int y, Color c, GLubyte (*texture)[SCR_WIDTH][4]);
	void DrawLine(Vector a, Vector b, Color c, GLubyte(*texture)[SCR_WIDTH][4]);
	void DrawPolygon(Polygon p, Color c, GLubyte(*texture)[SCR_WIDTH][4]);
	void Fill(Polygon p, Color c, GLubyte(*texture)[SCR_WIDTH][4]);
};