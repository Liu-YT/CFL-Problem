#include <iostream>
#include <algorithm>
#include "ioHelper.hpp"
#include "problem.hpp"
#include "GA.hpp"
#include "SA.hpp"

using namespace std;

#define defaultMethod "SA" 

int main(int argc, char *argv[]) {
    string method = defaultMethod;
    bool exportResult = false;

    if(argc >= 2) 
        method = argv[1];
    transform(method.begin(), method.end(), method.begin(), ::toupper);

    if(argc == 3 && string(argv[2]) == "export") {
        exportResult = true;
        string filename = "./result/csv/" + method + ".csv";
        ofstream writeFile(filename, ios::out);
        // header
        writeFile << "Problem" << "," << "Result" << "," << "Time(s)" << "," << "Facility Status" << "," << "Customer Status" << endl;
    }

    // 指定使用GA就使用GA运算，否则都使用SA
    for(int i = 1; i <= 71; ++i) {
        problem *p = readProblem("p" + to_string(i));      
        if(method == "GA")
            GA(p, exportResult);
        else
            SA(p, exportResult);
        delete p;
    }
    return 0;
}