#ifndef CUTOUT_H
#define CUTOUT_H

#include "ScalarField.h"

class Cutout:public ScalarField{
	public:
		Cutout(ScalarField* a, ScalarField* b){fieldA = a; fieldB = b;}

		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		ScalarField* fieldA;
		ScalarField* fieldB;
};

#endif
