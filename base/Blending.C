#include "Blending.h"

float Blending::eval(const lux::Vector& x){
	float result = fieldA->eval(x)/sigma1 + fieldB->eval(x)/sigma2 - beta;
	return result;
}

lux::Vector Blending::grad(const lux::Vector& x){
	return lux::Vector(0, 0, 0);
}
