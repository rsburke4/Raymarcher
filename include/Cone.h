#ifndef CONE_H
#define CONE_H

#include "ScalarField.h"

class Cone:public ScalarField{
	public:
		Cone(float h, float t, lux::Vector x, lux::Vector n){
			height = h;
			theta = t;
			x0 = x;
			normal = n.unitvector();}

		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		float height;
		float theta;
		lux::Vector x0;
		lux::Vector normal;
};

#endif
