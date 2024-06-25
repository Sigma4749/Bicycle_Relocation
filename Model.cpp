#include "Model.h"
#include <stdio.h>
#include <string>
using namespace std;

void Model::allocVar()
{
    int N = my_input_->N_VERTICES;
    int E = my_input_->N_ARCS;
    cout <<"The model has "<< N << " vertices and "<< E << " arcs "<< endl;
    
    H_ = IloNumVarArray(env_, E + 1);
    h_ = IloNumVarArray(env_, E + 1);
    
    int k = 0;
    for (int i = 0; i < E ; ++i)
    {
        string name;
        name = "H_" + to_string((my_input_->arcs[i]).tail) + "_" + to_string((my_input_->arcs[i]).head);
        H_[k] = IloNumVar(env_, 0, IloInfinity, IloNumVar::Int, name.c_str());
        name = "h_" + to_string((my_input_->arcs[i]).tail) + "_" + to_string((my_input_->arcs[i]).head);
        h_[k] = IloNumVar(env_, 0, IloInfinity, IloNumVar::Int, name.c_str());
        ++k;
    }
}

void Model::buildModel(int CAP, int TMAX)
{
    // First kind of constraints: conservation of flow H
    for(int i = 0; i < my_input_->N_VERTICES; ++i)
    {
        int counter = 0;
        IloExpr expr1(env_);
        for(int j = 0; j < my_input_->in_arcs[i].size(); ++j)
        {
            expr1 -= H_[ (my_input_->arcs[my_input_->in_arcs[i][j]]).index];
            counter++;
        }
        
        for(int j = 0; j < my_input_->out_arcs[i].size();++j)
        {
            expr1 += H_[ (my_input_->arcs[my_input_->out_arcs[i][j]]).index];
            counter++;
        }
        if (counter > 0)
        {
            mod_.add( expr1 == 0);
        }
        expr1.end();
        
    }

    // Second kind of constraints: semiflow h -> sum of flows at vertex i is equal to v(i)
    for(int i = 0; i < my_input_->N_VERTICES; ++i)
    {
        IloExpr expr2(env_);
        for(int j = 0; j < my_input_->in_arcs[i].size();++j)
        {
            expr2 -= h_[ (my_input_->arcs[my_input_->in_arcs[i][j]]).index];
        }
        
        for(int j = 0; j < my_input_->out_arcs[i].size();++j)
        {
            expr2 += h_[ (my_input_->arcs[my_input_->out_arcs[i][j]]).index];
        }
        mod_.add( expr2 == my_input_->vertices_status[i]);
        expr2.end();
    }
    
    // Third kind of constraints:  h <= CAP * H
    for(int j = 0; j < my_input_->N_ARCS; ++j)
    {
        IloExpr expr3(env_);
        expr3 -= h_[ my_input_->arcs[j].index];
        expr3 += CAP * H_[my_input_->arcs[j].index];
        mod_.add( expr3 >= 0);
    }
    
    // Objective function
    IloExpr expr3(env_);
    for(int i = 0; i < my_input_->N_ARCS; ++i)
    {
        expr3 += (my_input_->arcs[i]).cce_cost * H_[(my_input_->arcs[i]).index];
        expr3 += (my_input_->arcs[i]).cve_cost * h_[(my_input_->arcs[i]).index];
    }
    mod_.add(IloMinimize(env_, expr3));
    expr3.end();
}

/* Export the model as an .lp file*/
void Model::writeModel()
{
    cplex_.exportModel("model.lp");
}

/* Creates and solve the model */
void Model::createSolve(int CAP, int TMAX)
{
    allocVar();
    buildModel(CAP, TMAX);
    writeModel();
}
