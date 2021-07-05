#include "Camera.h"

float* Camera::GetPerspectiveMatrix(float fovInDegrees, float near, float far, float aspectRatio)
{
    float fov = fovInDegrees * PI / 180.f;
    float f = 1.f / tan(fov / 2.f);
    float projectionMatrix[] = {
        f / aspectRatio, 0.f, 0.f, 0.f, // 1ere colonne
        0.f, f, 0.f, 0.f, // 2eme colonne
        0.f, 0.f, (far + near) / (near - far), -1.f, // 3eme colonne
        0.f, 0.f, (2.f * far * near) / (near - far), 0.f // 4eme colonne
    };

    return projectionMatrix;
}

float* Camera::GetOrthographicMatrix(float right, float left, float top, float bottom, float far, float near)
{
    float projectionMatrix[] = {
        2.0f / (right - left), 0.f, 0.f, 0.f, // 1ere colonne
        0.f, 2.0f / (top - bottom), 0.f, 0.f, // 2eme colonne
        0.f, 0.f, -2.0f / (far - near), 0.f, // 3eme colonne
        -((right + left)/(right - left)), -((top + bottom) / (top - bottom)), -((far + near) / (far - near)), 1.f // 4eme colonne
    };

    return projectionMatrix;
}

float* Camera::GetOrthographicMatrix(float size, float aspectRatio, float far, float near)
{
    float right, left, bottom, top;
    float width = aspectRatio * size;
    right = width / 2.0f;
    left = -right;
    float height = width / aspectRatio;
    top = height / 2.0f;
    bottom = -top;

    return GetOrthographicMatrix(right, left, top, bottom, far, near);
}
