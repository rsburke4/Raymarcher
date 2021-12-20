#include <omp.h>
#include "DeepShadowMap.h"
#include "Construction.h"

using namespace std;

DeepShadowMap::DeepShadowMap(ScalarField* d, ColorField* c, Light* l, lux::Vector corner, float delta, int n, float s, float k){
	cout << "Inside constructor\n";
	dense = d;
	color = c;
	light = l;
	field = nullptr;

	numX = n;
	numY = n;
	numZ = n;
	initPointsAs(0.0);

	deltaX = delta;
	deltaY = delta;
	deltaZ = delta;
	llc = corner;
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
			//#pragma omp parallel for
			for(int i = 0; i < gridSize; i++){
				lux::Vector ijk = lux::Vector(i, j, k);
				lux::Vector worldIJK = (ijk * gridDelta) + llc;
				lux::Vector directionToLight = light->getPosition() - worldIJK;
				lux::Vector ray = worldIJK;
				lux::Color result = lux::Color(0.0, 0.0, 0.0, 1.0);


				float S = 0.0;
				float T = 1.0;
				float densities = 0.0;
				//Ray march from point until reach light
				//Only storing the T value
				while(S < directionToLight.magnitude()){
				//Check to see if hit something at this point
					if(S == 0.0 && dense->eval(ray) <= 0){
						break;
					}
					ray += marchRes * directionToLight.unitvector();
					float newDense = dense->eval(ray);
					if(newDense > 0.0) densities += dense->eval(ray) * marchRes;



					S += marchRes;
				}
				setValueAt(ijk, densities);
			}
		}
	}

}
