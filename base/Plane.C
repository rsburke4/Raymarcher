#include "Plane.h"

float Plane::eval(const lux::Vector& x){
	return (x - x0) * normal;
}

lux::Vector Plane::grad(const lux::Vector& x){
		return lux::Vector(0.0, 0.0, 0.0);
}
