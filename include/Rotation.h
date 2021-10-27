#ifndef ROTATION_H
#define ROTATION_H

#include <iostream>
#include "ScalarField.h"
#include "Matrix.h"

class Rotation:public ScalarField{
	public:
		Rotation(ScalarField* sf, lux::Matrix r){field = sf; rotation = r;}

		float eval(const lux::Vector& x){
			lux::Vector toVal = rotation.inverse().vecMultMat(x);
			return field->eval(toVal);
		}
		lux::Vector grad(const lux::Vector& x){ return lux::Vector(0, 0, 0);}

		static lux::Matrix createRotationMatrix(float angle, lux::Vector axis);
	private:
		ScalarField* field;
		lux::Matrix rotation;

};

#endif
