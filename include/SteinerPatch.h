#ifndef STEINERPATCH_H
#define STEINERPATCH_H

#include "ScalarField.h"
#include "Vector.h"

class SteinerPatch:public ScalarField{
	public:
		SteinerPatch(){}

		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);


};

#endif
