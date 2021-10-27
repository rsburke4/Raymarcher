#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H

#include "ScalarField.h"

class Icosahedron:public ScalarField{
	public:
		Icosahedron(){}
		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:

};

#endif
