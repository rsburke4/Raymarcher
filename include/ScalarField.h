#ifndef SCALARFIELD_H
#define SCALARFIELD_H

#include "Vector.h"

class ScalarField{
	public:
		ScalarField(){}
		virtual float eval(const lux::Vector& x){return -1;}
		virtual lux::Vector grad(const lux::Vector& x){return lux::Vector(0, 0, 0);}
};


#endif
