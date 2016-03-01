#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <time.h> //Used for random operator power

using namespace std;
/* 
//  Class Individual
//  this class represents each of the individuals that will be used on a GABIL
//  genetic algorithm. The class will consist of the following variables: 
//  a) A set of rules. This will be a vector<int> of size n*k, where n is the number 
//  of rules and k is the size of each rule. Each int will be 0 or 1, accordingly.
//  b) The size of the rule set.
//  c) A fitness value, defined by the fitness function.
//
//  Additionally, it will have the following functions:
//  a) mutation operator, which will "flip" a "bit" of a rule chain.
//  b) selection operator, which will be a 2 point crossover variant.
//  c) fitness function, that compares the obtained values against expected values.
*/

const int MAX_RS = 3;   //Upper bound to the number of initial rules

float randomF();    //returns a random value between 0 and 1

class Individual {
  public:
    vector<int> ruleSet;
    int setSize;
    float fitness;

    //****************** CONSTRUCTORS ********************//
    
    Individual() {}
    
    //creates an individual with a random ruleSet, given a rule size
    Individual(int size) {
        
        float a = randomF();
        int rules = static_cast <int> (floor(randomF()*MAX_RS))+1; //Defines the ruleSet size
        vector<int> rs(size*MAX_RS);
        for (int i = 0; i < size*rules; i++) {
            rs.at(i) = randomF() > 0.5 ? 1 : 0; //Initial randomization
        }
        ruleSet = rs;
        setSize = size;
        fitness = -1;        
    }

    //Creates an individual with pre established values
    Individual(vector<int> rs, int size) {
        ruleSet = rs;
        setSize = size;
    }

    //******************* FUNCTIONS ***********************//
    void mutate() {
        for (int i = 0; i < ruleSet.size(); i++) {
            if (randomF() < 0.01) {
                ruleSet.at(i) = (ruleSet.at(i)+1)%2;    //shifts the bit 
                break;    
            } 
        }
    }

    //Defines a lower bound for an individual's rule set
    //This value goes between 0 and the ruleSet size - 1,
    //so that we can ensure a 2 point partition.
    //Possible Range: [0, size-1]
    int lowerBound() {
        return (int) floor(randomF()*ruleSet.size());
    }

    //Same deal, but with an upper bound
    //This one needs the lower bound to ensure that it will create it after
    //and not before it.
    //Possible Range: [lowerbound+1, size]
    int upperBound(int lb) {
        return (int) floor(randomF()*(ruleSet.size()-lb))+lb+1;  //it works, trust me
    }

    //******This is where it gets Tricky*********//
    //Defines a lower bound for an individual's ruleset
    //unlike the other case, this is restricted to values that would
    //only fit the first lower bound, so it can ensure a crossover
    //they must also ensure that the upper bound doesnt go outside 
    //the array.. so there's that as well
    //Possible Range: [0+lowk, idonteven]. Discrete values with "size" jumps
    int lowerBoundT(int lowk, int highk) {
        int kk = lowk % setSize;       //How offset is the lowerBound in relation to a rule's start
        int range = highk - lowk;   //The size of the (soon to be crossovered) segment
        int rules = ruleSet.size()/setSize;    
        return (int) ( ((floor(randomF()*(rules-floor((range+kk-1)/setSize)))+1)*setSize)
            +kk-setSize );   //C stands for Complex.
        return 0;
        
    }

    //Same deal, but trickier.
    //Possible Range: [0+highk, idonteven]
    int upperBoundT(int lowk, int highk, int lb) {
        int rules = ruleSet.size()/setSize;
        int range = highk - lowk;
        return int (floor(randomF()*(rules-floor(lb/setSize)-floor((range)/setSize))) 
                * setSize + lb+range);  //Bruh
    }


};

float randomF() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}


int main() {

    srand(time(NULL));
    
    int s = 4;
    int a = 3;
    Individual i(s);
}

