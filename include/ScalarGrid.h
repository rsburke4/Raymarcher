#ifndef GRID_H
#define GRID_H

#include <vector>
#include "ScalarField.h"

class ScalarGrid:public ScalarField{
	public:
		ScalarGrid(){}
		ScalarGrid(ScalarField* sf, lux::Vector corner, float dx, float dy, float dz, int nx, int ny, int nz);
		ScalarGrid(ScalarField* sf, lux::Vector corner, float d, int n);

		float eval(const lux::Vector&x);

		void initPointsAs(float val);
		void initPointsFromField();
		bool isValidIndex(lux::Vector ijk);
		void initPointsAsInf();
		lux::Vector getLlc(){ return llc; }
		int getNumX(){ return numX; }
		int getNumY(){ return numY; }
		int getNumZ(){ return numZ; }
		float getDX(){ return deltaX; }
		float getDY(){ return deltaY; }
		float getDZ(){ return deltaZ; }
		float getValueAt(lux::Vector x);
		void setValueAt(lux::Vector x, float v);
		lux::Vector grad(const lux::Vector&x){ return lux::Vector(0.0, 0.0, 0.0); }
		std::vector<int> getIJK(int index);

		lux::Vector getAxisSizes(){ return lux::Vector(numX, numY, numZ); }
	protected:
		ScalarField* field;

		std::vector<float> points;

		lux::Vector llc;
		float deltaX;
		float deltaY;
		float deltaZ;

		int numX;
		int numY;
		int numZ;

};

#endif
