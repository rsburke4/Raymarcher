#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "ScalarField.h"
#include "Vector.h"

class Ellipse:public ScalarField{
	public:
		Ellipse(float r1, float r2, lux::Vector n);

		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		float rMajor;
		float rMinor;
		lux::Vector normal;

};

#endif
