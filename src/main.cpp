#include <iostream>
#include "ioHelper.hpp"
#include "problem.hpp"
#include "SA.hpp"

using namespace std;

int main() {
    for(int i = 1; i <= 71; ++i) {
        problem *p = readProblem("p" + to_string(i));
        SA(p);
        delete p;
    }
    return 0;
}