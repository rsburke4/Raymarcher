#ifndef COLORFIELD_H
#define COLORFIELD_H

#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Matrix.h"
#include "ScalarField.h"
#include "Union.h"
#include "Mask.h"

class ColorField{
	public:
		ColorField(){}
		ColorField(lux::Color c, ScalarField* m){ color = c; mask = m; }
		ColorField operator+(ColorField& cf);
		virtual lux::Color eval(const lux::Vector& x);
		virtual lux::Matrix grad(const lux::Vector& x);


//		lux::Color getColor(){ return color; }

	private:
		lux::Color color;
		ScalarField* mask;
};

#endif
