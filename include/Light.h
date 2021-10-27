#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "ScalarGrid.h"
#include "Vector.h"
#include "Color.h"

class Light{
	public:
		Light(lux::Vector p, lux::Color c, float b){
			position = p;
			color = c;
			brightness = b;
		}
		lux::Vector getPosition(){ return position; }
		float getBrightness(){ return brightness; }
		lux::Color getColor(){ return color; }

	private:
		lux::Color color;
		lux::Vector position;
		float brightness;
};

#endif
