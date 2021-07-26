#pragma once
#include "../Maths.h"
#include "Ray Tracing Lib/Entity.h"
class Camera : public Entity {
public:
	float* GetPerspectiveMatrix(float fovInDegrees, float near, float far, float aspectRatio);
	float* GetOrthographicMatrix(float right, float left, float top, float bottom, float far, float near);
	float* GetOrthographicMatrix(float size, float aspectRatio, float far, float near);
};