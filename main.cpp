#include <iostream>
#include <fstream>
#include "ising.h"

using namespace std;

int main(){
  //srand(time(NULL));
  //Ising example(5, 1);
  //example.printConfig();
  //example.printConfig();
  //cout << example.probToFlip(1, 2, 0.25) << endl;
  //example.parallelSimulate(0.25);
  //example.printConfig();
  //Ising::testInput("butts");
  //cout << "average magnetization over a lot of trials: " <<  example.simulateMag(1, 100000, 0.25) << endl;
  //example.reset();
  //example.recordSnapshot("exampleSnapshot");
  //Ising::makeConfigMap();
  //Ising::testConfigMap();
  
  //for(int size = 10; size < 20; size++){
  int size = 20;
    vector<float> magSqs;
    for(float betas = 0.1; betas < 1; betas += 0.005){
      string filename = "inputfiles/in_L" + to_string(size) + "_" + to_string(betas);
      
      Ising::produceInputFile(filename, betas, size);
      magSqs.push_back(Ising::simFromFile(filename));

 
    }
    Ising::writeArrToFile("magSqFiles/magSq" + to_string(size), magSqs);
    //}
 
  

  
 
}
