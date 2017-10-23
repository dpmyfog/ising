#include <iostream>
#include <utility>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <fstream>
#include <bitset>
#include <tuple>
#include "input.h"


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

void Ising::setAllUp(){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      configuration[i][j] = 1;
    }
  }
}

void Ising::setAllDown(){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      configuration[i][j] = -1;
    }
  }
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
      return configuration[this->size - 1][tgtCol];	//out of bounds, use periodic BCs
    else 
      return configuration[tgtRow][tgtCol];
    
  } else if(direction == 1){ //east
    int tgtRow = row;
    int tgtCol = col + 1;
    if(tgtCol >= this->size)
      return configuration[tgtRow][0];
    else 
      return configuration[tgtRow][tgtCol];
    
  } else if(direction == 2){ //south
    int tgtRow = row + 1; 	//go to row below
    int tgtCol = col;		//column still same
    if(tgtRow >= this->size) //periodic BCs
      return configuration[0][tgtCol];
    else
      return configuration[tgtRow][tgtCol];
    
  } else if(direction == 3){ //west
    int tgtRow = row;
    int tgtCol = col - 1;
    if(tgtCol < 0) 
      return configuration[tgtRow][this->size - 1];
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
  int counter = 0;
  
  while(counter < timesteps){
    //cout << "in first while" << endl;
    //cout << "counter: " <<  counter << endl;
    int randRow = rand()%size;
    int randCol = rand()%size;
    float probFlip = probToFlip(randRow, randCol, beta);
    //cout << "probability to flip " << probFlip << endl;
    float randomNumber = (float)rand()/(float)(RAND_MAX);
    //cout << randomNumber << endl;
    if(randomNumber <= probFlip){ //flip spin
      flipSpin(randRow, randCol);
      //cout << "flipped!" << endl;
      
    }
    counter++;    
    //cout << "Magnetization Sq: " << calculateMagSq() << endl;  
  }
  //printConfig();
}

void Ising::parallelSimulate(int timesteps, float beta){
  //build a and b grids
  vector<tuple<int, int>> aGrid;
  vector<tuple<int, int>> bGrid;
  //these will contain pointers to the spins in the actual configuration, so we can play with them
  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      if(((row + col) % 2) == 0){
	tuple<int, int> evenTuple(row, col);
	aGrid.push_back(evenTuple);
      }
      else{
	tuple<int, int> oddTuple(row, col);
	bGrid.push_back(oddTuple);
	
      }
    }
  }

  //now aGrid contains pointers to all spins with even indexSum
  //bGrid contains pointers to all spins with odd indexSum
  for(int steps = 0; steps < timesteps; steps++){
#pragma omp parallel for
    for(tuple<int, int> tup: aGrid){
      int row = get<0>(tup);
      int col = get<1>(tup);
      float probFlip = probToFlip(row, col, beta);
      //cout << "probability to flip " << probFlip << endl;
      float randomNumber = (float)rand()/(float)(RAND_MAX);
      //cout << randomNumber << endl;
      if(randomNumber <= probFlip){ //flip spin
	flipSpin(row, col);
	//cout << "flipped (" << row << "," << col << ")!" << endl;
      }
    }
#pragma omp barrier
#pragma omp parallel for
    for(tuple<int, int> tup: bGrid){
      int row = get<0>(tup);
      int col = get<1>(tup);
      float probFlip = probToFlip(row, col, beta);
      //cout << "probability to flip " << probFlip << endl;
      float randomNumber = (float)rand()/(float)(RAND_MAX);
      //cout << randomNumber << endl;
      if(randomNumber <= probFlip){ //flip spin
	flipSpin(row, col);
	//cout << "flipped (" << row << "," << col << ")!" << endl;
      }
    }
  }
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
  Ising::writeArrToFile("magSqs", magSq);
  return total/magSq.size();
}


float Ising::simFromFile(string infilename){ //we'll always run 2*size^2 times
  InputClass input;
  ifstream inputFile;

  //cout << "in simfromfile using " << infilename << endl;
  inputFile.open(infilename);
  if(!inputFile) cout << "could not read file" << endl;
  input.Read(inputFile);
  double beta=input.toDouble(input.GetVariable("beta"));
  int size=input.toInteger(input.GetVariable("Lx"));
  int numSteps = 2*size*size;
  string outFile = input.GetVariable("outFile");

  string magSqFilename = "magSqFiles/L"+ to_string(size);
  
  Ising model(size, 1);
  model.parallelSimulate(numSteps, beta);
  model.recordSnapshot(outFile);


  return model.calculateMagSq();
  
  
  
}

void Ising::writeArrToFile(string filename, vector<float> myvec){
  ofstream myfile;
  myfile.open(filename);
  for(int i = 0; i < myvec.size(); i++){
    myfile << myvec[i] << endl;
  }
  myfile.close();
  
}



void Ising::produceInputFile(string filename, float beta, int size){
  ofstream myfile;
  myfile.open(filename);
  string outfilename = "outfiles/out_L" + to_string(size) + "_" + to_string(beta);
  myfile << "beta=" << beta << endl;
  myfile << "Lx=" << size << endl;
  myfile << "Ly=" << size << endl;
  myfile << "outFile=outfiles/" << outfilename;
}

void Ising::recordSnapshot(string filename){
  ofstream myfile;
  myfile.open(filename);
  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      myfile << configuration[row][col] << " ";
    }
    myfile << endl;
  }
}

void Ising::makeConfigMap(){
  Ising example(3, 1);
  string filename = "configurationMapBoltzmann";
  vector<float> energies;
  for(int config = 0; config < 512; config++){
    string bitstring = bitset<9>(config).to_string();
    //now have the bitstring representing the configuration, let first (most sig bit) represent (2,2) and last (least sig bit) represent (0,0)
    example.setAllUp(); //everything is set to up
    for(int row = 0; row < 3; row ++){
      for(int col = 0; col < 3; col++){
	int strIdx = 9 - (row*3 + col) - 1; // (2,2) -> idx = 0 -> most significant
	int bitval = stoi(to_string(bitstring[strIdx])) - 48;
        if(bitval == 1) example.configuration[row][col] = bitval;
	else example.configuration[row][col] = -1;
      }
    }
    //now the example is set to the correct configuration, record the energy
    //energies.push_back(exp(-1*example.getEnergy()));
    energies.push_back(exp(-0.25 *example.getEnergy())); //beta*J = 0.25
  }
  writeArrToFile(filename, energies);
  
}

void Ising::testConfigMap(){
  vector<float> endStates5;
  vector<float> endStates50;
  vector<float> endStates500;
  Ising example(3,1);
  //endStates now stores the number of times a configuration gets landed on. This should be the same as the configurationMap from before.
  for(int config = 0; config < 512; config++){
    endStates5.push_back(0);
    endStates50.push_back(0);
    endStates500.push_back(0);
  }

  //simulate for a while
  //calculate binary state number associated with config
  //increment array indexed with that state
  for(int rep = 0; rep < 10000; rep++){
    cout << "began with rep = " << rep << endl;
    example.setAllUp();
    example.simulate(5, 0.25);
    example.printConfig();
    int config = 0;
    for(int row = 0; row < 3; row++){
      for(int col = 0; col < 3; col++){
	int exponent = (row*3 + col);
	int base = 0;
	if(example.configuration[row][col] == 1)
	  base = 1;
	else
	  base = 0;
	config += base*pow(2, exponent);
      }
    }
    cout << "incremented " << config << "th spot" << endl;
    cout << rep << endl;
    endStates5[config]++;
  }
  writeArrToFile("endStates5", endStates5);
  
  for(int rep = 0; rep < 10000; rep++){
    example.setAllUp();
    example.simulate(50, 0.25);
    int config = 0;
    int base = 0;
    for(int row = 0; row < 3; row++){
      for(int col = 0; col < 3; col++){
	int exponent = row*3 + col;
	int base = 0;
	if(example.configuration[row][col] == 1)
	  base = 1;
	else
	  base = 0;
	config += base*pow(2, exponent);
      }
    }
    endStates50[config]++;
  }
  writeArrToFile("endStates50", endStates50);
  
  

  for(int rep = 0; rep < 10000; rep++){
    example.setAllUp();
    example.simulate(500, 0.25);
    int config = 0;
    int base = 0;
    for(int row = 0; row < 3; row++){
      for(int col = 0; col < 3; col++){
	int exponent = row*3 + col;
        if(example.configuration[row][col] == 1)
	  base = 1;
	else
	  base = 0;
	config += base*pow(2, exponent);
      }
    }
    endStates500[config]++;
  }
  writeArrToFile("endStates500", endStates500);
  
}


vector<vector<int>> Ising::coarseGrain(){
  vector<vector<int>> newConfig = configuration;
  int newSize = size/3;
  newConfig.resize(newSize);
  for(int i = 0; i < newSize; i++){
    newConfig[i].resize(newSize);
  }

  //want to coarse grain by processing top left cell
  for(int coarserow = 0; coarserow < newSize; coarserow++){
    for(int coarsecol = 0; coarsecol < newSize; coarsecol++){
      int numUp = 0;
      int numDown = 0;

      int startRowIdx = coarserow*3;
      int endRowIdx = coarserow*3 + 3;

      int startColIdx = coarsecol*3;
      int endColIdx = coarsecol*3 + 3;

      for(int row = startRowIdx; row < endRowIdx; row++){
	for(int col = startColIdx; col < endColIdx; col++){
	  if(configuration[row][col] == 1) numUp++;
	  else numDown++;
	}
      }

      if(numUp > numDown) newConfig[coarserow][coarsecol] = 1;
      else newConfig[coarserow][coarsecol] = -1;
    }
  }

  return newConfig;

  
}
