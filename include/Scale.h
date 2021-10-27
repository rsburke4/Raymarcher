#ifndef SCALE_H
#define SCALE_H

#include "ScalarField.h"

class Scale:public ScalarField{
	public:
		Scale(ScalarField* sf, float t){ field = sf; factor = t; }

		float eval(const lux::Vector& x){ return field->eval(x * (1/factor));}
		lux::Vector grad(const lux::Vector& x){ return lux::Vector(0, 0, 0);}


	private:
		ScalarField* field;
		float factor;

};

#endif
