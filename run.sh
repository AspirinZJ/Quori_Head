cd src
rm out
rm libs2i.so
gcc --shared -fPIC -o libs2i.so sphere2Image/mTot.c sphere2Image/sphere2Plane.c sphere2Image/sphere2Image.c -lm
gcc -o out main.c rotation_theta.c -lGL -lGLU -lglut -lm -ldl
sudo ./out