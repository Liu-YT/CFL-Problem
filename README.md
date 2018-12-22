# Capacitated Facility Location Problem 

## Introducation

Suppose there are n facilities and m customers. We wish to choose: 
* which of the n facilities to open 
* the assignment of customers to facilities 
 
The objective is to minimize the sum of the opening cost and the assignment cost. 

**The total demand assigned to a facility must not exceed its capacity.** 

## Environment

* `Win 10`
* `C++ 11`


## Usage

### Compile
```
g++ ./src/main.cpp -o solution -std=c++11
```
### Run
**Use SA by default if no method is specified**
* `SA`
    ```
    solution SA
    ```
* `GA`
    ```
    solution GA
    ```
* Export `csv`
    ```
    # Export GA results
    solution GA export

    #Export SA results
    solution SA export
    ```