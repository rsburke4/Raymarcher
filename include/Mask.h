#ifndef MASK_H
#define MASK_H

#include <cstddef>
#include "Vector.h"
#include "ScalarField.h"

class Mask:public ScalarField{
	public:
		Mask(){field = NULL;}
		Mask(ScalarField* sf){ field = sf; }
		float eval(const lux::Vector& x){
			//cout << "inside mask sphere val: " << field->eval(x) << endl;
		       	if(field != NULL){
				return (field->eval(x)>0)?1:0;
			}
			return 0;
		}
		lux::Vector grad(const lux::Vector& x){return lux::Vector(0, 0, 0);}

	private:
		ScalarField* field;
};

#endif
