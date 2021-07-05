#pragma once
#include "Polygon.h"
#include "../Maths/Maths.h"

class Sutherland {
private:
	Maths m_maths;
public:
	Sutherland();
	Polygon Clip(Polygon& poly, Polygon& window);
	bool Cut(Vector S, Vector Pj, Vector Fi, Vector Fiplus1);
	bool Visible(Vector S, Vector Fi, Vector Fiplus1, Polygon windowPolygon);
	Vector Intersection(Vector S, Vector Pj, Vector Fi, Vector Fiplus1);
	bool IsIntersectionSegmentSegment(Vector S, Vector Pj, Vector Fi, Vector Fiplus1);
	bool IsPolygonAInsideB(Polygon a, Polygon b);
};