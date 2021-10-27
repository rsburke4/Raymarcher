#include <cmath>
#include <iostream>
#include "PyroclasticSphere.h"
#include "PerlinNoise.h"

using namespace lux;

float PyroclasticSphere::fractalSumPerlinNoise(const Vector& x){
	Vector trans = x - translation;

	float fractalSum = 0.0;
	for(int i = 0; i < octaves; i++){
		Vector input = trans * frequency * pow(fJump, i);
		fractalSum +=  pow(roughness, i) * noise.eval(input);
		//std::cout << "POW: " << pow(roughness, i) << std::endl;
	}
	fractalSum *= (1.0 - roughness)/(1.0 - pow(roughness, (float)octaves));
	fractalSum *= amplitude;
	//std::cout << "PYROEVALInside: " << fractalSum << std::endl;
	return fractalSum;
}

PyroclasticSphere::PyroclasticSphere(float rad, float f, float r, int o, float fj, float a, Vector trans){
	roughness = r;
	octaves = o;
	fJump = fj;
	translation = trans;
	frequency = f;
	radius = rad;
	amplitude = a;
	sphere = Sphere(radius);
	noise = PerlinNoise(1);

}

float PyroclasticSphere::eval(const Vector& x){
	Vector input = (x/x.magnitude()) * radius;
	//float fspn = fractalSumPerlinNoise(input);
	float result = sphere.eval(x) + fabs(fractalSumPerlinNoise(input));
	//std::cout << "Pyroeval Outside: " << fractalSumPerlinNoise(input) << std::endl;
	//std::cout << "Pyroeval ABS: " << fabs(fspn) << std::endl;
	return result;
}
