#include <cmath>
#include <iostream>
#include "Wisp.h"

using namespace lux;

Wisp::Wisp(
float dens,
int dots,
Vector center, 
float pScl, 
long pNum, 
float clmp, 
Vector llc, 
float d, 
int n){
	density = dens;
	pCenter = center;
	pScale = pScl;
	points = pNum;
	clump = clmp;
	wispDots = dots;

	grid = new ScalarGrid(nullptr, llc, d, n);
	grid->initPointsAs(0.0);

	noise = PerlinNoise(1);
	twister = std::mt19937();
	twister.seed(rand());

	octaves = 0;
	frequency = 0;
	fJump = 0;
	amplitude = 0;
	roughness = 0;
	translation = Vector();

}

void Wisp::setNoiseParams(float f, float r, int o, float fj, float a, Vector trans){
	frequency = f;
	amplitude = a;
	octaves = o;
	fJump = fj;
	translation = trans;
	roughness = r;
}

float Wisp::fractalSumPerlinNoise(const Vector& x){
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


void Wisp::init(){
	Vector worldPoint = Vector();
	float twisterHalf = (float)twister.max()/2.0f;
	float twisterMax = twister.max();
	std::cout << ((float)twister() - twisterHalf)/twisterMax << std::endl;
	//Makes multiple wisp dots
	for(int j=0; j < wispDots; j++){
//std::cout << "FirstFor\n";
		//noise = PerlinNoise();
		roughness += (rand()%200-100)/100;
		fJump += (rand()%200-100)/100;
		translation += Vector((rand()%200-100)/10, (rand()%200-100)/10, (rand()%200-100)/10);
		frequency += (rand()%200-100)/100;
		//Makes one wisp dot
		for(long i=0; i < points; i++){
//std::cout << "second for\n";
			Vector randomPoint = Vector(
				((float)twister()-twisterHalf)/twisterMax, 
				((float)twister()-twisterHalf)/twisterMax, 
				((float)twister()-twisterHalf)/twisterMax);
			Vector sphereProjection = randomPoint.unitvector();
			float radius = pow(fabs(fractalSumPerlinNoise(sphereProjection)), clump);
			sphereProjection *= radius;
//			std::cout << "Rand: " << randomPoint.X() << std::endl;
			worldPoint = sphereProjection * pScale + pCenter;
			Vector displacement = Vector(
			fractalSumPerlinNoise(Vector(worldPoint.X(), worldPoint.Y(), worldPoint.Z())),
			fractalSumPerlinNoise(Vector(worldPoint.X()+0.1, worldPoint.Y()+0.1, worldPoint.Z()+0.1)),
			fractalSumPerlinNoise(Vector(worldPoint.X()-0.1, worldPoint.Y()-0.1, worldPoint.Z()-0.1)));
			worldPoint = worldPoint + displacement;	

			//Bake
//			std::cout << "World: " << worldPoint.X() << std::endl;
//			std::cout << "LLC: " << grid->getLlc().X() << std::endl;
			Vector relativePoint = worldPoint - grid->getLlc();
			//Ensure point is in the grid
			if(relativePoint.X() >= 0 && relativePoint.Y() >= 0 && relativePoint.Z() >= 0){
				float Lx = grid->getNumX() * grid->getDX();
				float Ly = grid->getNumY() * grid->getDY();
				float Lz = grid->getNumZ() * grid->getDZ();
				if(relativePoint.X() < Lx && relativePoint.Y() < Ly && relativePoint.Z() < Lz){
					//If it is, bake it at an index in the grid
					float gridPosX = (relativePoint.X()/Lx) * (grid->getNumX() - 1);
					float gridPosY = (relativePoint.Y()/Lx) * (grid->getNumY() - 1);
					float gridPosZ = (relativePoint.Z()/Lx) * (grid->getNumZ() - 1);
	
					int indexI = gridPosX;
					int indexJ = gridPosY;
					int indexK = gridPosZ;

					float weightI = indexI - gridPosX;
					float weightJ = indexJ - gridPosY;
					float weightK = indexK - gridPosZ;




					//Begin the setting (baking) process
					Vector indexVector = Vector(indexI, indexJ, indexK);
					float newVal = 0.0;

					float Gijk = grid->getValueAt(indexVector);
					newVal = density * (1 - weightI) * (1 - weightJ) * (1 - weightK) + Gijk;
//std::cout << newVal << std::endl;
//std::cout << indexVector.X() << " " << indexVector.Y() << " " << indexVector.Z() << std::endl;
					grid->setValueAt(indexVector, newVal);

					Gijk = grid->getValueAt(indexVector + Vector(1, 0, 0));
					newVal = density * weightI * (1 - weightJ) * (1 - weightK) + Gijk;
					grid->setValueAt(indexVector + Vector(1, 0, 0), newVal);
	
					Gijk = grid->getValueAt(indexVector + Vector(0, 1, 0));
					newVal = density * (1 - weightI) *  weightJ * (1 - weightK) + Gijk;
					grid->setValueAt(indexVector + Vector(0, 1, 0), newVal);
	
					Gijk = grid->getValueAt(indexVector + Vector(0, 0, 1));
					newVal = density * (1 - weightI) * (1 - weightJ) *  weightK + Gijk;
					grid->setValueAt(indexVector + Vector(0, 0, 1), newVal);

					Gijk = grid->getValueAt(indexVector + Vector(1, 1, 0));
					newVal = density * weightI * weightJ *  (1 - weightK) + Gijk;
					grid->setValueAt(indexVector + Vector(1, 1, 0), newVal);

					Gijk = grid->getValueAt(indexVector + Vector(0, 1, 1));
					newVal = density * (1 - weightI) * weightJ * weightK + Gijk;
					grid->setValueAt(indexVector + Vector(0, 1, 1), newVal);

					Gijk = grid->getValueAt(indexVector + Vector(1, 0, 1));
					newVal = density * weightI * (1 - weightJ) *  weightK + Gijk;
					grid->setValueAt(indexVector + Vector(1, 0, 1), newVal);

					Gijk = grid->getValueAt(indexVector + Vector(1, 1, 1));
					newVal = density * (1 - weightI) * weightJ *  weightK + Gijk;
					grid->setValueAt(indexVector + Vector(1, 1, 1), newVal);
				
				}
				else{ std::cout << "Failed check2: " << relativePoint.X() << " " << relativePoint.Y() << " " << relativePoint.Z() << std::endl; }
			}
			else{ std::cout << "Failed check1" << relativePoint.X() << " " << relativePoint.Y() << " " << relativePoint.Z() << std::endl; }
		}
	}
}


