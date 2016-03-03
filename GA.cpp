#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <time.h> //Used for random operator power
#include "GabilInd.cpp"

using namespace std;

class Population {
  public:
    vector<Individual> pop;
    vector<int> attrSize;       //the size of each rule attribute (used on evaluation methods)
    vector<vector<int> > trainer;        //training data


    /********************************* CONSTRUCTORS ********************************/
    Population() {}

    Population(vector<Individual> _pop, vector<vector<int> > trainingData, vector<int> attrS) {
        pop = _pop;
        trainer = trainingData;
        attrSize = attrS;
    }



    /********************************* FUNCTIONS **********************************/
    void fitOut() {
        for (vector<Individual>::iterator it = pop.begin(); it < pop.end(); it++) {
            it->getFitness(trainer, attrSize);
        }
        sort(pop.begin(), pop.end());
        
    }
    
};

int main() {
    srand(time(NULL));    //Random seed initialization
    
    int ruleSize = 5; // the size of each rule, including the classification
    vector<int> attrS;
    attrS.push_back(2);
    attrS.push_back(2);
    attrS.push_back(-1);
    
    vector<Individual> pops;
    for (int i = 0; i < 200; i++) {
        pops.push_back(Individual(ruleSize));
    }

    

    vector<int> train;
    train.push_back(1);
    train.push_back(0);
    train.push_back(1);
    train.push_back(0);
    train.push_back(0);
    vector<vector<int> > data;
    data.push_back(train);
    vector<int> train2;
    train2.push_back(1);
    train2.push_back(0);
    train2.push_back(0);
    train2.push_back(1);
    train2.push_back(1);
    data.push_back(train2);


    Population estequetaki(pops, data, attrS);        //Palabra aguda

    estequetaki.fitOut();
    for (vector<Individual>::iterator it = estequetaki.pop.begin(); it < estequetaki.pop.end(); it++) {
        cout << it->fitness << endl;
    }
    



    /*for (int i = 0; i < 1000; i ++) {
        
    }*/

}
