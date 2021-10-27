#include "ColorField.h"
#include "Construction.h"

lux::Color ColorField::eval(const lux::Vector& x){
	lux::Color result;
	result = mask->eval(x) * color;
	return result;
}

lux::Matrix ColorField::grad(const lux::Vector& x){
	return lux::Matrix();
}
