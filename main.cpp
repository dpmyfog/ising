#include <iostream>
#include <fstream>
#include "ising.h"

using namespace std;

int main(){
  srand(time(NULL));
  Ising example(20, 1);
  //example.printConfig();
  //cout << example.probToFlip(1, 2, 0.25) << endl;
  //example.simulate(100, 0.25);
  cout << "average magnetization over 10000 trials: " <<  example.simulateMag(1, 100000, 0.25) << endl;
}
