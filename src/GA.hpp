#ifndef GA_HPP
#define GA_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "problem.hpp"
using namespace std;

// 种群大小
#define POSIZE 40

// 遗传代数
#define MAXGEN 200

// 交叉概率
#define PXOVER 0.8

// 变异概率
#define PMUTATION 0.2


// 个体定义
struct individual {
    // 客户对应的工厂
    vector<int> cusToFac;

    // 工厂可用的容量
    vector<int> freeCapacity;

    // 费用
    long long int cost;

    individual(problem* p) {
        cost = 0;
        freeCapacity = p->facCapacity;
        cusToFac = vector<int>(p->numOfCus);
    }

    individual &operator=(const individual &i) {
        this->cusToFac = i.cusToFac;
        this->freeCapacity = i.freeCapacity;
        this->cost = i.cost;
    }

    individual(individual* i) {
        this->cusToFac = i->cusToFac;
        this->freeCapacity = i->cusToFac;
        this->cost = i->cost;
    }
};

// 种群
vector<individual*> population;

// 释放内存
void freeMemory() {
    for(int i = POSIZE; i >= 0; --i)
        delete population[i];
    population.clear();
}

// 适应度计算，计算当前方案总费用
int evalute(problem *p, individual *ind) {
    ind->cost = 0;
    // fixed cost
    for (int i = 0; i < p->numOfFac; ++i) {
        if (ind->freeCapacity[i] != p->facCapacity[i]) {
            ind->cost += p->facFixedCost[i];
        }
    }

    // assignment cost
    for (int i = 0; i < p->numOfCus; ++i)
        ind->cost += p->useCost[i][ind->cusToFac[i]];
    return ind->cost;
}

// 初始化种群
void initPopulation(problem* p) {
    // 为了加速较优个体的产生，初始时候通过贪心加入一部分个体

    // 随机生成解
    for(int i = 0; i < POSIZE / 2; ++i) {
        individual* ind = new individual(p);
        for(int j = 0; j < p->numOfCus; ++j) {
            int fac = rand() % (p->numOfFac);
            while (p->cusDemand[j] > ind->freeCapacity[fac])
                fac = rand() % (p->numOfFac);
            ind->freeCapacity[fac] -= p->cusDemand[j];
            ind->cusToFac[j] = fac;
        } 
        evalute(p, ind);
        population.push_back(ind);   
        
    }

    // 贪心生成解
    vector<int> temp;
    for(int i = 0; i < p->numOfCus; ++i) temp.push_back(i);
    for(int i = 0; i < POSIZE / 2; ++i) {
        individual *ind = new individual(p);
        random_shuffle(temp.begin(), temp.end());
        for(int j = 0; j < p->numOfCus; ++j) {
            int mixCost = INT_MAX;
            for(int k = 0; k < p->numOfFac; ++k) {
                if (p->cusDemand[temp[j]] <= ind->freeCapacity[k] && mixCost >= p->useCost[temp[j]][k]) {
                    mixCost = p->useCost[temp[j]][k];
                    ind->freeCapacity[k] -= p->cusDemand[temp[j]];
                    ind->cusToFac[temp[j]] = k;
                }
            }
        }
        evalute(p, ind);
        population.push_back(ind);
    }
    random_shuffle(population.begin(), population.end());
}

// 选择，轮盘赌
void select(problem* p) {
    // 个体适应度，等于其费用的倒数
    vector<double> fitness;
    // 整体适应度
    double sumOfFitness = 0;

    for(int i = 0; i < POSIZE; ++i) {
        fitness.push_back(1.0 / population[i]->cost);
        sumOfFitness += fitness[i];
    }

    // 累计概率
    vector<double> cumPro;
    for(int i = 0; i < POSIZE; ++i) {
        if (i == 0)
            cumPro.push_back(fitness[i] / sumOfFitness);
        else
            cumPro.push_back(fitness[i] / sumOfFitness + cumPro[i-1]);
    }

    // 轮盘赌生成子代
    vector<individual* > newPopulation;
    for(int i = 0; i < POSIZE; ++i) {
        double pro = (rand() % 100) * 1.0 / 100;
        for(int j = 0; j < POSIZE; ++j) {
            if(cumPro[j] >= pro) {
                individual* ind = new individual(population[j]);
                newPopulation.push_back(ind);
                break;    
            }
        }
    }
    freeMemory();
    population = newPopulation;
}

// 判断交叉是否有效
bool validCover(problem* p, individual* ind) {
    ind->freeCapacity = p->facCapacity;
    for(int i = 0; i < p->numOfCus; ++i) {
        ind->freeCapacity[ind->cusToFac[i]] -= p->cusDemand[i];
        if(ind->freeCapacity[ind->cusToFac[i]] < 0) {
            delete ind;
            return false;
        }
    }
    return true;
}

// 交叉
void crosscover(problem* p) {
    // 两点交叉 
    vector<individual*> subPopulation;
    for(int i = 0; i < POSIZE / 2; ++i) {
        if ((rand() % 100) * 1.0 / 100 <= PXOVER) {
            int count = 0;
            while(count++ < 100) {
                individual *a = new individual(population[i*2]);
                individual *b = new individual(population[i*2+1]);
                int geneFirst = rand() % (p->numOfCus);
                int geneSecond = rand() % (p->numOfCus - geneFirst) + geneFirst;
                for(int k = geneFirst; k <= geneSecond; k++) {
                    int temp = a->cusToFac[k];
                    a->cusToFac[k] = b->cusToFac[k];
                    b->cusToFac[k] = temp;
                }

                if (validCover(p, a) && validCover(p, b)) {
                    evalute(p, a);
                    evalute(p, b);
                    subPopulation.push_back(a);
                    subPopulation.push_back(b);
                }
            }
        }
    }
    sort(subPopulation.begin(), subPopulation.end(), [&](const individual* a, const individual* b) -> bool{ return a->cost < b->cost;});
    int num = 0;
    for(int i = 0; i < subPopulation.size(); ++i) {
        for(int j = 0; j < POSIZE; ++j) {
            if(population[j]->cost > subPopulation[i]->cost) {
                individual* temp = population[j];
                population[j] = subPopulation[i];
                ++num;
                delete temp;
                break;
            }
        }
    }
    for(int i = num; i < subPopulation.size(); ++i)
        delete subPopulation[i];
}

// 变异
void mutate(problem* p) {
    for(int i = 0; i < POSIZE; ++i) {
        double pro = (rand() % 100) * 1.0 / 100;
        if(pro <= PMUTATION) {
            for(int j = 0; j < p->numOfCus; ++j) {
                int newFac = rand() % p->numOfFac;
                population[i]->freeCapacity[population[i]->cusToFac[j]] += p->cusDemand[j];
                while (p->cusDemand[j] > population[i]->freeCapacity[newFac])
                    newFac = rand() % (p->numOfFac);
                population[i]->freeCapacity[newFac] -= p->cusDemand[j];
                population[i]->cusToFac[j] = newFac;
            }
        }
    }
}

void GA(problem *p, bool exportResult) {

    startTime = clock();
    
    // 初始化种群
    initPopulation(p);
    
    // 模拟遗传
    int curGen = 0;
    while(curGen < MAXGEN) {
        // 选择
        select(p);
        // 交叉
        crosscover(p);
        // 变异
        mutate(p);
        ++curGen; 
    }
    endTime = clock();

    // 输出一些必要信息
    int best = INT_MAX;
    int bestId;
    for(int i = 0; i < POSIZE; ++i) {
        if(population[i]->cost < best) {
            best = population[i]->cost;
            bestId = i;
        }
    }
    cout << "Problem: " << p->file << endl;
    cout << "Time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    cout << "Total Cost: " << population[bestId]->cost << endl;
    cout << "Facility Status: ";
    for (int i = 0; i < p->numOfFac; ++i)
        cout << (population[bestId]->freeCapacity[i] != p->facCapacity[i]) << " ";
    cout << endl;
    cout << "Customer Status: ";
    for (int i = 0; i < p->numOfCus; ++i)
        cout << population[bestId]->cusToFac[i] << " ";
    cout << endl << endl;

    if (exportResult)
        writeResult(p, population[bestId]->cost, (double)(endTime - startTime) / CLOCKS_PER_SEC, population[0]->freeCapacity, population[0]->cusToFac, "GA");

    // 释放内存
    freeMemory();
}

#endif