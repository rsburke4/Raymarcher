#include "Ellipse.h"

Ellipse::Ellipse(float r1, float r2, lux::Vector n){
	rMajor = r1;
	rMinor = r2;
	normal = n.unitvector();
}

float Ellipse::eval(const lux::Vector& x){
	float Z = x * normal;
	lux::Vector xPerp = x - (x * normal) * normal;
	float result = 1 - (pow(Z, 2) / pow(rMajor,2)) - (pow(xPerp.magnitude(), 2)/pow(rMinor,2));
	return result;
}

lux::Vector Ellipse::grad(const lux::Vector& x){
	return lux::Vector(0, 0, 0);
}
