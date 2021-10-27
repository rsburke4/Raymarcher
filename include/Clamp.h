#ifndef CLAMP_H
#define CLAMP_H

#include <iostream>
#include "ScalarField.h"
#include "Vector.h"

using namespace lux;
using namespace std;

class Clamp:public ScalarField{
	public:
		Clamp(ScalarField* sf, float mn, float mx, float c){
			field = sf;
			min = mn;
			max = mx;
			constant = c;
		}
		Clamp(ScalarField* sf, float mn, float mx){
			field = sf;
			min = mn;
			max = mx;
			constant = 1;
		}
		float eval(const Vector& x){ 
			if(field->eval(x)/constant < min){
				return min;
			}
			if(field->eval(x)/constant > max){
				return max;
			}
			return field->eval(x) / constant;
		}
		Vector grad(const Vector& x) const{ return Vector(0, 0, 0); }

	private:
		ScalarField* field;
		float min;
		float max;
		float constant;
};

#endif
