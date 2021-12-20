#ifndef PYRODISPLACE_H
#define PYRODISPLACE_H

#include "PerlinNoise.h"
#include "ScalarField.h"
#include "Vector.h"

using namespace lux;
class PyroDisplace:public ScalarField{
	public:
		PyroDisplace(){}
		PyroDisplace(ScalarField* sf, float f, float r, int o, float fj, float a, Vector trans);
		float eval(const Vector& x);
		Vector grad(const Vector& x);

	private:
		ScalarField *field;

		//Noise Params
		float frequency;
		float roughness;
		int octaves;
		float fJump;
		float amplitude;
		Vector translation;
		PerlinNoise noise;
};

#endif
