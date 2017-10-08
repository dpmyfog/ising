#ifndef ISING_H
#define ISING_H

#include <string>
#include <vector>

using namespace std;

class Ising {

  
  
  
 public:
  

  std::vector<vector<int>> configuration;
  int size;
  float couplingConst;
  
  Ising(int size, float newCouplingConst);
  Ising(const Ising& other);
  Ising(vector<vector<int>> newConfig, float newCouplingConst);
  
  float getEnergy();
  int getSpin(int row, int col);
  int getNeighborSpin(int row, int col, int direction);
  

};


#endif
