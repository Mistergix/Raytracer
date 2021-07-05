#include "Transform.h"

void Transform::SetPos(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void Transform::SetRotInDegrees(float x, float y, float z)
{
	m_rx = x;
	m_ry = y;
	m_rz = z;
}

void Transform::SetScale(float x, float y, float z)
{
	m_sx = x;
	m_sy = y;
	m_sz = z;
}

Transform::Transform()
{
	SetPos(0.0f, 0.0f, 0.0f);
	SetRotInDegrees(0.0f, 0.0f, 0.0f);
	SetScale(1.0f, 1.0f, 1.0f);
}

float* Transform::GetTranslationMatrix()
{
	float translationMatrix[] = {
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				m_x, m_y, m_z, 1.f
	};

	return translationMatrix;
}

float* Transform::GetRotationMatrixX()
{
	float angle = m_rx * PI / 180.0f;
	float rotationMatrixX[] = {
			1.f, 0.f, 0.f, 0.f, // 1ere colonne
			0.f, cos(angle), sin(angle), 0.f, // 2eme colonne
			0.f, -sin(angle), cos(angle), 0.f, // 3eme colonne
			0.f, 0.f, 0.f, 1.f // 4eme colonne
	};

	return rotationMatrixX;
}

float* Transform::GetRotationMatrixY()
{
	float angle = m_ry * PI / 180.0f;
	float rotationMatrixY[] = {
			cos(angle), 0.f, -sin(angle), 0.f, // 1ere colonne
			0.f, 1.f, 0.f, 0.f, // 2eme colonne
			sin(angle), 0.f, cos(angle), 0.f, // 3eme colonne
			0.f, 0.f, 0.f, 1.f // 4eme colonne
	};

	return rotationMatrixY;
}

float* Transform::GetRotationMatrixZ()
{
	float angle = m_rz * PI / 180.0f;
	float rotationMatrixZ[] = {
			cos(angle), sin(angle), 0.f, 0.f, // 1ere colonne
			-sin(angle), cos(angle), 0.f, 0.f, // 2eme colonne
			0.f, 0.f, 1.f, 0.f, // 3eme colonne
			0.f, 0.f, 0.f, 1.f // 4eme colonne
	};

	return rotationMatrixZ;
}

float* Transform::GetScaleMatrix()
{
	float scaleMatrix[] = {
				m_sx, 0.f, 0.f, 0.f,
				0.f, m_sy, 0.f, 0.f,
				0.f, 0.f, m_sz, 0.f,
				0.f, 0.f, 0.f, 1.f
	};

	return scaleMatrix;
}

float Transform::GetX()
{
	return m_x;
}

float Transform::GetY()
{
	return m_y;
}

float Transform::GetZ()
{
	return m_z;
}

float Transform::GetRX()
{
	return m_rx;
}

float Transform::GetRY()
{
	return m_ry;
}

float Transform::GetRZ()
{
	return m_rz;
}

float Transform::GetSX()
{
	return m_sx;
}

float Transform::GetSY()
{
	return m_sy;
}

float Transform::GetSZ()
{
	return m_sz;
}
