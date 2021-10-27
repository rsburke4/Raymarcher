#include <iostream>
#include <math.h>
#include "StampedNoise.h"

using namespace lux;

StampedNoise::StampedNoise(float scale, float b, float fall, Vector llc, float d, float n){
	pScale = scale;
	falloff = fall;
	grid = new ScalarGrid(nullptr, llc, d, n);
	grid->initPointsAs(0.0);
	boundary = b;

	octaves = 0;
	amplitude = 0;
	frequency = 0;
	roughness = 0;
	fJump = 0;
	translation = Vector(0, 0, 0);
	density = 0;

	noise = PerlinNoise(1);
}

void StampedNoise::setNoiseParams(float f, float r, int o, float fj, float a, Vector trans){
	frequency = f;
	amplitude = a;
	octaves = o;
	fJump = fj;
	roughness = r;
	translation = trans;

	init();
}

float StampedNoise::fractalSumPerlinNoise(const Vector& x){
	Vector trans = x - translation;

	float fractalSum = 0.0;
	for(int i = 0; i < octaves; i++){
		Vector input = trans * frequency * pow(fJump, i);
		fractalSum +=  pow(roughness, i) * noise.eval(input);
		//std::cout << "POW: " << pow(roughness, i) << std::endl;
	}
	fractalSum *= (1.0 - roughness)/(1.0 - pow(roughness, (float)octaves));
	fractalSum *= amplitude;

	return fractalSum;
}

void StampedNoise::init(){
	pCenter = Vector(
		(int)(grid->getNumX()/2.0), 
		(int)(grid->getNumY()/2.0),
		(int)(grid->getNumZ()/2.0));
	for(int k = 0; k < grid->getNumZ(); k++){
		for(int j = 0; j < grid->getNumY(); j++){
			for(int i = 0; i < grid->getNumX(); i++){
				Vector gridPos = Vector(i, j, k);
				float averageDelta = (grid->getDX() + grid->getDY() + grid->getDZ())/3.0;
				Vector worldPos = (gridPos * averageDelta) + grid->getLlc();
				float comp = (worldPos - ((pCenter*averageDelta)+grid->getLlc())).magnitude()/(pScale);
				if(comp >= 1){
					grid->setValueAt(gridPos, 0.0);
				}
				else if(comp <= 0){
					grid->setValueAt(gridPos, 1.0);
				}
				else{
					float factor = (1 - comp)/(1 - boundary);
					grid->setValueAt(gridPos, 
						std::max(grid->getValueAt(gridPos),
						fractalSumPerlinNoise(gridPos * averageDelta) * factor));
				}
			}
		}
	}
}

float StampedNoise::eval(const Vector &x){
//	std::cout << grid->eval(x) << std::endl;
	return grid->eval(x);
	
}

Vector StampedNoise::grad(const Vector& x){
	return Vector(0.0, 0.0, 0.0);
}
