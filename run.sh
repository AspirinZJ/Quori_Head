cd src
rm a.out
rm libs2i.so
gcc --shared -fPIC -o libs2i.so sphere2Image/mTot.c sphere2Image/sphere2Plane.c sphere2Image/sphere2Image.c -lm
gcc -o a.out -g main.c genFigure.c rotateTheta.c -lGL -lGLU -lglut -lm -ldl
./a.out