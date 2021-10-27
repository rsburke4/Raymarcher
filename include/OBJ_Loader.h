// OBJ_Loader.h - A Single Header OBJ Model Loader
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include "Vector.h"

using namespace std;

class OBJ_Loader{
	public:
		OBJ_Loader(){}
		OBJ_Loader(string filename);

		vector<lux::Vector> Vertices;
		vector<unsigned int> Indices;		
};

#endif
