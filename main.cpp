#include <iostream>
#include <fstream>
#include "ising.h"

using namespace std;

int main(){
  srand(time(NULL));
  Ising example(20, 1);
  //example.printConfig();
  //cout << example.probToFlip(1, 2, 0.25) << endl;
  //example.simulate(100, 0.1);
 cout << "average magnetization over a lot of trials: " <<  example.simulateMag(1, 100000, 0.1) << endl;
  example.reset();
  Ising::makeConfigMap();
}
