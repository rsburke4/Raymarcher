#ifndef SPHERE_H
#define SPHERE_H

#include "Vector.h"
#include "ScalarField.h"

class Sphere:public ScalarField{
	public:
		Sphere(){radius = 1;}
		Sphere(float r){radius = r;}
		float eval(const lux::Vector& x){return radius - x.magnitude();}
		lux::Vector grad(const lux::Vector& x){
			float xmag = x.magnitude();
			return lux::Vector(x.X()/xmag, x.Y()/xmag, x.Z()/xmag);
			}

	protected:
		float radius;
};

#endif
