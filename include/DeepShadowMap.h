#ifndef DEEPSHADOWMAP_H
#define DEEPSHADOWMAP_H

#include <vector>
#include "ScalarField.h"
#include "ColorField.h"
#include "Light.h"
#include "Vector.h"
#include "Color.h"

using namespace std;

class DeepShadowMap:public ScalarGrid{
	public:
		DeepShadowMap(ScalarField* d, ColorField* c, Light* l, lux::Vector corner, float delta, int n, float s, float k);
		DeepShadowMap(){}

		//float eval(const lux::Vector &x){ return grid->eval(x); }
	//	lux::Vector grad(const lux::Vector& x){ return lux::Vector(0, 0, 0); }

		void initGrid();
		Light* getLight(){ return light; }

	private:
		Light* light;
		ScalarGrid* grid;
		ScalarField* dense;
		ColorField* color;
		int gridSize;
		float gridDelta; 
		float marchRes;
		float scatter;
		
};

#endif
