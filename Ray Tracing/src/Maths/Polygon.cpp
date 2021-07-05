#include "Polygon.h"

Polygon::Polygon()
{
	std::vector<Vector> vector;
	m_points = vector;
}

Polygon::Polygon(std::vector<Vector> points)
{
	m_points = points;
}

bool Polygon::IsTrigonometric()
{
	return trigonometricOrientation;
}

void Polygon::SetTrigonometric(bool trigo)
{
	trigonometricOrientation = trigo;
}

//https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
bool Polygon::IsInside(Vector p)
{	 
	BoundingBox b = GetBoundingBox();

	if (p.getX() < b.xMin || p.getX() > b.xMax || p.getY() < b.yMin || p.getY() > b.yMax)
	{
		return false;
	}

	int count = m_points.size();

	bool inside = false;
	for (int i = 0, j = count - 1; i < count; j = i++)
	{
		bool condition1 = (m_points[i].getY() > p.getY()) != (m_points[j].getY() > p.getY());
		bool condition2 = p.getX() < (m_points[j].getX() - m_points[i].getX()) * (p.getY() - m_points[i].getY()) / (m_points[j].getY() - m_points[i].getY()) + m_points[i].getX();
		if ( condition1 && condition2) {
			inside = !inside;
		}
	}

	return inside;
}

void Polygon::Add(Vector pt)
{
	m_points.push_back(pt);
}

Vector Polygon::GetPoint(int i)
{
	return m_points[i];
}

void Polygon::Clear()
{
	m_points.clear();
}

int Polygon::PointCount()
{
	return m_points.size();
}

Vector Polygon::GetNormalInterior(Vector a, Vector b)
{
	if (trigonometricOrientation) {
		return Vector{ -(b.getY() - a.getY()), b.getX() - a.getX() };
	}

	return Vector{ b.getY() - a.getY(), -(b.getX() - a.getX()) };
}

std::vector<Vector> Polygon::GetPoints()
{
	return m_points;
}

BoundingBox Polygon::GetBoundingBox()
{
	float minX = m_points[0].getX();
	float maxX = minX;
	float minY = m_points[0].getY();
	float maxY = minY;

	int count = m_points.size();

	// compute bounding box
	for (int i = 1; i < count; i++)
	{
		Vector q = m_points[i];
		minX = std::min(q.getX(), minX);
		maxX = std::max(q.getX(), maxX);
		minY = std::min(q.getY(), minY);
		maxY = std::max(q.getY(), maxY);
	}

	return BoundingBox{minX, maxX, minY, maxY};
}
