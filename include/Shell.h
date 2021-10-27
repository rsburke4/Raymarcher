#ifndef SHELL_H
#define SHELL_H

#include "ScalarField.h"
#include "Shell.h"

class Shell:public ScalarField{
	public:
		Shell(ScalarField* sf, float thick){field = sf; thickness = thick;}
		float eval(const lux::Vector& x);
		lux::Vector grad(){ return lux::Vector(0, 0, 0); }

	private:
		ScalarField* field;
		float thickness;
};

#endif
