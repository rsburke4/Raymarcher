#ifndef PYROSPHERE_H
#define PYROSPHERE_H

#include "ScalarField.h"
#include "Sphere.h"
#include "Vector.h"
#include "PerlinNoise.h"

using namespace lux;

class PyroclasticSphere:public ScalarField{
	public:
		PyroclasticSphere(float rad, float f, float r, int o, float fj, float a, Vector trans);
		float fractalSumPerlinNoise(const Vector& X);

		float eval(const Vector& x);

	private:
		float frequency;
		float roughness;
		float fJump;
		float octaves;
		float amplitude;
		float radius;
		Sphere sphere;
		Vector translation;
		PerlinNoise noise;
};

#endif
