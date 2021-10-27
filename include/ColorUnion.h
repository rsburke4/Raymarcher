#ifndef COLORUNION_H
#define COLORUNION_H

#include "Construction.h"
#include "ColorField.h"

class ColorUnion:public ColorField{
	public:
		ColorUnion(ColorField* cf1, ColorField* cf2){color1 = cf1; color2 = cf2;}

		lux::Color eval(const lux::Vector& x);
		lux::Matrix grad(const lux::Vector& x);

	private:
		ColorField* color1;
		ColorField* color2;

};

#endif
