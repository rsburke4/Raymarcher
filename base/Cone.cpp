#include <iostream>
#include "Cone.h"


float Cone::eval(const lux::Vector& x){
	if(x == x0){
		return 0.0f;
	}
	else if((x - x0)*normal > height){
		return height - (x - x0) * normal;
	}
	else if((x - x0)*normal < 0){
		return (x - x0) * normal;
	}
	else{
		return theta - acos(((x-x0)*normal)/(x-x0).magnitude());
	}
}

//Fix this later
lux::Vector Cone::grad(const lux::Vector& x){
	return lux::Vector(0.0, 0.0, 0.0);
}
