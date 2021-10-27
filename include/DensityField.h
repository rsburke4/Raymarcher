#ifndef DENSITYFIELD_H
#define DENSITYFIELD_H

#include "Vector.h"
#include "ScalarField.h"
#include "Mask.h"

class DensityField:public ScalarField{
	public:
		DensityField(ScalarField* sf, float d){mask = sf;  density = d; }
		float eval(const lux::Vector& x){return mask->eval(x) * density;}
		lux::Vector grad(const lux::Vector& x){return lux::Vector(0,0,0);}

	private:
		ScalarField* mask;
		float density;
};

#endif
