#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "ScalarField.h"

class Intersection:public ScalarField{
	public:
		Intersection(ScalarField* a, ScalarField* b){fieldA = a; fieldB = b;}

		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		ScalarField* fieldA;
		ScalarField* fieldB;
};

#endif
