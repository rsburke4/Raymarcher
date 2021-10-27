#ifndef LEVELSET_H
#define LEVELSET_H

#include "ScalarField.h"
#include "ScalarGrid.h"
#include "OBJ_Loader.h"

using namespace std;

class LevelSet:public ScalarField{
	public:
		LevelSet(string filename, int band, lux::Vector corner, float d, int n);
		
		float eval(const lux::Vector& x);
		lux::Vector grad(const lux::Vector& x){ return lux::Vector(0.0, 0.0, 0.0); }

		lux::Vector nearestPointTransform(const lux::Vector& y);
		float signedDistance(lux::Vector ijk, lux::Vector n, lux::Vector p);

	private:
		std::vector<std::vector<int>> triangles;
		std::vector<lux::Vector> points;
		
		ScalarGrid* set;
		OBJ_Loader obj;

		float bandwidth;
};

#endif
