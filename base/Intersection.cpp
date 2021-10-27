#include "Intersection.h"

float Intersection::eval(const lux::Vector& x){
	return  fmin(fieldA->eval(x), fieldB->eval(x));
}

lux::Vector Intersection::grad(const lux::Vector& x){
	return lux::Vector(0, 0, 0);
}
