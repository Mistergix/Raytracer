#pragma once
#include "Vector.h"
#include <vector>

struct BoundingBox {
	float xMin, xMax, yMin, yMax;
};

class Polygon {
private:
	std::vector<Vector> m_points;
	bool trigonometricOrientation;
public:
	Polygon();
	Polygon(std::vector<Vector> points);
	bool IsTrigonometric();
	void SetTrigonometric(bool trigo);
	bool IsInside(Vector pt);
	void Add(Vector pt);
	Vector GetPoint(int i);
	void Clear();
	int PointCount();
	Vector GetNormalInterior(Vector a, Vector b);
	std::vector<Vector> GetPoints();
	BoundingBox GetBoundingBox();
};