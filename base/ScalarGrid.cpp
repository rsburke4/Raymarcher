#include <iostream>
#include "ScalarGrid.h"

using namespace std;

ScalarGrid::ScalarGrid(ScalarField* sf, lux::Vector corner, float dx, float dy, float dz, int nx, int ny, int nz){
	field = sf;

	deltaX = dx;
	deltaY = dy;
	deltaZ = dz;

	numX = nx;
	numY = ny;
	numZ = nz;

	llc = corner;

//	initPoints();
}

ScalarGrid::ScalarGrid(ScalarField* sf, lux::Vector corner, float d, int n){
	field = sf;

	deltaX = d;
	deltaY = d;
	deltaZ = d;

	numX = n;
	numY = n;
	numZ = n;

	llc = corner;

//	initPoints();
}

void ScalarGrid::initPointsFromField(){
	if(field == nullptr){
		field = new ScalarField();
	}
	for(int k = 0; k < numX; k++){
		std::cout << "K: " << k << std::endl;
		for(int j = 0; j < numY; j++){
			for(int i = 0; i < numZ; i++){
				float locationX = llc.X() + i * deltaX;
				float locationY = llc.Y() + j * deltaY;
				float locationZ = llc.Z() + k * deltaZ;
				float value = field->eval(lux::Vector(locationX, locationY, locationZ));
			//	std::cout << "FIRSTVAL: " << value << std::endl;
			//	cout << "ijkVal: " << i << " " << j << " " << k << " " << value << endl;
				points.push_back(value);
			}
		}
	}
}

float ScalarGrid::getValueAt(lux::Vector x){
	int i = x.X();
	int j = x.Y();
	int k = x.Z();


	int index = i + (numX * j) + (numX * numY * k);

	if(index > points.size()){
		return 0;
	}
	if(index < 0){
		return 0;
	}
	return points[index];
}

float ScalarGrid::eval(const lux::Vector& x){
	float value = 0;

	lux::Vector y = x - llc;

	int i = y.X()/deltaX;
	int j = y.Y()/deltaY;
	int k = y.Z()/deltaZ;

	float weightI = (y.X()/deltaX) - i;
	float weightJ = (y.Y()/deltaY) - j;
	float weightK = (y.Z()/deltaZ) - k;
	

	//Check boundary conditions
	if((i+1 > numX) || (j+1 > numY) || (k+1 > numZ)){
		//cout << "Out of grid Bounds GET\n";
	//	cout << i << " " << j << " " << k << endl;
		return 0;
	}
	if((i < 0) || (j < 0) || (k < 0)){
		//cout << "Out of grid Bounds GET\n"; 
	//	cout << i << " " << j << " " << k << endl;
		return 0;
	}

	//This is the lerp for 8 crners of a cube
	float Gijk = getValueAt(lux::Vector(i, j, k));

	//cout << "Gijk: " << Gijk << endl;

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

void ScalarGrid::initPointsAsInf(){

	points.resize(numX*numY*numZ);
	for(int i = 0; i < numX*numY*numZ; i++){
		points[i] = 10 * pow(10, 6);
	}

	/*for(int k = 0; k < numX; k++){
		std::cout << "K: " << k << std::endl;
		for(int j = 0; j < numY; j++){
			for(int i = 0; i < numZ; i++){
				float locationX = llc.X() + i * deltaX;
				float locationY = llc.Y() + j * deltaY;
				float locationZ = llc.Z() + k * deltaZ;
				float value = field->eval(lux::Vector(locationX, locationY, locationZ));
			//	std::cout << "FIRSTVAL: " << value << std::endl;
				points.push_back(value);
			}
		}
	}*/

}

void ScalarGrid::initPointsAs(float val){
	points.resize(numX*numY*numZ);
	for(unsigned int i = 0; i < points.size(); i++){
		points[i] = val;
	}
}

void ScalarGrid::setValueAt(lux::Vector x, float v){
	int i = x.X();
	int j = x.Y();
	int k = x.Z();

	/*for(size_t y = 0; y < points.size(); y++){
		std::cout << "LOG: " << points[y] << std::endl;
	}*/

	if((i > numX) || (j > numY) || (k > numZ)){
		//cout << "Out of grid Bounds SET\n";
		return;
	}
	if((i < 0) || (j < 0) || (k < 0)){
		//cout << "Out of grid Bounds SET\n"; 
		return;
	}
	//cout << "IN BOUNDS SET\n";

	int index = i + (numX * j) + (numX * numY * k);

	if(index > points.size()-1){
		return;
	}
	if(index < 0){
		return;
	}

//	cout << "Hey buddy. I actually set a value.\n";
	points[index] = v;
}

vector<int> ScalarGrid::getIJK(int index){
	int k = index / (numY * numX);
	int j = (index / numX) - (k * numY);
	int i = index - (k * numY * numX) - (j * numX);
	vector<int> values{i, j, k};
	return values;
}

bool ScalarGrid::isValidIndex(lux::Vector ijk){
	int i = ijk.X();
	int j = ijk.Y();
	int k = ijk.Z();

	if(i < 0 || j < 0 || k < 0){
		return false;
	}
	if(i > numX-1 || j > numY-1 || k > numZ-1){
		return false;
	}
	return true;
}
