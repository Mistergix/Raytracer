#pragma once
#include "Maths/Maths.h"

class Transform {
private:
	float m_x, m_y, m_z;
	float m_rx, m_ry, m_rz;
	float m_sx, m_sy, m_sz;
public:
	void SetPos(float x, float y, float z);
	void SetRotInDegrees(float x, float y, float z);
	void SetScale(float x, float y, float z);
	Transform();
	float* GetTranslationMatrix();
	float* GetRotationMatrixX();
	float* GetRotationMatrixY();
	float* GetRotationMatrixZ();
	float* GetScaleMatrix();
	float GetX();
	float GetY();
	float GetZ();
	float GetRX();
	float GetRY();
	float GetRZ();
	float GetSX();
	float GetSY();
	float GetSZ();
};