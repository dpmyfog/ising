#include <iostream>
#include <fstream>
#include "ising.h"

using namespace std;

int main(){
  Ising example(3, 1);
  Ising copyExample(example);
  
  cout << example.getEnergy() << endl;
}
