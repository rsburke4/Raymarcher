#ifndef TORUS_H
#define TORUS_H

#include "Vector.h"
#include "ScalarField.h"

class Torus:public ScalarField{
	public:
		Torus(){rMajor = 0; rMinor = 0;}
		Torus(float rMaj, float rMin, lux::Vector n){
			rMajor = rMaj;
			rMinor = rMin;
			normal = n;}

		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		float rMajor;
		float rMinor;
		lux::Vector normal;
};

#endif
