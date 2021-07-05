#include "vector3.h"

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3::Vector3(float a, float b, float c) : x(a), y(b), z(c) {}

Vector3::Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

Vector3 Vector3::Up() {return Vector3(0.0f, 1.0f, 0.0f);}
Vector3 Vector3::Right() {return Vector3(1.0f, 0.0f, 0.0f);}
Vector3 Vector3::Forward() {return Vector3(0.0f, 0.0f, 1.0f);}

inline float Vector3::magnitudeSquared() {
    return square(x) + square(y) + square(z);
}

inline float Vector3::magnitude() {
    return sqrt(magnitudeSquared());
}

Vector3 Vector3::normalized(){
    Vector3 copy (*this);
    float mag = copy.magnitude();
    copy = copy / mag;
    return copy;
}

Vector3& Vector3::operator =(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

Vector3& Vector3::operator += (const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator -= (const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3& Vector3::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector3& Vector3::operator /= (float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

Vector3 Vector3::operator -() const
{
	return Vector3(-x, -y, -z);
}

float dot(Vector3 a, Vector3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 cross(Vector3 a, Vector3 b)
{
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}