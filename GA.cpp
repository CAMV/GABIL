#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
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
            int l1 = i1.lowerBound();
            int h1 = i1.upperBound(l1);
            int l2 = i2.lowerBoundT(l1, h1);
            int h2 = i2.upperBoundT(l1, h1, l2);

            //cout << "bounds:" << endl;
            //cout << l1 << " " << h1 << " " << l2 << " " << h2 << endl;
            //cout << "sizes: " << i1.ruleSet.size() << " " << i2.ruleSet.size() << endl;

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
            Individual son1(rs1, i1.setSize);

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
            Individual son2(rs2,i2.setSize);

            //cout << "created" << endl;


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
                //cout << "SELECTING PARENTS" << endl;
                vector<Individual>::iterator it = pop.begin(); //first parent
                vector<Individual>::iterator it2 = pop.begin(); //second parent
                float r1 = randomF()*(bigWeight);
                float r2 = randomF()*(bigWeight);
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
                //cout << "crossing over" << endl;
                if (it->ruleSet.size() < it2->ruleSet.size()) {
                    //cout << "wat" << endl;
                    i = crossover(*it, *it2);
                }
                else {
                    //cout << "da" << endl;
                    i = crossover(*it2, *it);
                }


                //Mutation
                //cout << "mutating" << endl;
                i.at(0).mutate();
                i.at(1).mutate();

                //Adding
                newGene.push_back(i.at(0));
                newGene.push_back(i.at(1));
                k--;
            }

            return newGene;
        }

        vector<Individual> tournamentCrossover() {

            int k = pop.size()*OFFSPRING_AMOUNT;
            vector<Individual> newGene;
            for (int i = 0; i < k; i++) {
                //get First parent
                Individual p1;
                p1 = pop.at((int) floor(randomF()*pop.size()));   //set an initial
                for (int j = 0; j < k/2 - 1; j++) {
                    int r = (int) floor(randomF()*pop.size());
                    if (p1.fitness < pop.at(r).fitness) {   //compare to new ones
                        p1 = pop.at(r);
                    }
                }

                //get Second parent
                Individual p2;
                p2 = pop.at((int) floor(randomF()*pop.size()));   //set an initial
                for (int j = 0; j < k/2 - 1; j++) {
                    int r = (int) floor(randomF()*pop.size());
                    if (p2.fitness < pop.at(r).fitness) {       //compare to new ones
                        p2 = pop.at(r);
                    }
                }
                vector<Individual> newi;
                //crossover
                if (p1.ruleSet.size() < p2.ruleSet.size()) {
                    newi = crossover(p1,p2);
                }

                else {
                    newi = crossover(p2,p1);
                }

                //mutation
                newi.at(0).mutate();
                newi.at(1).mutate();

                //addition
                newGene.push_back(newi.at(0));
                newGene.push_back(newi.at(1));

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
            return;
        }

        void tournamentSelection() {
            int toDel = pop.size() - ORIGINAL_SIZE;
            for (int k = 0; k < toDel; k++) {
                int i = (int) floor(randomF()*pop.size());
                for (int j = 0; j < 20; j++) {
                    int r = (int) floor(randomF()*pop.size());
                    if (pop.at(i).fitness > pop.at(r).fitness) {
                        i = r;
                    }
                }
                pop.erase(pop.begin() + i);
            }
            setBigWeight();
            return;
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

                newGene = tournamentCrossover();
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
                tournamentSelection();
            }
            fitOut();
        }

    };

vector< vector<int> > readTrainingSet(string fileName) {

    vector<vector<int> > data;  //Training Set
    vector<int> train;
    train.clear();
    int value;
    string line;
    float f;

    ifstream file("Datos");

    if (file.is_open()) {
        int aPos = 1;
        while (getline(file, line, ',')) {
            //cout << aPos << " " << line << "\n";
            switch (aPos) {
                case 1:
                    if (line.compare("a") == 0) {
                        value = 1;
                    }
                    else if (line.compare("b") == 0) {
                        value = 2;
                    }
                    else {
                        value = 3;
                    }
                    break;
                case 2:
                //cout << line <<"HERE";
                    if (line.compare("?") != 0){
                        
                        f = stof(line);
                        if (f < 20) {
                            value = 1;
                        }
                        else if (f >= 20 && f < 25) {
                            value = 2;
                        }
                        else if (f >= 25 && f < 35) {
                            value = 4;
                        }
                        else if (f >= 35 && f < 40) {
                            value = 8;
                        }
                        else if (f >= 40 && f < 50) {
                            value = 16;
                        }
                        else if (f >= 50 && f < 60) {
                            value = 32;
                        }
                        else if (f >= 60) {
                            value = 64;
                        }
                    }
                    else 
                    {
                        value = 127;
                    }
                    break;
                case 3:
                    if (line.compare("?") != 0){
                        f = stof(line);
                        if (f < 20) {
                            value = 1;
                        }
                        else if (f >= 0 && f < 4) {
                            value = 2;
                        }
                        else if (f >= 4 && f < 8) {
                            value = 4;
                        }
                        else if (f >= 8 && f < 12) {
                            value = 8;
                        }
                        else if (f >= 12) {
                            value = 16;
                        }
                    }
                    else 
                    {
                        value = 31;
                    }
                    break;
                case 4:
                    if (line.compare("u") == 0) {
                        value = 1;
                    }
                    else if (line.compare("y") == 0) {
                        value = 2;
                    }
                    else if (line.compare("l") == 0) {
                        value = 4;
                    }
                    else if (line.compare("t") == 0) {
                        value = 8;
                    }
                    else {
                        value = 15;
                    }
                    break;
                case 5:
                    if (line.compare("g") == 0) {
                        value = 1;
                    }
                    else if (line.compare("p") == 0) {
                        value = 2;
                    }
                    else if (line.compare("gg") == 0){
                        value = 4;
                    }
                    else {
                        value = 7;
                    }
                    break;
                case 6:
                    if (line.compare("c") == 0) {
                        value = 1;
                    }
                    else if (line.compare("d")== 0) {
                        value = 2;
                    }
                    else if (line.compare("cc") == 0){
                        value = 4;
                    }
                    else if (line.compare("i") == 0){
                        value = 8;
                    }
                    else if (line.compare("j") == 0){
                        value = 16;
                    }
                    else if (line.compare("k") == 0){
                        value = 32;
                    }
                    else if (line.compare("m") == 0){
                        value = 64;
                    }
                    else if (line.compare("r") == 0){
                        value = 128;
                    }
                    else if (line.compare("q") == 0){
                        value = 256;
                    }
                    else if (line.compare("w") == 0){
                        value = 512;
                    }
                    else if (line.compare("x") == 0){
                        value = 1024;
                    }
                    else if (line.compare("e") == 0){
                        value = 2048;
                    }
                    else if (line.compare("aa") == 0){
                        value = 4096;
                    }
                    else if (line.compare("ff") == 0){
                        value = 8192;
                    }
                    else {
                        value = 16383;
                    }
                    break;
                case 7:
                    if (line.compare("v") == 0){
                        value = 1;
                    }
                    else if (line.compare("h") == 0) {
                        value = 2;
                    }
                    else if (line.compare("bb") == 0){
                        value = 4;
                    }
                    else if (line.compare("j") == 0) {
                        value = 8;
                    }
                    else if (line.compare("n") == 0) {
                        value = 16;
                    }
                    else if (line.compare("z") == 0) {
                        value = 32;
                    }
                    else if (line.compare("dd") == 0) {
                        value = 64;
                    }
                    else if (line.compare("ff") == 0) {
                        value = 128;
                    }
                    else if (line.compare("oo") == 0) {
                        value = 256;
                    }
                    else {
                        value = 511;
                    }
                    break;
                case 8:
                    if (line.compare("?") != 0){
                        f = stof(line);
                        if (f <= 0) {
                            value = 1;
                        }
                        else if (f > 0 && f <= 2) {
                            value = 2;
                        }
                        else if (f > 2 && f <= 10) {
                            value = 4;
                        }
                        else if (f > 10) {
                            value = 8;
                        }
                    }
                    else
                    {
                        value = 15;
                    }
                    break;
                case 9:
                    if (line.compare("t") == 0) {
                        value = 1;
                    }
                    else if (line.compare("f") == 0) {
                        value = 2;
                    }
                    else {
                        float
                                value = 3;
                    }
                    break;
                case 10:
                    if (line.compare("t") == 0) {
                        value = 1;
                    }
                    else if (line.compare("f") == 0) {
                        value = 2;
                    }
                    else {
                        value = 3;
                    }
                    break;
                case 11:
                    if (line.compare("?") != 0){
                        f = stof(line);
                        if (f >= 0 && f < 1) {
                            value = 1;
                        }
                        else if (f >= 1 && f < 3) {
                            value = 2;
                        }
                        else if (f >= 3) {
                            value = 4;
                        }
                    }
                    else
                    {
                        value = 7;
                    }
                    break;
                case 12:
                    if (line.compare("t") == 0) {
                        value = 1;
                    }
                    else if (line.compare("f") == 0) {
                        value = 2;
                    }
                    else {
                        value = 3;
                    }
                    break;
                case 13:
                    if (line.compare("g") == 0){
                        value = 1;
                    }
                    else if (line.compare("p") == 0) {
                        value = 2;
                    }
                    else if (line.compare("s") == 0) {
                        value = 4;
                    }
                    else {
                        value = 7;
                    }
                    break;
                case 14:
                    if (line.compare("?") != 0){
                        f = stof(line);
                        if (f >= 0 && f < 100) {
                            value = 1;
                        }
                        else if (f >= 100 && f < 200) {
                            value = 2;
                        }
                        else if (f >= 200 && f < 300) {
                            value = 4;
                        }
                        else if (f >= 300 && f < 500) {
                            value = 8;
                        }
                        else if (f >= 500) {
                            value = 16;
                        }
                    }
                    else
                    {
                        value = 31;
                    }
                    break;
                case 15:
                    if (line.compare("?") != 0){
                        f = stof(line);
                        if (f >= 0 && f < 3) {
                            value = 1;
                        }
                        else if (f >= 3 && f < 200) {
                            value = 2;
                        }
                        else if (f >= 200) {
                            value = 4;
                        }
                    }
                    else 
                    {
                        value = 7;
                    }
                    break;
                case 16:
                    if (line.compare("+") == 0) {
                        value = 1;
                    }
                    else if (line.compare("-") == 0) {
                        value = 0;
                    }
                    break;
            }
            train.push_back(value);
            aPos++;
            if (aPos >= 16) {
                aPos = 1;
                data.push_back(train);
            }

        }


    }
            file.close();
    return data;
}

int main() {
    srand(time(NULL));    //Random seed initialization

    // Train Set from file
    vector< vector<int> > data =  readTrainingSet("Datos");

    vector< vector<int> > trainSet, testSet;

    for (vector< vector<int> >::iterator it = data.begin(); it != data.end(); ++it)
    {
        int probabilidad = rand() % 10 + 1;
        if (probabilidad > 7){
            testSet.push_back(*it);
        }
        else
        {
            trainSet.push_back(*it);
        }

    }

    std::cout << '\n';


    int ruleSize = 69; // the size of each rule, including the classification

    // Vector with the rules Sizes
    vector<int> attrS;
    attrS.push_back(2);
    attrS.push_back(7);
    attrS.push_back(5);
    attrS.push_back(4);
    attrS.push_back(3);
    attrS.push_back(14);
    attrS.push_back(9);
    attrS.push_back(4);
    attrS.push_back(2);
    attrS.push_back(2);
    attrS.push_back(3);
    attrS.push_back(2);
    attrS.push_back(3);
    attrS.push_back(5);
    attrS.push_back(3);
    attrS.push_back(-1);

    vector<Individual> pops;
    for (int i = 0; i < ORIGINAL_SIZE; i++) {
        pops.push_back(Individual(ruleSize));
    }


    Population gaStructure(pops, trainSet, attrS);        //Palabra aguda

    gaStructure.fitOut();
    int i = 0;
    cout << gaStructure.bigWeight << endl;


    for (int i = 0; i < 1000; i ++) {
        cout << "iteracion : " << i << ", BW: " << gaStructure.bigWeight << ", UBW: "
            << gaStructure.bigUnfitWeight << endl;
        cout << "pop size: " << gaStructure.pop.size() << ", ruleSize: "
            << gaStructure.pop.at(199).setSize << endl;
        gaStructure.newGen(1,1);
    }

    cout << gaStructure.bigWeight << endl;

}

