#ifndef STAMPEDNOISE_H
#define STAMPEDNOISE_H

#include "PerlinNoise.h"
#include "ScalarField.h"
#include "ScalarGrid.h"

using namespace lux;

class StampedNoise:public ScalarField{
	public:
		StampedNoise(float scale, float fall, float b, Vector llc, float d, float n);

		void setNoiseParams(float f, float r, int o, float fj, float a, Vector trans);
		void init();
		float fractalSumPerlinNoise(const Vector& x);


		float eval(const Vector &x);
		Vector grad(const Vector &x);

	private:
		int octaves;
		float amplitude;
		float frequency;
		float roughness;
		float fJump;
		Vector translation;
		float density;

		PerlinNoise noise;
		ScalarGrid* grid;
		float pScale;
		Vector pCenter;
		float falloff;
		float boundary;

};

#endif
