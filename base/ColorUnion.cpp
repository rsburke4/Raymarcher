#include <iostream>
#include "ColorUnion.h"


lux::Color ColorUnion::eval(const lux::Vector& x){
	return color1->eval(x) + color2->eval(x);
}

lux::Matrix ColorUnion::grad(const lux::Vector& x){
	return lux::Matrix();
}
