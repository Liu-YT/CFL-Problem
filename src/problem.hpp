#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

// 计时
clock_t startTime, endTime;

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

    // customer demand
    vector<int> cusDemand;

    // customer,  assignment of customers to facilities
    map<int, vector<int> > useCost;

    problem(string file) {
        this->file = file;
    }
};

#endif