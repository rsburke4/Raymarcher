#include "Shell.h"
#include "Dilation.h"
#include "Cutout.h"

float Shell::eval(const lux::Vector& x){
	Dilation A = Dilation(field, thickness/2);
	Dilation B = Dilation(field, -thickness/2);
	return Cutout(&A, &B).eval(x);
}
