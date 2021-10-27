#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "OBJ_Loader.h"

using namespace std;


OBJ_Loader::OBJ_Loader(string filename){
	ifstream file;
	file.open(filename.c_str());
	string line;

	if(file.is_open()){
		while(getline(file, line)){
			if(line.front() == 'v'){
				size_t pos = 0;
				string x = line.substr(line.find(" "));
				string y = x.substr(x.find(" ", 1));
				string z = y.substr(y.find(" ", 1));
				Vertices.push_back(lux::Vector(stof(x), stof(y), stof(z)));
			}
			if(line.front() == 'f'){
				line = line.substr(1);
				while(line.find(" ") != string::npos){
					int number = stoi(line);
					cout << number << endl;
					Indices.push_back(number);
					line = line.substr(1);
					if(line.find(" ") != string::npos){
						line = line.substr(line.find(" "));
					}
				}
			}
		}
		file.close();
	}
}
