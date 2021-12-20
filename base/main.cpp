#include <stdio.h>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include "Camera.h"
#include "ScalarField.h"
#include "Image.h"
//#include "Sphere.h"
#include "Shapes.h"
#include "Construction.h"
//#include "Icosahedron.h"
#include "Transforms.h"
//#include "OpenImageIO/imageio.h"
#include "Mask.h"
#include "ScalarGrid.h"
#include "ColorGrid.h"
#include "LevelSet.h"

bool writeImage(const char* filename, lux::Image image){
	//const char* filename = "test.exr";
	const int xres = image.Width();
	const int yres = image.Height();
	const int channels = 4;
	std::vector<float> imageData = image.getdata1d();
	std::cout << "Image 1D Array Prepped\n";
/*
	OIIO::ImageOutput* out = OIIO::ImageOutput::create(filename);
//	std::unique_ptr<OIIO::ImageOutput> out = OIIO::ImageOutput::create (filename);
	OIIO::ImageSpec spec(xres, yres, channels, OIIO::TypeDesc::FLOAT);

	spec.channelformats.push_back(OIIO::TypeDesc::FLOAT);
	spec.channelformats.push_back(OIIO::TypeDesc::FLOAT);
	spec.channelformats.push_back(OIIO::TypeDesc::FLOAT);
	spec.channelformats.push_back(OIIO::TypeDesc::FLOAT);

	spec.channelnames.clear();
	spec.channelnames.push_back("R");
	spec.channelnames.push_back("G");
	spec.channelnames.push_back("B");
	spec.channelnames.push_back("A");
	
	if (out){
		out->open(filename, spec);
		std::cout << "File Opened\n";
		out->write_image(OIIO::TypeDesc::FLOAT, (void*)&imageData[0]);
		out->close();
		return true;
	}*/
	ofstream outputFile;
	outputFile.open(filename);
	for(int i = 0; i < imageData.size(); i++){
		outputFile << imageData[i] << "\n";
	}
	outputFile.close();
	return true;
	//return false;
}

int main(int argc, char *argv[]){
	//Setup camera
	Camera camera = Camera();
	camera.setEyeViewUp(
		lux::Vector(0, 0, -5),
		lux::Vector(0, 0, 1),
		lux::Vector(0, -1, 0));
	camera.setNearPlane(0.0);
	camera.setFarPlane(100.0);

	//Add objects to scene
	int imageHeight, rangeA, rangeB, gridSize, input;
	float stepSize, gridRes, kappa;
	string prefix;

	string params = argv[1];
	rangeA = atoi(params.substr(0, params.find("-")).c_str());
	rangeB = atoi(params.substr(params.find("-")+1, params.find(":")).c_str());
	params.erase(0, params.find(":")+1);
	input = atoi(params.substr(0, params.find(":")).c_str());
	params.erase(0, params.find(":")+1);
	imageHeight = atoi(params.substr(0, params.find(":")).c_str());
	params.erase(0, params.find(":")+1);
	prefix = params;
	
	//rangeA = atoi(rangeA_string);
	//rangeB = atoi(rangeB_string);
	std::cout << rangeA << endl;
	std::cout << rangeB << endl;
	std::cout << imageHeight << endl;
	std::cout << prefix << endl;
	//int input = 1;
	gridSize = 500;
	stepSize = 0.001;
	kappa = 0.5;

	//string range = 
	//Get input parameters
/*	std::cout << "Input Start Frame" << std::endl;
	std::cin >> rangeA;
	std::cout << "Input End Frame" << std::endl;
	std::cin >> rangeB;
	std::cout << "Input Image Height" << std::endl;
	std::cin >> imageHeight;
	std::cout << "Input Step Size" << std::endl;
	std::cin >> stepSize;
	std::cout << "Input File Prefix" << std::endl;
	std::cin >> prefix;
	std::cout << "Input Grid Size" << std::endl;
	std::cin >> gridSize;
	std::cout << "Input Kappa" << std::endl;
	std::cin >> kappa;

	int input;
	cin >> input;
*/

	Light light1 = Light(lux::Vector(3, 2, 5), lux::Color(1.0, 0.9, 0.8, 1.0), 0.3);
	Light light2 = Light(lux::Vector(-3, 1, 5), lux::Color(0.8, 0.9, 1.0, 1.0), 0.3);
	Light light3 = Light(lux::Vector(3, 3, -5), lux::Color(1.0, 1.0, 1.0, 1.0), 0.1);

	DeepShadowMap key;
	DeepShadowMap fill;
	DeepShadowMap rim;

	int frameNumber = rangeA;
//Delete Next line for Pyroclasts


	float noiseSpeedX = 0.05;
	float noiseSpeedY = 0.05;
	float noiseSpeedZ = 0.02;

	if(input == 1){


for(int i = rangeA; i <= rangeB; i++){
/*for(int j = 1; j <= 14; j++){
for(int k = 1; k <= 14; k++){
for(int a = 1; a <= 14; a++){
for(int o = 1; o <= 5; o++){*/
		frameNumber = i;
		/*PyroclasticSphere pyro = PyroclasticSphere(
			1.0,
			1.689958,
			0.39,
			5,
			2.511764,
			1.689958,
			lux::Vector(0.0, noiseSpeedY * frameNumber, noiseSpeedZ * frameNumber)
		);*/
		PyroclasticSphere pyro = PyroclasticSphere(
			1.0,
			3.0*(float(frameNumber%100)/100.0) + 0.1,
			2.0*(float(frameNumber%200)/200.0) + 0.1,
			2*int(frameNumber%500/200) + 1,
			3.0*(float(frameNumber%153)/153.0) + 0.1,
			2.0*(float(frameNumber%239)/239.0) + 0.1,
			lux::Vector(0.0, noiseSpeedY * frameNumber, noiseSpeedZ * frameNumber)
		);
		//Sphere pyro = Sphere(1);
		std::cout << "Created Mask\n";
		Mask mask = Mask(&pyro);
		//Clamp mask = Clamp(&pyro, 0, 1, 0.00005);
		DensityField denseField = DensityField(&pyro, 100.0);
		ColorField colorField = ColorField(lux::Color(1.0, 1.0, 1.0, 1.0), &mask);

		cout << "Making map 1\n";
		DeepShadowMap key = DeepShadowMap(&denseField, &colorField, &light1, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Making map 2\n";
		DeepShadowMap fill = DeepShadowMap(&denseField, &colorField, &light2, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Making map 3\n";
		DeepShadowMap rim = DeepShadowMap(&denseField, &colorField, &light3, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Finished building map";
		std::vector<DeepShadowMap*> shadowMaps;
		shadowMaps.push_back(&key);
		shadowMaps.push_back(&fill);
		shadowMaps.push_back(&rim);


		float interp = ((float)1.0/121.0)*2.0 * M_PI;
		lux::Vector newEyePos = lux::Vector(sin(interp)*5, 2, cos(interp)*5);
		lux::Vector newView = lux::Vector(0, 0, 0) - newEyePos;
		lux::Vector newUp = lux::Vector(0, -1, 0);
		camera.setEyeViewUp(
				newEyePos,
				newView,
				newUp);
		lux::Image image = camera.render(imageHeight, stepSize, &colorField, &denseField, shadowMaps, kappa);

		int decimal = std::to_string(frameNumber).length();
		string frameString = std::string(4-decimal, '0') + std::to_string(frameNumber);
		std::cout << frameString << endl;
		std::string str = "/scratch1/rsburke/pyro/" + prefix + "_" + frameString + ".exr";
		const char* filename = str.c_str();
		writeImage(filename, image);
		frameNumber++;
}
}

//And here we have wisps


	if(input == 2){
	float pulseSpeed = 0.01;
	float pulseAmp = 0.1;
for(int i = rangeA; i <= rangeB; i++){
/*for(int j = 1; j <= 14; j++){
for(int k = 1; k <= 14; k++){
for(int a = 1; a <= 14; a++){
for(int o = 1; o <= 5; o++){*/

		frameNumber = i;	

/*		Wisp wisp = Wisp(1.0, 3, Vector(0, 0, 0), 1.0*(float(frameNumber%200/200.0)) + 0.5, 10000000, 1.0f/3.0f, Vector(-2, -2, -2), 0.008, 500);
		wisp.setNoiseParams(
			3.0*float(frameNumber%100/100.0) + 0.1,
			2.0*float(frameNumber%200/200.0) + 0.1,
			2*int(frameNumber%500/200) + 1,
			3.0*float(frameNumber%153/153.0) + 0.1,
			2.0*float(frameNumber%239/239.0) + 0.1,
			Vector(0.0, 0.0, 0.0));
*/
		Wisp wisp = Wisp(
			1.0,
		       	3, 
			Vector(0, 0, 0), 
			1.135 + (sin(frameNumber * pulseSpeed) * pulseAmp), 
			10000000, 
			1.0f/3.0f, 
			Vector(-2, -2, -2), 
			0.008, 
			500);
		wisp.setNoiseParams(
			0.97,
			1.37,
			1,
			2.590196,
			1.162761,
			Vector(0.0, frameNumber * noiseSpeedY, frameNumber * noiseSpeedZ)
		);
		wisp.init();

		Clamp denseField = Clamp(&wisp, -0.8, 10, 0.05);
		Clamp mask = Clamp(&wisp, -0.8, 1, 0.05);
		//Mask mask = Mask(&denseField);
		//denseField = DensityField(&mask, 1.0);
		ColorField colorField = ColorField(lux::Color(1.0, 1.0, 1.0, 1.0), &mask);

		cout << "Making map 1\n";
		//DeepShadowMap key = DeepShadowMap(&denseField, &colorField, &light1, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Making map 2\n";
		//DepShadowMap fill = DeepShadowMap(&denseField, &colorField, &light2, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Making map 3\n";
		//DeepShadowMap rim = DeepShadowMap(&denseField, &colorField, &light3, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Finished building map";
		std::vector<DeepShadowMap*> shadowMaps;
		//shadowMaps.push_back(&key);
		//shadowMaps.push_back(&fill);
		shadowMaps.push_back(&rim);


		float interp = ((float)1.0/121.0)*2.0 * M_PI;
		lux::Vector newEyePos = lux::Vector(sin(interp)*5, 2, cos(interp)*5);
		lux::Vector newView = lux::Vector(0, 0, 0) - newEyePos;
		lux::Vector newUp = lux::Vector(0, -1, 0);
		camera.setEyeViewUp(
				newEyePos,
				newView,
				newUp);
		lux::Image image = camera.render(imageHeight, stepSize, &colorField, &denseField, shadowMaps, kappa);
		int decimal = std::to_string(frameNumber).length();
		string frameString = std::string(4-decimal, '0') + std::to_string(frameNumber);
		std::cout << frameString << endl;
		std::string str = "/scratch1/rsburke/pyro/" + prefix + "_" + frameString + ".exr";
		const char* filename = str.c_str();
		writeImage(filename, image);
		frameNumber++;

}
}







//Finally we have stamped noise
	if(input == 3){

for(int i = rangeA; i <= rangeB; i++){

		StampedNoise denseField = StampedNoise(1.0, 0.8, 0.4, Vector(-2, -2, -2), 0.008, 500);

		/*denseField.setNoiseParams(
				3.0*(float(frameNumber%100)/100.0) + 0.1,
				1.0*(float(frameNumber%200)/200) + 0.1,
				2*(int(frameNumber%500)/200) + 1,
				3.0*(float(frameNumber%153)/153.0) + 0.1,
				2.0*(float(frameNumber&239)/239.0) + 0.1,
				Vector(0.0, 0.0, 0.0));
*/
		denseField.setNoiseParams(
			1.66,
			0.62,
			5,
			2.96274,
			1.8824,
			Vector(0.0, frameNumber * noiseSpeedY, frameNumber * noiseSpeedZ)
		);
		Clamp mask = Clamp(&denseField, -0.2, 1, 0.05);
		//DensityField densityField = DensityField(&mask, 1.0);
		ColorField colorField = ColorField(lux::Color(1.0, 1.0, 1.0, 1.0), &mask);

		cout << "Making map 1\n";
		//DeepShadowMap key = DeepShadowMap(&denseField, &colorField, &light1, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Making map 2\n";
		//DeepShadowMap fill = DeepShadowMap(&denseField, &colorField, &light2, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Making map 3\n";
		//DeepShadowMap rim = DeepShadowMap(&denseField, &colorField, &light3, lux::Vector(-2, -2, -2), 0.008, 500, 0.01, kappa);
		cout << "Finished building map";
		std::vector<DeepShadowMap*> shadowMaps;
		//shadowMaps.push_back(&key);
		//shadowMaps.push_back(&fill);
		shadowMaps.push_back(&rim);


		float interp = ((float)1.0/121.0)*2.0 * M_PI;
		lux::Vector newEyePos = lux::Vector(sin(interp)*5, 2, cos(interp)*5);
		lux::Vector newView = lux::Vector(0, 0, 0) - newEyePos;
		lux::Vector newUp = lux::Vector(0, -1, 0);
		camera.setEyeViewUp(
				newEyePos,
				newView,
				newUp);
		lux::Image image = camera.render(imageHeight, stepSize, &colorField, &denseField, shadowMaps, kappa);
		/*std::string str = "/scratch1/rsburke/pyro/" + prefix + "_" + std::to_string(frameNumber) + ".log";
		const char* filename = str.c_str();
		writeImage(filename, image);
		frameNumber++;*/

		int decimal = std::to_string(frameNumber).length();
		string frameString = std::string(4-decimal, '0') + std::to_string(frameNumber);
		std::cout << frameString << endl;
		std::string str = "/scratch1/rsburke/pyro/" + prefix + "_" + frameString + ".exr";
		const char* filename = str.c_str();
		writeImage(filename, image);
		frameNumber++;
	}
}

	//std::cout << "Created Desnsity and Colors\n";

//std::cout << "Main.c eval: " << denseField->eval(lux::Vector(-1, -1, -1)) << std::endl;

        if(input == 4){
for(int i = rangeA; i <= rangeB; i++){
	Mask maskOne;
        LevelSet levelSet = LevelSet("models/cleanbunny.obj", 30, lux::Vector(-2, -2, -2), 0.02, 200);
	cout << "Made Set" << endl;
	Sphere sphere = Sphere();
	PyroDisplace pyro, pyro2;
	//Spins
	if(i >= 100 && i <= 300){
		pyro = PyroDisplace(&levelSet, 10.0, 0.8, 3, 0.4, 0.3, Vector(0, 0, 0));
		pyro2 = PyroDisplace(&pyro, 20.0, 0.8, 3, 0.2, 0.2, Vector(-1, 0.2, -8.8));
	}
	//Fluffyfy
	else if(i < 100){
		pyro = PyroDisplace(&levelSet, 10.0, 0.8, 3, 0.4, 0.3 * ((float)i/100.0), Vector(0, 0, 0));
		pyro2 = PyroDisplace(&pyro, 20.0, 0.8, 3, 0.2, 0.2 * ((float)i/100.0), Vector(-1, 0.2, -8.8));
	}
	//Translate
	else if(i > 300 && i < 400){	
		pyro = PyroDisplace(&levelSet, 10.0, 0.8, 3, 0.4, 0.3, Vector(0, ((float)i-300.0)*noiseSpeedY, ((float)i-300.0)*noiseSpeedZ));
		pyro2= PyroDisplace(&pyro, 20.0, 0.8, 3, 0.2, 0.2, Vector(0, ((float)i-300.0)*(-noiseSpeedY), ((float)i-300.0)*(-noiseSpeedZ)));
	}
	else if(i >= 400){	
		pyro = PyroDisplace(&levelSet, 10.0, 0.8, 3, 0.4, 0.3 * ((500.0-(float)i)/100.0), Vector(0, 100.0*noiseSpeedY, 100.0*noiseSpeedZ));
		pyro2 = PyroDisplace(&pyro, 20.0, 0.8, 3, 0.2, 0.2 * ((500.0-(float)i)/100.0), Vector(0, 100.0*(-noiseSpeedY), 100.0*(-noiseSpeedZ)));
	}
	ScalarGrid grid = ScalarGrid(&pyro2, lux::Vector(-2, -2, -2), 0.01, 400);
	grid.initPointsFromField();
       //Sphere levelSet = Sphere(1);
        std::cout << "Read in the file\n";
        Mask levelMask = Mask(&grid);
        maskOne = Mask(&sphere);
        Clamp levelClamp = Clamp(&grid, 0.0, 1.0, 0.005);

        std::cout << "Created Mask\n";
        DensityField denseField = DensityField(&levelClamp, 100.0);
        //Clamp clampSet = Clamp(&denseField, 0, 3, 1.8);
//      DensityField denseField = DensityField(&density, 1.0);
        ColorField colorField = ColorField(lux::Color(1.0, 1.0, 1.0, 1.0), &levelMask);

        std::cout << "Created Desnsity and Colors\n";

        std::cout << "Main.c eval: " << denseField.eval(lux::Vector(0.5, 0.5, 0.5)) << std::endl;

                cout << "Making map 1\n";
                DeepShadowMap key = DeepShadowMap(&denseField, &colorField, &light1, lux::Vector(-2, -2, -2), 0.008, 500, 0.001, kappa);
                cout << "Making map 2\n";
                DeepShadowMap fill = DeepShadowMap(&denseField, &colorField, &light2, lux::Vector(-2, -2, -2), 0.008, 500, 0.001, kappa);
                cout << "Making map 3\n";
                DeepShadowMap rim = DeepShadowMap(&denseField, &colorField, &light3, lux::Vector(-2, -2, -2), 0.008, 500, 0.001, kappa);

                std::vector<DeepShadowMap*> shadowMaps;
                shadowMaps.push_back(&key);
		shadowMaps.push_back(&fill);
                shadowMaps.push_back(&rim);


		float interp = 0.0;
		if(i >= 100 && i <= 300){
			interp = (((float)i-100.0)/200.0)*2.0 * M_PI;
		}	
		lux::Vector newEyePos = lux::Vector(sin(interp)*5, 2, cos(interp)*5);

		lux::Vector newView = lux::Vector(0, 0, 0) - newEyePos;
		lux::Vector newUp = lux::Vector(0, -1, 0);
		camera.setEyeViewUp(
				newEyePos,
				newView,
				newUp);
		lux::Image image = camera.render(imageHeight, stepSize, &colorField, &denseField, shadowMaps, kappa);
		/*std::string str = "/scratch1/rsburke/pyro/" + prefix + "_" + std::to_string(frameNumber) + ".log";
		const char* filename = str.c_str();
		writeImage(filename, image);
		frameNumber++;*/

		int decimal = std::to_string(frameNumber).length();
		string frameString = std::string(4-decimal, '0') + std::to_string(frameNumber);
		std::cout << frameString << endl;
		std::string str = "/scratch1/rsburke/pyro/" + prefix + "_" + frameString + ".exr";
		const char* filename = str.c_str();
		writeImage(filename, image);
		frameNumber++;
}	
}	

	printf("Ran successfully\n");
	return 0;
}
