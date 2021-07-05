#pragma once
#include "Maths/Maths.h"
class Camera {
public:
	float* GetPerspectiveMatrix(float fovInDegrees, float near, float far, float aspectRatio);
	float* GetOrthographicMatrix(float right, float left, float top, float bottom, float far, float near);
	float* GetOrthographicMatrix(float size, float aspectRatio, float far, float near);
};