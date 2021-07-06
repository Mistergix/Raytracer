#include "RayHit.h"

RayHit::RayHit() : ray(), distance(RAY_DISTANCE_MAX), shape(NULL) { }

RayHit::RayHit(const RayHit& other) : ray(other.ray), distance(other.distance), shape(other.shape) { }

RayHit::RayHit(const Ray& ray) : ray(ray), distance(ray.distanceMax), shape(NULL) { }

RayHit& RayHit::operator =(const RayHit& other)
{
	ray = other.ray;
	distance = other.distance;
	shape = other.shape;
	return *this;
}

Vector3 RayHit::HitPoint() const
{
	return ray.Compute(distance);
}

bool RayHit::Hit() const
{
	return (shape != NULL);
}

Vector3 RayHit::Direction() const {
	return ray.direction;
}

Vector3 RayHit::Origin() const {
	return ray.origin;
}

Color RayHit::GetImpactColor(const Scene& scene)
{
	Vector3 impact = HitPoint();
	Material m = shape->GetMaterial(impact);
	Ray normal = shape->GetNormal(impact, ray.origin);
	Color c = m.ka * (scene.GetAmbiant()) * m.kd;
	for (int l = 0; l < scene.NbLights(); l++) {
		const Light* light = scene.GetLight(l);
		Vector3 lv = light->GetVectorToLight(impact);
		float alpha = dot(lv, normal.direction);
		if (alpha > 0.0f)
			c += (light->id) * (m.kd) * alpha;

		Vector3 rm = (2.0f * dot(lv, normal.direction) * normal.direction) - lv;

		float beta = dot(-rm, ray.direction);
		if (beta > 0.0f)
			c += (light->is) * (m.ks) * pow(beta, m.shininess);
	}

	return c;
}

Color RayHit::GetShadowFeeler(const Scene& scene)
{
	Vector3 impact = HitPoint();
	for (int l = 0; l < scene.NbLights(); l++) {
		const Light* light = scene.GetLight(l);
		Vector3 lv = light->GetVectorToLight(impact);
		Ray toLight(impact, lv);

		for (int s = 0; s < scene.NbShapes(); s++) {
			Shape* sh = scene.GetShape(s);
			if (shape->index == sh->index) { continue; }
			if (sh->DoesIntersect(toLight).intersect) {
				return color* scene.shadowFactor;
			}

		}
	}

	return color;
}

