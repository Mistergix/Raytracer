#include "Maths.h"

float Maths::Scalar(Vector a, Vector b)
{
	return a.getX() * b.getX() + a.getY() * b.getY();
}
