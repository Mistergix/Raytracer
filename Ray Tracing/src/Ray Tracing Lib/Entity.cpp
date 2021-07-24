#include "Entity.h"
#include <cmath>	  			  				    	  
Entity::Entity() {
	trans = Matrix();
	transInv = Matrix();
}

void Entity::translate(Vector3 transV) {
	Matrix t;
	t(0, 3) = transV.x;
	t(1, 3) = transV.y;
	t(2, 3) = transV.z;

	trans = t * trans;

	transInv = trans.inverse4x4();
}

void Entity::scale(float f) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			trans(i, j) = trans(i, j) * f;
		}
	}
	transInv = trans.inverse4x4();
}

void Entity::rotateX(float deg) {

	Matrix m;
	m(1, 1) = std::cos(deg);
	m(1, 2) = -std::sin(deg);
	m(2, 1) = std::sin(deg);
	m(2, 2) = std::cos(deg);

	trans = m * trans;

	transInv = trans.inverse4x4();
}
void Entity::rotateY(float deg) {

	Matrix m;
	m(0, 0) = std::cos(deg);
	m(0, 2) = std::sin(deg);
	m(2, 0) = -std::sin(deg);
	m(2, 2) = std::cos(deg);

	trans = m * trans;

	transInv = trans.inverse4x4();
}
void Entity::rotateZ(float deg) {
	Matrix m;
	m(0, 0) = std::cos(deg);
	m(0, 1) = -std::sin(deg);
	m(1, 0) = std::sin(deg);
	m(1, 1) = std::cos(deg);

	trans = m * trans;

	transInv = trans.inverse4x4();
}

void Entity::rotate(const Vector3 deg) {
	rotateX(deg.x);
	rotateY(deg.y);
	rotateZ(deg.z);
}

Vector3 Entity::localToGlobal(const Vector3& v) const {
    return trans * v;
}

Vector3 Entity::globalToLocal(const Vector3& v) const {
    return transInv * v;
}

Ray Entity::localToGlobal(const Ray& r) const {
	Vector3 origin = localToGlobal(r.origin);
	Vector3 direction = localToGlobal(r.direction);
	return Ray(origin, direction);
}

Ray Entity::globalToLocal(const Ray& r) const {
	Vector3 origin = globalToLocal(r.origin);
	Vector3 direction = globalToLocal(r.direction);
	return Ray(origin, direction);
}