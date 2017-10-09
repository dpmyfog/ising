#include <iostream>
#include <fstream>
#include "ising.h"

using namespace std;

int main(){
  Ising example(10, 1);
  example.printConfig();
  //cout << example.probToFlip(1, 2, 0.25) << endl;
  //example.simulate(100, 0.25);
  cout << "average magnetization over 10000 trials: " <<  example.simulateMag(100000, 1000, 0.25) << endl;
}
