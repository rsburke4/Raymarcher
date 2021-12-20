#include "PyroDisplace.h"
#include <iostream>

using namespace std;
using namespace lux;

PyroDisplace::PyroDisplace(ScalarField* sf, float f, float r, int o, float fj, float a, Vector trans){
	noise = PerlinNoise(1);

	field = sf;

	frequency = f;
	roughness = r;
	octaves = o;
	fJump = fj;
	amplitude = a;
	translation = trans;
}

float PyroDisplace::eval(const Vector& x){
	//Nearest (or closest) point transform
	Vector dist = x;
	int i = 0;
	float fEval = field->eval(dist);
	lux::Vector fGrad = field->grad(dist);
	while(fabs(fEval) >= 0.01){
		dist = dist - ((fEval*fGrad)/pow(fGrad.magnitude(), 2.0));
		//dist = field->eval(temp);
		fEval = field->eval(dist);
		fGrad = field->grad(dist);
	//	cout << "I: " << i << endl;
//		cout << "Fabs Eval: " << fabs(fEval) << endl;
//		cout << "Dist: " << dist.X() << " " << dist.Y() << " " << dist.Z() << endl;
		i++;
		if(i >= 10){ break; }
	}
//	cout << "Yes" << endl;
	float result = field->eval(x) + fabs(PerlinNoise::fractalSumPerlinNoise(
		&noise,
		dist,
		translation,
		frequency,
		roughness,
		octaves,
		fJump,
		amplitude)); 
	//cout << "Result: " << result << endl;
	return result;
}

lux::Vector PyroDisplace::grad(const lux::Vector& x){
//      cout << "Good Point" << endl;
                //Otherwise it's inside the grid
                //And we should proceed as usual
        float delta = 0.001;
        lux::Vector result;
        lux::Vector dX = lux::Vector(delta, 0.0, 0.0);
        lux::Vector dY = lux::Vector(0.0, delta, 0.0);
        lux::Vector dZ = lux::Vector(0.0, 0.0, delta);
        result[0] = (eval(x+dX) - eval(x-dX))/(2.0 * delta);
        result[1] = (eval(x+dY) - eval(x-dY))/(2.0 * delta);
        result[2] = (eval(x+dZ) - eval(x-dZ))/(2.0 * delta);
        return result;

}
