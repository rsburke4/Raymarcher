#ifndef WISP_H
#define WISP_H

#include <stdio.h>
#include <iostream>
#include <random>
#include "ScalarField.h"
#include "ScalarGrid.h"
#include "PerlinNoise.h"
#include "Vector.h"

using namespace lux;

class Wisp:public ScalarField{
	public:
		Wisp(
			float dens,
			int dots,
			Vector center,
			float pScl,
			long pNum,
			float clmp, 
			Vector llc,
			float d, 
			int n);

		void setNoiseParams();

		void init();
		float fractalSumPerlinNoise(const Vector& x);
		void setNoiseParams(float f, float r, int o, float fj, float a, Vector trans);
		float eval(const Vector& x){ return grid->eval(x); }
		Vector grad(const Vector& x){ return Vector(0.0, 0.0, 0.0);}

	private:
		int octaves;
		float amplitude;
		float frequency;
		float roughness;
		float fJump;
		Vector translation;
		float density;
		
		Vector pCenter;
		float pScale;
		long points;
		int wispDots;
		float clump;
		PerlinNoise noise;
		std::mt19937 twister;
		ScalarGrid* grid;
};

#endif
