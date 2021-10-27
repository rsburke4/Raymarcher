#ifndef TRANSLATION_H
#define TRANSLATION_h

#include "ScalarField.h"

class Translation:public ScalarField{
	public:
		Translation(ScalarField* f, lux::Vector dx){field = f; deltaX = dx;}

		float eval(const lux::Vector& x){return field->eval(x - deltaX);}
		lux::Vector grad(const lux::Vector& x){return lux::Vector(0, 0, 0);}

	private:
		ScalarField* field;
		lux::Vector deltaX;

};

#endif
