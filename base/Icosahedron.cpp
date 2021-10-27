#include "Icosahedron.h"

float Icosahedron::eval(const lux::Vector& x){
	float T = (1 + sqrt(5))/2;
	if(x.magnitude() > 1.8*M_PI){
		return -1.8 * M_PI;	
	}
	else{
		float result = 
			cos(x.X() + T*x.Y()) + cos(x.X() - T*x.Y())
			+ cos(x.Y() + T*x.Z()) + cos(x.Y() - T*x.Z())
			+ cos(x.Z() + T*x.X()) + cos(x.Z() - T*x.X()) - 2.0;
		return result;
	}
}

lux::Vector Icosahedron::grad(const lux::Vector& x){
	return lux::Vector(0.0, 0.0, 0.0);
}
