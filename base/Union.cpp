#include "Union.h"
#include "Vector.h"

float Union::eval(const lux::Vector& x){
	return fmax(A->eval(x), B->eval(x));
}

lux::Vector Union::grad(const lux::Vector& x){
	return lux::Vector(0, 0, 0);
}
