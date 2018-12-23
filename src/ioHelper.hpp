#ifndef IO_HELPER_HPP
#define IO_HELPER_HPP

#include <string>
#include <fstream>
#include "problem.hpp"
using namespace std;

problem* readProblem(string filename) {
    problem* p = new problem(filename);
    filename = "./instances/" + filename;
    ifstream readFile(filename, ios::in);

    if(!readFile) {
        cerr << "Open file faild" << endl;
    }

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

void writeResult(problem *p, long long int result, double costTime, vector<int> freeCapacity, vector<int> cusToFac, string filename = "SA") {
    filename = "./result/csv/" + filename + ".csv";
    ofstream writeFile(filename, ios::app);
    writeFile << p->file << "," << result << "," << costTime << ",";
    for (int i = 0; i < p->numOfFac; ++i)
        writeFile << (freeCapacity[i] != p->facCapacity[i]) << " ";
    writeFile << ",";
    for (int i = 0; i < p->numOfCus; ++i)
        writeFile << cusToFac[i] << " ";
    writeFile << endl;
    writeFile.close();
}

#endif