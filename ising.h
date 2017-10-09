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
  static constexpr long double kBoltz = 1.38064e-23;
  
  Ising(int size, float newCouplingConst);
  Ising(const Ising& other);
  Ising(vector<vector<int>> newConfig, float newCouplingConst);

  void printConfig();
  
  float getEnergy();
  int getSpin(int row, int col);
  int getNeighborSpin(int row, int col, int direction);
  void flipSpin(int row, int col);
  float probToFlip(int row, int col, float beta);
  float calculateMagSq();

  void simulate(int timesteps, float beta);
  float simulateMag(int timesteps, int samples, float beta);
  void reset();
  
  

};


#endif
