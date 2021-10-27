#ifndef BLENDING_H
#define BLENDING_H

#include "ScalarField.h"

class Blending:public ScalarField{
	public:
		Blending(ScalarField* a, ScalarField* b, float sig1, float sig2, float lerp){
			fieldA = a;
			fieldB = b;
			sigma1 = sig1;
			sigma2 = sig2;
			beta = lerp;}

		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		ScalarField* fieldA;
		ScalarField* fieldB;
		float sigma1;
		float sigma2;
		float beta;
};

#endif
