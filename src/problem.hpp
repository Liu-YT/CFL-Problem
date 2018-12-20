#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <Map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct problem {
    // record
    string file;

    /* data */

    int numOfFac;
    
    int numOfCus;

    // facility capacity
    vector<int> facCapacity;

    // facility fixed cost of opening
    vector<int> facFixedCost;

    vector<int> cusDemand;
    
    // customer, cusCost
    map<int, vector<int> > useCost;

    problem(string file) {
        this->file = file;
    }
};

#endif