#ifndef Model_h
#define Model_h

#include <iostream>
#include <vector>
#include <string>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
#include "Input.h"

class Model
{
public:
    IloEnv env_;
    IloModel mod_;
    IloCplex cplex_;
    Input *my_input_;
    IloNumVarArray H_; //
    IloNumVarArray h_; //
    vector<string> lazy_cuts_added;
    
    Model(Input* my_input)
    {
        my_input_ = my_input;
        env_ = IloEnv();
        mod_ = IloModel(env_);
        cplex_ = IloCplex(mod_);
    }
    
    void createSolve(int CAP, int TMAX);
    
private:
    void allocVar();
    void buildModel(int CAP, int TMAX);
    void solveModel();
    void writeModel();
};

#endif /* Model_h */
