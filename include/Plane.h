#ifndef PLANE_H
#define PLANE_H

#include "ScalarField.h"

class Plane:public ScalarField{
	public:
		Plane(lux::Vector x, lux::Vector n){x0 = x; normal = n;}
		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		lux::Vector normal;
		lux::Vector x0;
};

#endif
