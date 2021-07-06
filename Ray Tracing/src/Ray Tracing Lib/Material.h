#pragma once
#include "Color.h"

class Material {
public:
	Material();
	Material(const Color& kd, const Color& ks, const Color& ka, float shininess);
	Color kd;
	Color ka;
	Color ks;
	float shininess;
};