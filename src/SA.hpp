#ifndef SA_HPP
#define SA_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include "problem.hpp"
using namespace std;

#define SEARCHTIMES 5000

// 计时
clock_t startTime, endTime;

// 初始温度
double T;

// 温度下降速率
double alpha;

// 总的消耗
long long int totalCost;

// 工厂可用容量
vector<int> freeCapacity;

// 客户对应的工厂
vector<int> cusToFac;

// 初始化全局变量
void init(problem* p) {
    T = 100;
    alpha = 0.9;
    totalCost = 0;
    freeCapacity = p->facCapacity;
    cusToFac = vector<int>(p->numOfCus);
}

// 计算当前解的费用
int calCost(problem* p, vector<int>& cus2fac, vector<int>& fCap) {
    int cost = 0;
    // fixed cost
    for(int i = 0; i < p->numOfFac; ++i)
        if (fCap[i] != p->facCapacity[i])
            cost += p->facFixedCost[i];
    // assignment cost
    for(int i = 0; i < p->numOfCus; ++i)
        cost += p->useCost[i][cus2fac[i]];
    return cost;  
}

// 输出必要信息
void printDebugInfo(problem* p) {
    cout << "Problem: " << p->file << endl;
    cout << "Time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s"  << endl;
    cout << "Total Cost: " << totalCost << endl;
    cout << "Facility Status: ";
    for (int i = 0; i < p->numOfFac; ++i)
        cout << (freeCapacity[i] != p->facCapacity[i]) << " ";
    cout << endl;
    cout << "Customer State: ";
    for (int i = 0; i < p->numOfCus; ++i)
        cout << cusToFac[i] << " ";
    cout << endl << endl;
}

void SA(problem* p) {
    startTime = clock();

    // 初始化信息
    init(p);

    // 初始解
    for(int i = 0; i < p->numOfCus; ++i) {
        for(int j = 0; j < p->numOfFac; ++j) {
            if(p->cusDemand[i] <= freeCapacity[j]) {
                if(freeCapacity[j] == p->facCapacity[j]) 
                    totalCost += p->facFixedCost[j];
                freeCapacity[j] -= p->cusDemand[i];
                totalCost += p->useCost[i][j];
                cusToFac[i] = j;
                break;
            }
        }
    }

    // 模拟退火
    while(T > 0.01) {
        /* 扰动获得寻找较优解，并且可能接收差解 */
        int i = 0;
        while(i++ < SEARCHTIMES) {
            int first = rand() % (p->numOfCus);
            int second = rand() % (p->numOfCus - first) + first;
            vector<int> newCusToFac = cusToFac;
            vector<int> newFreeCapacity = freeCapacity;

            // 回收容量
            for(int j = first; j <= second; ++j)
                newFreeCapacity[newCusToFac[j]] += p->cusDemand[j];

            // 随机分配一个新的工厂
            for(int j = first; j <= second; ++j) {
                int newFac = rand() % (p->numOfFac);
                while (p->cusDemand[j] > newFreeCapacity[newFac])
                    newFac = rand() % (p->numOfFac);
                newCusToFac[j] = newFac;
                newFreeCapacity[newFac] -= p->cusDemand[j];
            }

            int cost = calCost(p, newCusToFac, newFreeCapacity);
            double probability = (rand() % 100) * 1.0 / 100;

            // 处理当前解，一定概率接受
            if (cost <= totalCost || probability < exp(-abs(cost-totalCost) / T)) {
                cusToFac = newCusToFac;
                freeCapacity = newFreeCapacity;
                totalCost = cost;
            }
        }
        // 退火
        T *= alpha;
    }

    endTime = clock();

    printDebugInfo(p);
}

#endif

