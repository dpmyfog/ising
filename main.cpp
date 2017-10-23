#include <iostream>
#include <fstream>
#include "ising.h"

using namespace std;


void generateMagnetizationDistribn(float t1){
  int size = 27;
  float temperature = t1;
  vector<float> magSqs;
  Ising model(size, 1);

  model.simulate(27*10, temperature);
  
  for(int steps = 0; steps < 1000; steps++){
    cout << "steps: " << steps << endl;
    model.simulate(27*10, temperature);
    magSqs.push_back(model.calculateMagSq());
  }

  Ising::writeArrToFile("magSqFiles/magDistribn_" + to_string(t1), magSqs);
  
}

void testProbabilityDistribn(){
  
  for(int size = 10; size < 20; size++){
  
    
    vector<float> magSqs;
    for(float betas = 0.1; betas < 1; betas += 0.005){
    
    string filename = "inputfiles/in_L" + to_string(size) + "_" + to_string(betas);
    
    Ising::produceInputFile(filename, betas, size);
    magSqs.push_back(Ising::simFromFile(filename));
    
    
    }
    Ising::writeArrToFile("magSqFiles/magSq" + to_string(size), magSqs);
  
  }
}
  
    
void generatePM(float temp1, float temp2){

  float temperature1 = temp1;
  float temperature2 = temp2;
  
  Ising lowTemp(81, 1);
  Ising highTemp(81, 1);

  Ising nativeSmall(27, 1);

  string baseFileName = "coarsegrained/";
  string magnFileName = "coarsemagnetizations/";
    
    
  lowTemp.simulate(81*81, temperature1);
  highTemp.simulate(81*81, temperature2);

  
  
  vector<float> singleLoTempMags;
  vector<float> singleHiTempMags;

  vector<float> doubleLoTempMags;
  vector<float> doubleHiTempMags;
  
  for(int i = 0; i < 1000; i++){
    cout << "i: " << i << endl;
    lowTemp.simulate(81*10, temperature1);
    highTemp.simulate(81*10, temperature2);

    Ising tempSingleLowTemp(lowTemp.coarseGrain(), 1);
    Ising tempSingleHiTemp(highTemp.coarseGrain(), 1);

    //tempSingleLowTemp.recordSnapshot(baseFileName + to_string(i) + "_low_single");
    //tempSingleHiTemp.recordSnapshot(baseFileName + to_string(i) + "_hi_single");

    Ising tempDoubleLowTemp(tempSingleLowTemp.coarseGrain(), 1);
    Ising tempDoubleHiTemp(tempSingleHiTemp.coarseGrain(), 1);

    //tempDoubleLowTemp.recordSnapshot(baseFileName + to_string(i) + "_low_double");
    //tempDoubleHiTemp.recordSnapshot(baseFileName + to_string(i) + "_hi_single");
    
    singleLoTempMags.push_back(tempSingleLowTemp.calculateMagSq());
    singleHiTempMags.push_back(tempSingleHiTemp.calculateMagSq());

    doubleLoTempMags.push_back(tempDoubleLowTemp.calculateMagSq());
    doubleHiTempMags.push_back(tempDoubleHiTemp.calculateMagSq());

    
    
  }
  Ising tempSingleLowTemp(lowTemp.coarseGrain(), 1);
  Ising tempSingleHiTemp(highTemp.coarseGrain(), 1);

  tempSingleLowTemp.recordSnapshot(baseFileName + "_low_single");
  tempSingleHiTemp.recordSnapshot(baseFileName + "_hi_single");

  Ising tempDoubleLowTemp(tempSingleLowTemp.coarseGrain(), 1);
  Ising tempDoubleHiTemp(tempSingleHiTemp.coarseGrain(), 1);
    
  tempDoubleLowTemp.recordSnapshot(baseFileName + "_low_double");
  tempDoubleHiTemp.recordSnapshot(baseFileName + "_hi_double");
  
  Ising::writeArrToFile(magnFileName + "_single_mag_low", singleLoTempMags);
  Ising::writeArrToFile(magnFileName + "_single_mag_high", singleHiTempMags);
  
  Ising::writeArrToFile(magnFileName + "_double_mag_low", doubleLoTempMags);
  Ising::writeArrToFile(magnFileName + "_double_mag_high", doubleHiTempMags);
  

}


int main(){
  //Ising example(9,1);
  //example.printConfig();
  //Ising coarseGrained(example.coarseGrain(), 1.0f);
  //coarseGrained.printConfig();
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
  cout << "generating PM" << endl;
  generatePM(0.10, 0.80);
  //cout << "generating magnetn distribns" << endl; 
  //for(float temp = 0.05; temp < 0.5; temp += 0.01){
  // generateMagnetizationDistribn(temp);
  //}
  

  
}
