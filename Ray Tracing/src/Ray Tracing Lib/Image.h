#include <string>
#include <GLFW\glfw3.h>
#include "Color.h"

const unsigned int SCR_WIDTH = 100;
const unsigned int SCR_HEIGHT = 100;

class Image
{
protected:
	int width, height;
public:
	Image(int width, int height);

	int GetWidth() const;
	int GetHeight() const;
	void DrawPixel(int x, int y, Color c, GLubyte(*texture)[SCR_WIDTH][4]);
};