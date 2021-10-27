#include "Box.h"

float Box::eval(const lux::Vector &x){
	return radius - pow(x.X(),2*q) - pow(x.Y(),2*q) - pow(x.Z(),2*q);
}

lux::Vector Box::grad(const lux::Vector &x){
	return lux::Vector(0.0, 0.0, 0.0);
}
