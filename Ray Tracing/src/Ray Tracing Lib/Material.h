#pragma once
#include "Color.h"

//ka : la composante ambiante, qui se multiplie par la lumière ambiante de la scène.
//kd : la composante diffuse, qui se multiplie par la composante diffuse de chaque lumière.
//ks : la composante spéculaire, qui se multiplie par la composante spéculaire de chaque lumière.

class Material {
public:
	Material();
	Material(const Color& kd, const Color& ks, const Color& ka, float shininess);
	Color kd;
	Color ka;
	Color ks;
	float shininess;
};