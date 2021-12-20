OBJS = base/Matrix.o base/LinearAlgebra.o base/PerlinNoise.o base/Image.o base/Camera.o base/OBJ_Loader.o base/Rotation.o base/Shell.o base/ColorField.o base/ColorUnion.o base/ScalarGrid.o base/ColorGrid.o base/DeepShadowMap.o base/Torus.o base/LevelSet.o base/Cone.o base/Ellipse.o base/Union.o base/Cutout.o base/Intersection.o base/Blending.o base/SteinerPatch.o base/Plane.o base/Box.o base/Icosahedron.o base/PyroclasticSphere.o base/Wisp.o base/StampedNoise.o base/PyroDisplace.o base/main.o

ROOTDIR = .
LIB = ./lib/volumes.a

INCLUDES = -I./include

MAIN = ./base/main.cpp

CXX = g++ -shared -Wall -g -O2 -fPIC $(DEFINES) -fopenmp -std=c++11

.cpp.o:
	$(CXX) -c $(INCLUDES) $< -o $@

$(LIB): $(OBJS)
	$(MAKE) $?
	ar rv $(LIB) $?

exec:
	g++ -o executable $(OBJS) include/ScalarField.h include/Cone.h include/DensityField.h -L/usr/lib/ -fopenmp

clean:
	rm -rf base/*.o $(LIB) base/*~ include/*~ executable 
