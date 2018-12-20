#ifndef IO_HELPER_HPP
#define IO_HELPER_HPP

#include <string>
#include <fstream>
#include "problem.hpp"
using namespace std;

problem* readProblem(string filename) {
    filename = "./../Instances/" + filename;
    problem* p = new problem(filename);
    
    ifstream readFile(filename, ios::in);
    readFile >> p->numOfFac >> p->numOfCus;

    int a, b;
    // facilities info
    for(int i = 0; i < p->numOfFac; ++i) {
        readFile >> a >> b;
        p->facCapacity.push_back(a);
        p->facFixedCost.push_back(b);
    }

    // customers info
    for(int i = 0; i < p->numOfCus; ++i)  {
        readFile >> a;
        p->cusDemand.push_back(a);
    }
    

    // demand cost
    for(int i = 0; i < p->numOfFac; ++i) {
        for(int j = 0; j < p->numOfCus; ++j) {
            readFile >> a;
            p->useCost[j].push_back(a);
        }
    }

    readFile.close();
    return p;
}


#endif