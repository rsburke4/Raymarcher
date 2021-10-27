#include <omp.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <queue>
#include "LevelSet.h"

using namespace std;

LevelSet::LevelSet(string filename, int band, lux::Vector corner, float d, int n){
//	obj = objl::Loader();
//	obj.LoadFile(filename);
	//objl::Mesh mesh = obj.LoadedMeshes[0];

	OBJ_Loader obj = OBJ_Loader(filename);

	set = new ScalarGrid(nullptr, corner, d, n);
	set->initPointsAsInf();

	float distU, distV, distUPlusV;
	queue<lux::Vector> validPoints;

	//Load points into data structures we actually like
	for(unsigned int i = 0; i < obj.Vertices.size(); i++){
		float x = obj.Vertices[i].X();
		float y = obj.Vertices[i].Y();
		float z = obj.Vertices[i].Z();
		points.push_back(lux::Vector(x, y, z));
		cout << "Vert0: " << points[0].X() << " " << points[0].Y() << " " << points[0].Z() << endl;
	}

	//Load triangles into a vector we like
	for(unsigned int i = 0; i < obj.Indices.size(); i+=3){
		vector<int> triangle;
		triangle.push_back(obj.Indices[i]);
		triangle.push_back(obj.Indices[i+1]);
		triangle.push_back(obj.Indices[i+2]);

	//	cout << triangle[0] << " " << triangle[1] << " " << triangle[2] << endl;
		triangles.push_back(triangle);
	}


	//Make bounding boxes for each triangle that extend
	//An additional bandwidth amount
	for(unsigned itt = 0; itt < triangles.size(); itt++){
		lux::Vector pointA = points[triangles[itt][0]-1];
		lux::Vector pointB = points[triangles[itt][1]-1];
		lux::Vector pointC = points[triangles[itt][2]-1];


		lux::Vector pointAijk = lux::Vector(
			int((pointA.X()-corner.X())/d),
			int((pointA.Y()-corner.Y())/d),
			int((pointA.Z()-corner.Z())/d));

		lux::Vector pointBijk = lux::Vector(
			int((pointB.X()-corner.X())/d),
			int((pointB.Y()-corner.Y())/d),
			int((pointB.Z()-corner.Z())/d));

		lux::Vector pointCijk = lux::Vector(
			int((pointC.X()-corner.X())/d),
			int((pointC.Y()-corner.Y())/d),
			int((pointC.Z()-corner.Z())/d));

		//Transform triangle points in world space
		// to grid points in space ranging from 0 to 1;
	//	pointA = (pointA + corner) * d;

cout << "PointA: " << pointA.X() << " ";
cout << pointA.Y() << " ";
cout << pointA.Z() << " \n";

cout << "PointAijk: " << pointAijk.X() << " ";
cout << pointAijk.Y() << " ";
cout << pointAijk.Z() << " \n";



	//	pointB = (pointB + corner) * d;
	//	pointC = (pointC + corner) * d;

		lux::Vector edge1 = pointB - pointA;
		lux::Vector edge2 = pointC - pointA;
		lux::Vector edge3 = pointC - pointB;
		lux::Vector normal = edge1 ^ edge2;
		normal /= normal.magnitude();


		float maxX = max(max(pointAijk.X(), pointBijk.X()), pointCijk.X());
		float maxY = max(max(pointAijk.Y(), pointBijk.Y()), pointCijk.Y());
		float maxZ = max(max(pointAijk.Z(), pointBijk.Z()), pointCijk.Z());

		float minX = min(min(pointAijk.X(), pointBijk.X()), pointCijk.X());
		float minY = min(min(pointAijk.Y(), pointBijk.Y()), pointCijk.Y());
		float minZ = min(min(pointAijk.Z(), pointBijk.Z()), pointCijk.Z());

		lux::Vector llc = lux::Vector(minX-band, minY-band, minZ-band);
		lux::Vector urc = lux::Vector(maxX+band, maxY+band, maxZ+band);

		//	cout << "llc: " << llc.X() << " " << llc.Y() << " " << llc.Z() << endl;
		//	cout << "urc: " << urc.X() << " " << urc.Y() << " " << urc.Z() << endl;

		//Loop over all grid points contained in this box
		//And calculculate accurate distance from point to triangle
		for(int k = llc.Z(); k < urc.Z(); k++){
			cout << "K: " << k << endl;
			for(int j = llc.Y(); j < urc.Y(); j++){
				for(int i = llc.X(); i < urc.X(); i++){
//cout<< "Triangle5: " << triangles[10][0] << " " << triangles[10][1] << " " << triangles[10][2] << endl;
					if(i >= 0 && j >= 0 && k >= 0){
						if(i < n && j < n && k < n){
						float dist = pow(10, 6);
						lux::Vector ijk = lux::Vector(i, j, k);
						lux::Vector worldIJK = (ijk * d) + corner;
	
						//Find the UV Coords
						float U = (edge2 ^ (worldIJK - pointA)) * (edge2 ^ edge1);
						U /= pow((edge2 ^ edge1).magnitude(), 2);
						float V = (edge1 ^ (worldIJK - pointA)) * (edge1 ^ edge2);
						V /= pow((edge2 ^ edge1).magnitude(), 2);

					//	cout << "U: " << U << endl;
					//	cout << "V: " << V << endl;

						//Special case if point maps onto triangle
						if(U >= 0 && V >= 0 && U+V >= 0 && U <= 1 && V <= 1 &&  U+V <= 1){
							dist = signedDistance(worldIJK, normal, pointA);
						//	cout << "IN TRIANGLE" << endl;
						}
						//Otherwise distance is the closest to an edge
						else{
						//	float distU, distV, distUPlusV;
							if (U > 1){ distU = 1; }
							else if (U < 0){ distU = 0; }
							else{ distU = (-1 * edge1 * (pointA - worldIJK))/edge1.magnitude(); }

							
							if (V > 1){ distV = 1; }
							else if(V < 0){ distV = 0; }
							else{ distV = (-1 * edge2 * (pointA - worldIJK))/edge2.magnitude(); }
							
							if (U+V > 1){ distUPlusV = 1; }
							else if (U+V < 0){ distUPlusV = 0; }
							else{ distUPlusV = (-1 * edge3 * (pointB - worldIJK))/edge3.magnitude();}

							distU = ((pointA - worldIJK) + distU * edge1).magnitude();
							distV = ((pointA - worldIJK) + distV * edge2).magnitude();
							distUPlusV = ((pointB - worldIJK) + distUPlusV * edge3).magnitude();
							dist = min(min(distU, distV), distUPlusV);
						}
					//	cout << "Dist: " << dist << endl;

			
						//cout << "Setting Val " << i << " " << j << " " << k;
						set->setValueAt(ijk, min(dist, set->getValueAt(ijk)));
						//validPoints.push_back(ijk);
						if(set->getValueAt(ijk) < pow(10,6)){
							validPoints.push(ijk);
						}
						//cout << "Value at " << i << " " << j << " " << k;
						//cout << " is: " << set->getValueAt(ijk) << endl;
						} //End if check valid 1
					} // End if check valid 2 (Can combine wi)
				} // End For I
			} // End For J
		} //End for K
	} //End for triangle

	//Loop through the grid again and determine if points are inside,
	//outside, or if they remain the initial value.
	for(int k = 0; k < n; k++){
		cout<< "Grid K: " << k << endl;
		for(int j = 0; j < n; j++){
			#pragma omp parallel for
			for(int i = 0; i < n; i++){
				lux::Vector ijk = lux::Vector(i, j, k);
				lux::Vector worldIJK = (ijk * d) + corner;
				float value = set->getValueAt(ijk);

				
				//If initial value, outside band
			//	if(value pow(10, 5)){
			//		set->setValueAt(ijk, 0.0);
			//	}
				//Idealy, values are either changed, or initial.
			//	else{
					int intersections = 0;
					lux::Vector direction = lux::Vector(1, 0, 0);

					//Check for an intersection with a triangle
					for(unsigned int itt = 0; itt < triangles.size(); itt++){
						lux::Vector pointA = points[triangles[itt][0]-1];
						lux::Vector pointB = points[triangles[itt][1]-1];
						lux::Vector pointC = points[triangles[itt][2]-1];

//cout << "POINT A: " << pointA.X() << endl;

						lux::Vector edge1 = pointB - pointA;
						lux::Vector edge2 = pointC - pointA;
					//	lux::Vector edge3 = pointC - pointB;

						//Fire parallel rays
						float t = ((pointA - worldIJK) * (edge1 ^ edge2))/(direction * (edge1 ^ edge2));
						lux::Vector pointT = worldIJK + (direction * t);
					//	float planePoint = (pointT - pointA) * (edge1 ^ edge2);

					//	cout << "T: " << t << endl;
						//No Intersection. Outside band
					//	if(t < 0){
						//	set->setValueAt(ijk, 0.0);
							//This is unlikely, but whatever.
							//We'll ignore and continue for now.
							//continue;
					//	}
						//Otherwise, check that plane intersection is withinn triangle

						if(t >= 0){
							float U = (edge2 ^ (pointT - pointA)) * (edge2 ^ edge1);
							U /= pow((edge2 ^ edge1).magnitude(), 2);
							float V = (edge1 ^ (pointT - pointA)) * (edge1 ^ edge2);
							V /= pow((edge2 ^ edge1).magnitude(), 2);

							if(U>=0 && V>=0 && U<=1 && V<=1 && U+V<=1 && U+V>=0){
								intersections++;
							}
						//	cout << "U: " << U << endl;
						//	cout << "V: " << V << endl;
						}
					} //Triangle Loop
				if(intersections%2 == 1){
					set->setValueAt(ijk, abs(value));
				//	cout << "Odd intersections" << endl;
				//	set->setValueAt(ijk, 1.0);
				}
				else{
				//	cout << "Even intersections: " in
				/*	if(set->getValueAt(ijk) * -1 > 0 ){
						cout << "Here's a problem: " << set->getValueAt(ijk) * -1 << endl;
						cout << "At: " << ijk.X() << " " << ijk.Y() << " " << ijk.Z() << endl;
					}
					else{
						cout << "But this isn't\n";
					}*/
					set->setValueAt(ijk, -1 * abs(value));
				//	set->setValueAt(ijk, 0.0);
				}
			} // I Loop
		} // J Loop
	} // K Loop
	bandwidth = band;

	//Expand Level Set

	while(!validPoints.empty()){
	//	cout << "Attempting to get ijk\n";
		lux::Vector currentIJK = validPoints.front();
		lux::Vector up = currentIJK + lux::Vector(0, 1, 0);
		lux::Vector down = currentIJK + lux::Vector(0, -1, 0);
		lux::Vector left = currentIJK + lux::Vector(-1, 0, 0);
		lux::Vector right = currentIJK + lux::Vector(1, 0, 0);
		lux::Vector forward = currentIJK + lux::Vector(0, 0, 1);
		lux::Vector back = currentIJK + lux::Vector(0, 0, -1);

		vector<lux::Vector> neighbors = {up, down, left, right, forward, back};

		for(size_t n = 0; n < neighbors.size(); n++){
			//Check value, and subtract or add distance
			if(set->getValueAt(neighbors[n]) > pow(10,5) && set->isValidIndex(neighbors[n])){
				float newVal = set->getValueAt(currentIJK) + d;
				set->setValueAt(neighbors[n], newVal);
				validPoints.push(neighbors[n]);
			}
			if(set->getValueAt(neighbors[n]) < -1 * pow(10,5) && set->isValidIndex(neighbors[n])){
				float newVal = set->getValueAt(currentIJK) - d;
				set->setValueAt(neighbors[n], newVal);
				validPoints.push(neighbors[n]);
			}
			
		} 
		validPoints.pop();
	}
}

lux::Vector nearestPointTransform(const lux::Vector& y){
	
}

float LevelSet::signedDistance(lux::Vector ijk, lux::Vector n, lux::Vector point){
//	cout << "Dist Norm: " << n.X() << " " << n.Y() << " " << n.Z() << endl;
//	cout << "Dist Point: " << point.X() << " " << point.Y() << " " << point.Z() << endl;
	float dist = abs((ijk - point) * n);
//	cout << "Dist dist: " << dist << endl;
	return dist;
}

float LevelSet::eval(const lux::Vector& x){
//	if(x.X() >= 0 && x.Y() >= 0 && x.Z() >= 0){
//		cout << "Set Eval Vector: " << x.X() << " " << x.Y() << " " << x.Z() << endl;
//		cout << "LEVELSET EVAL: " << set->eval(x) << endl;
//	}
//	if(set->eval(x) > pow(10, 5)){
//		return -1;
//	}
//	else{
	//	cout << "This is not the initial value. This is: " << set->eval(x) << endl;
		return set->eval(x);
//	}
}
