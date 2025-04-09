#!/bin/bash

# Compile the conversion functions implementation
g++ -g -c itoa.cpp -o itoa.o
g++ -g -c dtostrf.cpp -o dtostrf.o

# Compile the Arduino String class implementation
# Need to tell it to look in current directory for the conversion functions
g++ -g -IArduinoCore-API/api -I. -c ArduinoCore-API/api/String.cpp -o String.o

# Compile your main program
g++ -g -IArduinoCore-API/api -I. -c main.cpp -o main.o

# LINK all components together
g++ -g itoa.o dtostrf.o String.o main.o -o main