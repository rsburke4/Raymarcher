#include<iostream>
#include "Cutout.h"

float Cutout::eval(const lux::Vector& x){
	//std::cout << fmax(A->eval(x), -1.0 * B->eval(x)) << std::endl;
	return fmin(fieldA->eval(x), -1.0 * fieldB->eval(x));
}

lux::Vector Cutout::grad(const lux::Vector& x){
	return lux::Vector(0, 0, 0);
}
