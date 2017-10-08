#include <iostream>
#include <utility>
#include <fstream>
#include <string>
#include <vector>

#include "ising.h"

using namespace std;

Ising::Ising(int newsize, float newCouplingConst){
  couplingConst = newCouplingConst;
  size = newsize;
  configuration.resize(size, vector<int>(size, 0));
  for(int i = 0; i < size; i++){
    for(int j = 0; j <  size; j++){
      configuration[i][j] = 1; //default to spin-up
    }
  }
}

Ising::Ising(vector<vector<int>> newConfig, float newCouplingConst){
  size = newConfig.size();
  couplingConst = newCouplingConst;
  configuration = newConfig;
}

Ising::Ising(const Ising & other){
  size = other.size;
  couplingConst = other.couplingConst;
  configuration = other.configuration;
}




/*
 * Gets the spin of the neighbor in the indicated direction.
 * @return the spin of the neighbor in the direction of the spin at [row][col]. if the indices exceed the boundaries of the array, the function returns 0, so that the nonexistent neighbor does not contribute to the sum in the energy calculation
 *	
 */
int Ising::getNeighborSpin(int row, int col, int direction){
  if(direction == 0){ //north
    int tgtRow = row - 1;	//go to row above 
    int tgtCol = col;		//column still same			
    if(tgtRow < 0)
      return 0;	//out of bounds
    else 
      return configuration[tgtRow][tgtCol];
    
  } else if(direction == 1){ //east
    int tgtRow = row;
    int tgtCol = col + 1;
    if(tgtCol >= this->size)
      return 0;
    else 
      return configuration[tgtRow][tgtCol];
    
  } else if(direction == 2){ //south
    int tgtRow = row + 1; 	//go to row below
    int tgtCol = col;		//column still same
    if(tgtRow >= this->size) 
      return 0;
    else
      return configuration[tgtRow][tgtCol];
    
  } else if(direction == 3){ //west
    int tgtRow = row;
    int tgtCol = col - 1;
    if(tgtCol < 0) 
      return 0;
    else
      return configuration[tgtRow][tgtCol];
    
  }
}

/**
 * Gets spin of the ptcl in [row][col]
 */
int Ising::getSpin(int row, int col){
  return configuration[row][col];
}

/**
 * Gets the energy of the configuration
 * @return: the energy of the configuration (in configuration)
 */
float Ising::getEnergy(){
  //Loop through all nodes of the configuration
  //For each node in the configuration, get its nearest neighbors and use coupling coefficient to calculate the energy
  float totalEnergy = 0;
  for(int row = 0; row < configuration.size(); row++){
    for(int col = 0; col < configuration[0].size(); col++){
      for(int dir = 0; dir < 4; dir++){
	totalEnergy -= getSpin(row, col) * getNeighborSpin(row, col, dir) * this->couplingConst;
      }
    }
  }
  return totalEnergy;
}


