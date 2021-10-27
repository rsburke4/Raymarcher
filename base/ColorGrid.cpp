#include <iostream>
#include "ColorGrid.h"

ColorGrid::ColorGrid(ColorField* sf, lux::Vector corner, float dx, float dy, float dz, int nx, int ny, int nz){
	field = sf;

	deltaX = dx;
	deltaY = dy;
	deltaZ = dz;

	numX = nx;
	numY = ny;
	numZ = nz;

	llc = corner;

	initPoints();
}

ColorGrid::ColorGrid(ColorField* sf, lux::Vector corner, float d, int n){
	field = sf;

	deltaX = d;
	deltaY = d;
	deltaZ = d;

	numX = n;
	numY = n;
	numZ = n;

	llc = corner;

	initPoints();
}

void ColorGrid::initPoints(){

	for(int k = 0; k < numX; k++){
		std::cout << "K: " << k << std::endl;
		for(int j = 0; j < numY; j++){
			for(int i = 0; i < numZ; i++){
				float locationX = llc.X() + i * deltaX;
				float locationY = llc.Y() + j * deltaY;
				float locationZ = llc.Z() + k * deltaZ;
				lux::Color value = field->eval(lux::Vector(locationX, locationY, locationZ));
				points.push_back(value);
			}
		}
	}
}

lux::Color ColorGrid::getValueAt(lux::Vector x){
	int i = x.X();
	int j = x.Y();
	int k = x.Z();

	int index = i + (numX * j) + (numX * numY * k);

	return points[index];
}

lux::Color ColorGrid::eval(const lux::Vector& x){
	lux::Color value = lux::Color(0.0, 0.0, 0.0, 0.0);

	lux::Vector y = x - llc;

	int i = int(y.X()/deltaX);
	int j = int(y.Y()/deltaY);
	int k = int(y.Z()/deltaZ);

	float weightI = (y.X()/deltaX) - i;
	float weightJ = (y.Y()/deltaY) - j;
	float weightK = (y.Z()/deltaZ) - k;

	//Check boundary conditions
	if((i+1 > numX) || (j+1 > numY) || (k+1 > numZ)){
		return lux::Color(0.0, 0.0, 0.0, 0.0);
	}
	if((i < 0) || (j < 0) || (k < 0)){
		return lux::Color(0.0, 0.0, 0.0, 0.0);
	}

	//This is the lerp for 8 crners of a cube
	lux::Color Gijk = getValueAt(lux::Vector(i, j, k));
	value += Gijk * ((1 - weightI) * (1 - weightJ) * (1 - weightK));
	Gijk = getValueAt(lux::Vector(i+1, j, k));
	value += Gijk * (weightI * (1 - weightJ) * (1 - weightK));
	Gijk = getValueAt(lux::Vector(i, j+1, k));
	value += Gijk * ((1 - weightI) * weightJ * (1 - weightK));
	Gijk = getValueAt(lux::Vector(i, j, k+1));
	value += Gijk * ((1 - weightI) * (1 - weightJ) * weightK);
	Gijk = getValueAt(lux::Vector(i+1, j+1, k));
	value += Gijk * (weightI * weightJ * (1 - weightK));
	Gijk = getValueAt(lux::Vector(i+1, j, k+1));
	value += Gijk * (weightI * (1 - weightJ) * weightK);
	Gijk = getValueAt(lux::Vector(i, j+1, k+1));
	value += Gijk * ((1 - weightI) * weightJ * weightK);
	Gijk = getValueAt(lux::Vector(i+1, j+1, k+1));
	value += Gijk * weightI * weightJ * weightK;

	return value;
}
