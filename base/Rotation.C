#include "Rotation.h"


lux::Matrix Rotation::createRotationMatrix(float angle, lux::Vector axis){
	lux::Matrix r = lux::Matrix::unitMatrix();
	if(axis.magnitude() < 0.0000001){
		return r;
	}
	
	lux::Vector unit = axis.unitvector();
	lux::Matrix op = lux::Matrix::outerProduct(unit, unit);
	float sinangle = sin(angle);
	float cosangle = cos(angle);

	r *= cosangle;
	r += op * (1 - cosangle);
	r += lux::Matrix::Pauli0() * axis.X() * sinangle;
	r += lux::Matrix::Pauli1() * axis.Y() * sinangle;
	r += lux::Matrix::Pauli2() * axis.Z() * sinangle;

	return r;
}
