#include "Material.h"

Material::Material() : kd(Color()), ka(Color()), ks(Color()), shininess(0)
{
}

Material::Material(const Color& kd, const Color& ks, const Color& ka, float shininess) : kd(kd), ks(ks), ka(ka), shininess(shininess)
{
}
