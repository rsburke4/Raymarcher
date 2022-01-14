# Raymarcher
A Raymarcher that renders implicit surfaces, levelsets, pyroclastics, and noise effects such as wisps and stamped noise with lighting and shadows.

Primitive objects are defined in the 'include' folder. base/main.cpp acts as a modular framework for creating and placing instanced of these objects in the scene. main.cpp also includes the basic logic to write an image to an array, or to an image. By editing this file, you are defining a scene. Once the scene has been defined, the program needs to be compiled, and executed with the commands:

make clean
make
make exec
./exec

Doing so will generate an image in the directory defined in the main.cpp file.

Here are some example images generated with this technique:

[Images]
