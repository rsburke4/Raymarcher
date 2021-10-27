#ifndef UNION_H
#define UNION_H

#include "ScalarField.h"

class Union:public ScalarField{
	public:
		Union(ScalarField* a, ScalarField* b){A = a; B = b;}

		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		ScalarField* A;
		ScalarField* B;

};

#endif
