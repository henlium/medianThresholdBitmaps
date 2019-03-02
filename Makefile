all: bitmap image pgm main
	g++ -std=c++11 -Wall main.o bitmap.o image.o pgm.o -o alignMTB

bitmap: bitmap.cpp
	g++ -std=c++11 -Wall -c bitmap.cpp

image: image.cpp
	g++ -std=c++11 -Wall -c image.cpp

pgm: pgm.cpp
	g++ -std=c++11 -Wall -c pgm.cpp

main: main.cpp
	g++ -std=c++11 -Wall -c main.cpp
