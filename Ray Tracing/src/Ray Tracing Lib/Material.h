#pragma once
#include "Color.h"

//ka : la composante ambiante, qui se multiplie par la lumi�re ambiante de la sc�ne.
//kd : la composante diffuse, qui se multiplie par la composante diffuse de chaque lumi�re.
//ks : la composante sp�culaire, qui se multiplie par la composante sp�culaire de chaque lumi�re.

class Material {
public:
	Material();
	Material(const Color& kd, const Color& ks, const Color& ka, float shininess);
	Color kd;
	Color ka;
	Color ks;
	float shininess;
};