#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <time.h> //Used for random operator power

using namespace std;

vector<vector<int> > readTrainingSet(string fileName){

    vector<vector<int> > data;  //Training Set
    vector<int> train; 
    train.clear();
    int value;
    string line;
    
    ifstream file ("Datos");
    
    if (file.is_open())
    {
    int aPos = 1;
        while (getline(file, line, ','))
        {
            aPos++;
            switch (aPos){
                case 1:
                    if (line == "a")
                    {
                        value = 1;
                    }
                    else if (line == "b")
                    {
                        value = 2;
                    }
                    else
                    {
                        value = 3;
                    }
                    break;
                case 2:
                    float f = stof(line);
                    if (f < 20)
                    {
                        value = 1;
                    }
                    else if (f >= 20 && f < 25)
                    {
                        value = 2;
                    }
                    else if (f >= 25 && f < 35)
                    {
                        value = 4;
                    }
                    else if (f >= 35 && f < 40)
                    {
                        value = 8;
                    }
                    else if (f >= 40 && f < 50)
                    {
                        value = 16;
                    }
                    else if (f >= 50 && f < 60)
                    {
                        value = 32;
                    }
                    else if (f >= 60)
                    {
                        value = 64;
                    }
                    else
                    {
                        value = 127;
                    }
                    break;
                case 3:
                    float f = stof(line);
                    if (f < 20)
                    {
                        value = 1;
                    }
                    else if (f >= 0 && f < 4)
                    {
                        value = 2;
                    }
                    else if (f >= 4 && f < 8)
                    {
                        value = 4;
                    }
                    else if (f >= 8 && f < 12)
                    {
                        value = 8;
                    }
                    else if (f >= 12)
                    {
                        value = 16;
                    }
                    else
                    {
                        value = 31;
                    }
                    break;
                case 4:
                    if (line == "u")
                    {
                        value = 1;
                    }
                    else if (line == "y")
                    {
                        value = 2;
                    }
                    else if (line == "l")
                    {
                        value = 4;
                    }
                    else if (line == "t")
                    {
                        value = 8;
                    }
                    else
                    {
                        value = 15;
                    }
                    break;
                case 5:
                    if (line == "g")
                    {
                        value = 1;
                    }
                    else if (line == "p")
                    {
                        value = 2;
                    }
                    else if (line == "gg")
                    {
                        value = 4;
                    }
                    else
                    {
                        value = 7;
                    }
                    break;
                case 6:
                    if (line == "c")
                    {
                        value = 1;
                    }
                    else if (line == "d")
                    {
                        value = 2;
                    }
                    else if (line == "cc")
                    {
                        value = 4;
                    }
                    else if (line == "i")
                    {
                        value = 8;
                    }
                    else if (line == "j")
                    {
                        value = 16;
                    }
                    else if (line == "k")
                    {
                        value = 32;
                    }
                    else if (line == "m")
                    {
                        value = 64;
                    }
                    else if (line == "r")
                    {
                        value = 128;
                    }
                    else if (line == "q")
                    {
                        value = 256;
                    }
                    else if (line == "w")
                    {
                        value = 512;
                    }
                    else if (line == "x")
                    {
                        value = 1024;
                    }
                    else if (line == "e")
                    {
                        value = 2048;
                    }
                    else if (line == "aa")
                    {
                        value = 4096;
                    }
                    else if (line == "ff")
                    {
                        value = 8192;
                    }
                    else
                    {
                        value = 16383;
                    }
                    break;
                case 7:
                    if (line == "v")
                    {
                        value = 1;
                    }
                    else if (line == "h")
                    {
                        value = 2;
                    }
                    else if (line == "bb")
                    {
                        value = 4;
                    }
                    else if (line == "j")
                    {
                        value = 8;
                    }
                    else if (line == "n")
                    {
                        value = 16;
                    }
                    else if (line == "z")
                    {
                        value = 32;
                    }
                    else if (line == "dd")
                    {
                        value = 64;
                    }
                    else if (line == "ff")
                    {
                        value = 128;
                    }
                    else if (line == "oo")
                    {
                        value = 256;
                    }
                    else
                    {
                        value = 511;
                    }
                    break;
                case 8:
                    float f = stof(line);
                    if (f <= 0)
                    {
                        value = 1;
                    }
                    else if (f > 0 && f <= 2)
                    {
                        value = 2;
                    }
                    else if (f > 2 && f <= 10)
                    {
                        value = 4;
                    }
                    else if (f > 10)
                    {
                        value = 8;
                    }
                    else 
                    {
                        value = 15;
                    }
                    break;
                case 9:
                    if (line == "t")
                    {
                        value = 1;
                    }
                    else if (line == "f")
                    {
                        value = 2;
                    }
                    else
                    {
                        value = 3;
                    }
                    break;
                case 10:
                    if (line == "t")
                    {
                        value = 1;
                    }
                    else if (line == "f")
                    {
                        value = 2;
                    }
                    else
                    {
                        value = 3;
                    }
                    break;
                case 11:
                    float f = stof(line);
                    if (f >= 0 && f < 1)
                    {
                        value = 1;
                    }
                    else if (f >= 1 && f < 3)
                    {
                        value = 2;
                    }
                    else if (f >= 3)
                    {
                        value = 4;
                    }
                    else
                    {
                        value = 7;
                    }
                    break;
                case 12:
                    if (line == "t")
                    {
                        value = 1;
                    }
                    else if (line == "f")
                    {
                        value = 2;
                    }
                    else
                    {
                        value = 3;
                    }
                    break;
                case 13:
                    if (line == "g")
                    {
                        value = 1;
                    }
                    else if (line == "p")
                    {
                        value = 2;
                    }
                    else if (line == "s")
                    {
                        value = 4;
                    }
                    else
                    {
                        value = 7;
                    }
                    break;
                case 14:
                    float f = stof(line);
                    if (f >= 0 && f < 100)
                    {
                        value = 1;
                    }
                    else if (f >= 100 && f < 200)
                    {
                        value = 2;
                    }
                    else if (f >= 200 && f < 300)
                    {
                        value = 4;
                    }
                    else if (f >= 300 && f < 500)
                    {
                        value = 8;
                    }
                    else if (f >= 500)
                    {
                        value = 16;
                    }
                    else
                    {
                        value = 31;
                    }
                    break;
                case 15:
                    float f = stof(line);
                    if (f >= 0 && f < 3)
                    {
                        value = 1;
                    }
                    else if (f >= 3 && f < 200)
                    {
                        value = 2;
                    }
                    else if (f >= 200 )
                    {
                        value = 4;
                    }
                    else
                    {
                        value = 7;
                    }
                    break;
                case 16:
                    if (line == "+")
                    {
                        value = 1;
                    }
                    else if (line == "-")
                    {
                        value = 0;
                    }
                    break;
            }
            train.push_back(value);
            if (aPos >= 16) {
                aPos = 1;
                data.push_back(train);
            }
            else
            {
                aPos++;
            }
        
        file.close();
    }
    return data;
}

int main() {

    readTrainingSet("Datos.txt");
}
