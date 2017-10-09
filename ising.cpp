#include <iostream>
#include <utility>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

#include "ising.h"

using namespace std;

Ising::Ising(int newsize, float newCouplingConst){
  couplingConst = newCouplingConst;
  size = newsize;
  configuration.resize(size, vector<int>(size, 0));
  srand(time(NULL));
  for(int i = 0; i < size; i++){
    for(int j = 0; j <  size; j++){
      if(rand()%2 == 0) configuration[i][j] = 1; //default to spin-up
      else configuration[i][j] = -1;
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


void Ising::printConfig(){
  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      cout << configuration[row][col] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

void Ising::reset(){
  srand(time(NULL));
  for(int i = 0; i < size; i++){
    for(int j = 0; j <  size; j++){
      if(rand()%2 == 0) configuration[i][j] = 1; //default to spin-up
      else configuration[i][j] = -1;
    }
  }
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
  return totalEnergy/2;
}

void Ising::flipSpin(int row, int col){
  configuration[row][col] = configuration[row][col]*-1;
}

 
float Ising:: calculateMagSq(){
  int numSpins = size*size;
  int spinSum = 0;
  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      spinSum += configuration[row][col];
    }				     
  }
  return ((float)spinSum)*((float) spinSum) / numSpins / numSpins;
}



/**
 * Calculate the probability to change into the configuration with the spin at [row][col] flipped
 *
 */

float Ising::probToFlip(int row, int col, float beta){
  Ising copy(*this);
  copy.flipSpin(row, col);
  float thisEnergy = getEnergy();
  float otherEnergy = copy.getEnergy();
  //cout << thisEnergy << endl;
  //cout << otherEnergy << endl;
  
  float prob = exp(-1*beta*otherEnergy + 1*beta*thisEnergy);
  return min(prob, 1.0f);
}

void Ising::simulate(int timesteps, float beta){
  //cout << "got into simulate" << endl;
  int counter;
  
  while(counter < timesteps){
    //cout << "in first while" << endl;
    int randRow = rand()%size;
    int randCol = rand()%size;
    float probFlip = probToFlip(randRow, randCol, beta);
    //cout << "probability to flip " << probFlip << endl;
    float randomNumber = rand()/(RAND_MAX + 1.);
    if(randomNumber <= probFlip){ //flip spin
      flipSpin(randRow, randCol);
      counter++;
    }
    else{
      //do nothing
    }
    //cout << "Magnetization Sq: " << calculateMagSq() << endl;
    
    
  }
  //printConfig();

}

float Ising::simulateMag(int timestep, int samples, float beta){
  vector<float> magSq;
  
  //cout << "made vector" << endl;
  for(int i = 0; i < samples; i++){
    //cout << i << endl;
    simulate(timestep, beta);
    //cout << "completed " << i << " simulation(s). m = " << calculateMagSq() <<  endl;
    magSq.push_back(calculateMagSq());
    //cout << "pushed to back" << endl;
    //reset();
  }
  float total = 0;
  for(int i = 0; i < magSq.size(); i++){
    total+=magSq[i];
    //cout << total << endl;
  }
  return total/magSq.size();
}

