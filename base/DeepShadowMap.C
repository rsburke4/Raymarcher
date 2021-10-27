#include <omp.h>
#include "DeepShadowMap.h"
#include "Construction.h"

using namespace std;

DeepShadowMap::DeepShadowMap(ScalarField* d, ColorField* c, Light* l, lux::Vector corner, float delta, int n, float s, float k){
	cout << "Inside constructor\n";
	dense = d;
	color = c;
	light = l;
//	grid = new ScalarGrid(nullptr, corner, delta, n);
	field = nullptr;

	numX = n;
	numY = n;
	numZ = n;
	initPointsAs(0.0);

	deltaX = delta;
	deltaY = delta;
	deltaZ = delta;
	llc = corner;
	//grid->initPointsAs(0.0);
	gridSize = n;
	gridDelta = delta;
	marchRes = s;
	scatter = k;
	initGrid();
}

void DeepShadowMap::initGrid(){
	for(int k = 0; k < gridSize; k++){
		cout << "ShadowMapK: " << k << endl;
		for(int j = 0; j < gridSize; j++){
			#pragma omp parallel for
			for(int i = 0; i < gridSize; i++){
			//	std::cout << i << " " << j << " " << k << std::endl;
				lux::Vector ijk = lux::Vector(i, j, k);
				lux::Vector worldIJK = (ijk * gridDelta) + llc;
				lux::Vector directionToLight = light->getPosition() - worldIJK;
				lux::Vector ray = worldIJK;
				lux::Color result = lux::Color(0.0, 0.0, 0.0, 1.0);

				float S = 0.0;
				float T = 1.0;
				float densities = 0.0;
				//Ray march from point until you reach light
				//We're only storing the T value
				while(S < directionToLight.magnitude()){
				//Check to see if we even hit something at this point
					if(S == 0.0 && dense->eval(ray) <= 0){
						break;
					}
				//	cout << "RAY: " << ray.X() << " " << ray.Y() << ray.Z() << endl;
					ray += marchRes * directionToLight.unitvector();
				//	cout << "NEWRAY: " << ray.X() << " " << ray.Y() << ray.Z() << endl;
				//	cout << "Dense: " << dense->eval(ray);
					densities += dense->eval(ray);


				//	float deltaT = exp(-scatter * marchRes * dense->eval(ray));
				//	T *= deltaT;

					S += marchRes;
				//	ray += marchRes * directionToLight.unitvector();

				}
//				T = exp(-scatter * densities);
/*	if(densities > 0){
	cout << "worldIJK: " << worldIJK.X() << " " << worldIJK.Y() << " " << worldIJK.Z() << endl;
	cout << "Densities: " << densities << endl;
	cout << "T: " << T << endl;
}*/
				setValueAt(ijk, densities);
			}
		}
	}

}
