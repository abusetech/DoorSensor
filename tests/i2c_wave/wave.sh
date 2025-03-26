#!/bin/bash

mkdir -p include
cp ../../src/softi2c.* include/
cp common/* include/
#Strip all of the include statments that use system includes (We will mock these)
sed -i '/#include\s<.*>/d' include/softi2c.h

mkdir -p build
g++ -c include/common.cpp -o build/common.o
g++ -c include/softi2c.cpp -o build/softi2c.o
g++ -c main.cpp -o build/main.o
g++ -o sim_i2c build/main.o build/softi2c.o build/common.o 
./sim_i2c > out.csv
octave plt.m