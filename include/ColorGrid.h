#ifndef COLORGRID_H
#define COLORGRID_h

#include <vector>
#include "ColorField.h"

class ColorGrid:public ColorField{
	public:
		ColorGrid(ColorField* sf, lux::Vector corner, float dx, float dy, float dz, int nx, int ny, int nz);
		ColorGrid(ColorField* sf, lux::Vector corner, float d, int n);

		lux::Color eval(const lux::Vector& x);
		lux::Matrix grad(const lux::Vector& x){ return lux::Matrix(); }

	private:
		void initPoints();
		lux::Color getValueAt(lux::Vector x);

		ColorField* field;

		std::vector<lux::Color> points;

		lux::Vector llc;
		float deltaX;
		float deltaY;
		float deltaZ;

		int numX;
		int numY;
		int numZ;

};

#endif
