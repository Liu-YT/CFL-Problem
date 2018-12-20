#include <iostream>
#include "ioHelper.hpp"
#include "problem.hpp"

using namespace std;

int main() {
    problem* p = readProblem("p1");
    cout << p->numOfCus << " " << p->numOfFac << endl;
    delete p;
    return 0;
}