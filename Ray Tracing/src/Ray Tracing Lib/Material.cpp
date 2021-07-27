#include "Material.h"


Material::Material() : kd(Color()), ks(Color()), ka(Color()), shininess(0.0f), useTexture(false)
{
	texture =  new Texture("res/textures/tricolor.png");
}

Material::Material(const Color& kd, const Color& ks, const Color& ka, float shininess, Texture* t, bool useTexture) : kd(kd), ks(ks), ka(ka), shininess(shininess), useTexture(useTexture)
{
	texture = t;
}
Color Material::GetDiffuseColor(const Vector3& uv) const
{
	if (useTexture) { return texture->Sample(uv); }
	return kd;
}
