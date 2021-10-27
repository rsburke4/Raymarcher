#ifndef DILATION_H
#define DILATION_H

#include "ScalarField.h"

class Dilation:public ScalarField{
	public:
		Dilation(ScalarField* sf, float f){field = sf; factor = f;}

		float eval(const lux::Vector& x){ return field->eval(x) + factor;}
		lux::Vector grad(const lux::Vector& x){ return lux::Vector(0, 0, 0); }

	private:
		ScalarField* field;
		float factor;
};

#endif
