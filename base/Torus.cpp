#include "Torus.h"

float Torus::eval(const lux::Vector& x){
	lux::Vector xperp = x - (x * normal) * normal;
	float result = 4 * pow(rMajor,2) * pow(xperp.magnitude(), 2);
	result -= pow((pow(x.magnitude(), 2) + pow(rMajor, 2) - pow(rMinor, 2)), 2);
	return result;
}

//We'll come back and fill this in later
lux::Vector Torus::grad(const lux::Vector& x){
	return lux::Vector(0, 0, 0);
}
