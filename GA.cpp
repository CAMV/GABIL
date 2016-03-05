#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <time.h> //Used for random operator power
#include "GabilInd.cpp"

using namespace std;

const float OFFSPRING_AMOUNT = 0.2;
const int ORIGINAL_SIZE = 200;

class Population {
  public:
    vector<Individual> pop;
    vector<int> attrSize;       //the size of each rule attribute (used on evaluation methods)
    //vector<float> fitWeight;    //Aggregate weight of the fitness of each value
    vector<vector<int> > trainer;        //training data
    float bigWeight;                    //parent selection accumulated weight
    float bigUnfitWeight;               //survivor selection accumulated weight


    /********************************* CONSTRUCTORS ********************************/
    Population() {}

    Population(vector<Individual> _pop, vector<vector<int> > trainingData, vector<int> attrS) {
        pop = _pop;
        trainer = trainingData;
        attrSize = attrS;
    }



    /********************************* FUNCTIONS **********************************/
    
    //Evaluates the fitness function to each individual of the population
    
    void setBigWeight() {
        bigWeight = 0;
        bigUnfitWeight = 0;
        for (vector<Individual>::reverse_iterator it = pop.rbegin(); it < pop.rend(); it++) {
            bigWeight += it->fitness;
            it->fitWeight = bigWeight;
        }

        for (vector<Individual>::iterator it = pop.begin(); it < pop.end(); it++) {
            bigUnfitWeight += it->unFitness;
            it->unFitWeight = bigUnfitWeight;
        }
    }

    void fitOut() {
        for (vector<Individual>::iterator it = pop.begin(); it < pop.end(); it++) {
            if (it->fitness == -1) {    //Only if it hasn't been set before
                it->getFitness(trainer, attrSize);
            }
        }
        sort(pop.begin(), pop.end());
    
        setBigWeight();        
    }

    vector<Individual> crossover(Individual i1, Individual i2) {
        //Individual son1;
        //Individual son2;
       // cout << "dafuk" << endl;
        int l1 = i1.lowerBound();
        int h1 = i1.upperBound(l1);
        int l2 = i2.lowerBoundT(l1, h1);
        int h2 = i2.upperBoundT(l1, h1, l2);

       // cout << "baaa" << endl;

        vector<int> rs1;    //first new ruleset
        for (int k = 0; k < l1; k++) {
            rs1.push_back(i1.ruleSet.at(k));
        }
        for (int k = l2; k < h2; k++) {
            rs1.push_back(i2.ruleSet.at(k));
        }
        for (int k = h1; k < i1.ruleSet.size(); k++) {
            rs1.push_back(i1.ruleSet.at(k));
        }
        int s1 = rs1.size();
        Individual son1(rs1, s1);

        vector<int>rs2;     //second new ruleset
        for (int k = 0; k < l2; k ++) {
            rs2.push_back(i2.ruleSet.at(k));
        }
        for (int k = l1; k < h1; k++) {
            rs2.push_back(i1.ruleSet.at(k));
        }
        for (int k = h2; k < i2.ruleSet.size(); k++) {
            rs2.push_back(i2.ruleSet.at(k));
        }
        int s2 = rs2.size();
        Individual son2(rs2,s2);


        vector<Individual> sons;
        sons.push_back(son1);
        sons.push_back(son2);
        return sons;
    }


    //Genetic crossover with roulette selection method
    vector<Individual> rouletteCrossover() {
        vector<Individual> newGene;
        int k = (int) pop.size()*OFFSPRING_AMOUNT;     //amount of the pop that will yield offspring
        while ( k > 0) {
            //Parent Selection
            vector<Individual>::iterator it = pop.begin(); //first parent
            vector<Individual>::iterator it2 = pop.begin(); //first parent
            float r1 = randomF()*(bigWeight); //offset so it can pick the first individual
            float r2 = randomF()*(bigWeight); //offset so it can pick the first infividual
            //cout << r1 << endl;
            //cout << r2 << endl;
            //cout << bigWeight << endl;
            while (it < pop.end()) {            //find parent 1
                if (r1 < it->fitWeight) {
                    break;
                }
                it++;
            }
            while (it2 < pop.end()) {           //find parent 2
                if (r2 < it2->fitWeight) {     
                    break;
                }
                it2++;
            }

            //cout << "selected" << endl;


            vector<Individual> i;
            //Crossover
            if (it->ruleSet.size() < it2->ruleSet.size()) {
                //cout << "wat" << endl;
                i = crossover(*it, *it2);
            }
            else {
                //cout << "da" << endl;
                i = crossover(*it2, *it);
            }


            //Mutation
            i.at(0).mutate();
            i.at(1).mutate();

            //Adding
            newGene.push_back(i.at(0));
            newGene.push_back(i.at(1));
            k--;
        }

        return newGene;
    }

    void rouletteSelection() {
        int toDel = pop.size() - ORIGINAL_SIZE;
        for (int k = 0; k < toDel; k++) {
            float r = randomF() * bigUnfitWeight;
            vector<Individual>::iterator it = pop.begin(); 
            while (r > it->unFitWeight) {
                it++;
            }
            pop.erase(it);
            setBigWeight();
        }
    }

    //Creates the new generation, based on crossover and mutation attributes.
    //i will be the parameter the defines what selection method to use
    void newGen(int i, int j) {
        vector<Individual> newGene;
        //int rS = pop.at(0).setSize;
        if (i == 0) { //Roulette method
            newGene = rouletteCrossover();
        }
        if (i == 1) {
            //Tournament method
        }

        //cout << "crossed" << endl;
        //Create the new population
        vector<Individual> newPop;
        newPop.reserve(pop.size() + newGene.size());
        newPop.insert(newPop.end(), pop.begin(), pop.end());
        newPop.insert(newPop.end(), newGene.begin(), newGene.end());

        pop = newPop;
        fitOut();

        //Select survivors
        if (j == 0) {
            //cout << "SELECTION" << endl << endl;
            rouletteSelection();
        }
        if (j == 1) {
            //tournamente method
        }
        fitOut();
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
    for (int i = 0; i < ORIGINAL_SIZE; i++) {
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
    int i = 0;

    cout << estequetaki.bigWeight << endl;


    for (int i = 0; i < 100; i ++) {
        estequetaki.newGen(0,0);
    }

    cout << estequetaki.bigWeight << endl;

}
