#include <omp.h>
#include <iostream>
#include "Camera.h"
using namespace lux;

Camera::Camera()
{
   setEyeViewUp( Vector(0,0,1), Vector(0,0,-1), Vector(0,1,0) );
   setFov( 60.0 );
   setAspectRatio( 16.0/9.0 );
   setNearPlane(0.0);
   setFarPlane(1.0e6);
}

void Camera::setEyeViewUp( const Vector& eye, const Vector& view, const Vector& up )
{
   position = eye;
   axis_view = view.unitvector();
   axis_up = ( up - (up*axis_view) * axis_view ).unitvector();
   axis_right = (axis_view^axis_up).unitvector();
}

// view direction of a pixel at the fractional position x,y.
// Nominally 0 <= x <= 1 and 0 <= y <= 1 for the primary fov,
// but the values can extend beyond that
const Vector Camera::view( const double x, const double y )
{
   double xx = (2.0*x-1.0)*htanfov;
   double yy = (2.0*y-1.0)*vtanfov;
   return (axis_up * yy + axis_right * xx + axis_view).unitvector();
}

void Camera::setFov( const double fov )
{
   FOV = fov;
   htanfov = tan( FOV*0.5*M_PI/180.0 );
   vtanfov = htanfov/aspect_ratio;
}

void Camera::setAspectRatio( const double ar )
{
   aspect_ratio = ar;
   vtanfov = htanfov/aspect_ratio;
}


Image Camera::render(
  const int height,
  float deltaS,
  ColorField* color,
  ScalarField* density,
  vector<DeepShadowMap*> lightGrids,
  float kappa)
{
  int width = height * aspect_ratio;
  Image image = Image();
  image.reset(width, height, 4);

  //Row
  for(int i=0; i < height; i++){
    //Col
    cout << "I: " << i << endl;
    #pragma omp parallel for
    for(int j=0; j < width; j++){
    //std::cout <<  j << std::endl; 
// std::cout << (float)((float)i * (float)width + (float)j)/(float)((float)height * (float)width) * 100.0f << std::endl;    
    lux::Color lp = lux::Color(0, 0, 0, 1);
    lux::Vector direction = view((float)j/(float)width, (float)i/(float)height);
    /*
    float U = 2/aspect_ratio * (i-(aspect_ratio/2))*getHTanFOV();
    float V = aspect_ratio/2 * (j-(2/aspect_ratio))*getVTanFOV();
    lux::Vector direction = eye() + (V*up()) + (U*right());*/
    //lux::Vector ray = ((direction - eye().unitvector())
    //  /(view() * (direction - eye()))) - view();
    lux::Vector ray = eye() + direction.unitvector() * near;
    //cout << "------------" << endl;
    //double temp = view() * (direction - eye());
    //cout << "Temp: " << temp << endl;
//    cout << "Eye: " << eye().X() << " " << eye().Y() << " " << eye().Z() << endl; 
//    cout << "Direction: "<< direction.X() << " " << direction.Y() << " " <<direction.Z() << endl;
//    cout << "Ray: " << ray.X() << " " <<ray.Y() << " " <<ray.Z() << endl;
//    cout << "U: " << U << endl;
//    cout << "V: " << V << endl;
//    cout << "I: " << i << " J: " << j << endl;
    //cout << "View: " << view().X() << " " << view().Y() << " " << view().Z();
    //cout << "axis_view: " << axis_view.X() << " " <<axis_view.Y() << " " << axis_view.Z() << endl;

    float T = 1.0;
    float S = 0.0;
      //Ray
//      cout << "NEw loop \n"; 
      while(S <= farPlane()){
	ray += deltaS * direction.unitvector();
	float deltaT = exp(-kappa*deltaS*density->eval(ray));
	//cout << "Dense: " << density->eval(ray) << endl;
	/*
	cout << "DeltaT: " << deltaT << endl;
	cout << "Kappa: " << kappa << endl;*/
	//cout << "I: " << i << " J: " << j << endl;
	//cout << "Direction: " << direction.X() << " " << direction.Y() << " " << direction.Z() << endl; 
	//cout << "View:  " << view(i/width, j/height).X() << " " << view(i/width, j/height).Y() << " " << view(i/width, j/height).Z() << endl;
	//cout << "Ray: " << ray.X() << " " << ray.Y() << " " << ray.Z() << endl;
	//cout << "mask.eval(ray): " << mask->eval(ray) << endl;
	for(size_t l = 0; l < lightGrids.size(); l++){
		lux::Color lightColor = lightGrids[l]->getLight()->getColor();
		float lightVal = exp(-kappa * lightGrids[l]->eval(ray));
		float brightness = lightGrids[l]->getLight()->getBrightness();
		lp += color->eval(ray) * lightColor * lightVal * brightness * (((1 - deltaT) * T)/kappa);
	}
	//cout << "Color eval: " << color->eval(ray)[0] << " " << color->eval(ray).X() << endl;
	T *= deltaT;
	S += deltaS;
      }
      std::vector<float> vec = {lp[0], lp[1], lp[2], lp[3]};
      setPixel(image, j, i, vec);
//      cout << "Color: " << lp[0] << " " << lp[1] << " " << lp[2] << " " <<lp[3]  << endl;
    }
  }
  return image;
}
