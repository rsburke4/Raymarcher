#include "SteinerPatch.h"

float SteinerPatch::eval(const lux::Vector& x){
	float result = (pow(x.X(),2) * pow(x.Y(),2))
		+ (pow(x.X(), 2) * pow(x.Z(), 2))
		+ (pow(x.Y(), 2) * pow(x.Z(), 2))
		- (x.X() * x.Y() * x.Z());
	result *= -1;
	return result;
}

lux::Vector SteinerPatch::grad(const lux::Vector& x){
	return lux::Vector(0, 0, 0);
}
