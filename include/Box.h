#ifndef BOX_H
#define BOX_H

#include "ScalarField.h"

class Box:public ScalarField{
	public:
		Box(lux::Vector x, float r, float round){p = x; q = round; radius = r;}
		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x);

	private:
		float q;
		lux::Vector p;
		float radius;
};

#endif
