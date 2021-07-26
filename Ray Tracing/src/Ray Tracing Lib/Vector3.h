#pragma once
#include <iostream>
#include "../Maths.h"

struct Vector3 {
    float x;
    float y;
    float z;
    float w;

    Vector3();
    Vector3(float a, float b, float c);
    Vector3(float a, float b, float c, float w);
    Vector3(const Vector3& other);

    float GetByDim(int dim) const;

    void Print();
    
    inline float magnitude();
    inline float magnitudeSquared();

    static Vector3 Up();
    static Vector3 Right();
    static Vector3 Forward();

    static Vector3 One();

    Vector3 normalized();

    Vector3 operator -() const; 
    Vector3& operator += (const Vector3& other);
    Vector3& operator -= (const Vector3& other);
    Vector3& operator /= (float f);
    Vector3& operator *= (float f);
    Vector3& operator = (const Vector3& other);
    
};

float dot(Vector3 v1, Vector3 v2);
Vector3 cross(Vector3 v1, Vector3 v2);

inline float square(float n)
{
	return n * n;
}

inline Vector3 operator + (const Vector3& a, const Vector3& b){
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector3 operator - (const Vector3& a, const Vector3& b){
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector3 operator * (const Vector3& a, const Vector3& b){
    return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vector3 operator * (const Vector3& a, float f){
    return Vector3(a.x * f, a.y * f, a.z * f);
}

inline Vector3 operator * (float f, const Vector3& a){
    return a * f;
}

inline Vector3 operator / (const Vector3& a, const Vector3& b){
    return Vector3(a.x / b.x, a.y/ b.y, a.z / b.z);
}

inline Vector3 operator / (const Vector3& a, float f) {
    return a * (1 / f);
}

inline Vector3 operator / (float f, const Vector3& a){
    return Vector3(f / a.x, f / a.y, f / a.z);
}