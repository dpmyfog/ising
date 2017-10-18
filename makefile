ising:
	g++ -O3 -std=c++11 main.cpp ising.cpp input.h -o main

clean:
	rm ./inputfiles/*
	rm ./outfiles/*
	rm ./magSqFiles/*
