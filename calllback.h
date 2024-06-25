#pragma once
//  calllback.h

#ifndef calllback_h
#define calllback_h
extern vector<string> lazy_cuts;

#include <ilcplex/ilocplex.h>
#include <string>

#include "Model.h"
ILOSTLBEGIN

IloEnv env_;
IloModel mod_;

ILOLAZYCONSTRAINTCALLBACK4(newFlowCutsCallback, IloNumVarArray, H_, IloNumVarArray, h_, IloCplex, cplex_, Input, my_input)
{
    
    if (true)
    {
        vector<double> arcs_capacities;  //  This vector will contain the values of H_
        arcs_capacities.assign(my_input.arcs.size(), 0);
        
        vector<double> arcs_loads;  //  This vector will contain the values of h_
        arcs_loads.assign(my_input.arcs.size(), 0);
        
        IloEnv currentEnv = getEnv();
        
        IloInt i;
        IloInt NUM_ARCS = my_input.N_ARCS;
        
        for (i = 0; i < NUM_ARCS; ++i)
        {
            arcs_capacities[i] = getValue(H_[i]);
            arcs_loads[i] = getValue(h_[i]);
        }
        
        for (int j = 1; j < my_input.N_VERTICES; ++j)
        {
            vector<double> flow;
            flow.assign(my_input.N_ARCS, 0); // Initializes a zero vector flow
            
            FordFulkerson FF;
            vector<int> labelled_nodes;

            labelled_nodes = FF.max_flow(my_input.N_VERTICES, my_input.N_ARCS, 0, j, my_input.in_arcs, my_input.out_arcs, my_input.arcs, arcs_capacities, flow);
            
            int labels_counter = 0;
            vector<int> boolean_labelled_indexes;
            boolean_labelled_indexes.assign(my_input.N_VERTICES, 0);
            
            for (int j = 0; j < labelled_nodes.size(); ++j)
            {
                boolean_labelled_indexes[labelled_nodes[j]] = 1;
                labels_counter++;
            }
            
            // Add new constraints
            bool flag_entering_vehicles = false;
            
            IloExpr expr3(currentEnv);
            double solution_value = 0.0;
            string lazy_cut;
            int first = 0;
            int counter = 0;
            for (int j = 0; j < my_input.N_ARCS; ++j)
            {
                // Arcs going into the non reachable set of nodes
                if ((boolean_labelled_indexes[my_input.arcs[j].tail] == 1) &&
                    (boolean_labelled_indexes[my_input.arcs[j].head] == 0) )
                {
                    expr3 += my_input.TMAX * H_[my_input.arcs[j].index];
                    solution_value += my_input.TMAX* arcs_capacities[my_input.arcs[j].index];
                    if (first == 0)
                    {
                        counter++;
                        if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                        lazy_cut +=   to_string(my_input.TMAX) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                        first = 1;
                    }
                    else
                    {
                        counter++;
                        if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                        lazy_cut += " + " + to_string(my_input.TMAX) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                    }
                    flag_entering_vehicles = true;
                }
                
                // Arcs with both extremes in the non reachable set of nodes
                if ((boolean_labelled_indexes[my_input.arcs[j].tail] == 0) &&
                    (boolean_labelled_indexes[my_input.arcs[j].head] == 0))
                {
                    expr3 -= my_input.arcs[j].cve_cost * H_[my_input.arcs[j].index];
                    solution_value -= my_input.arcs[j].cve_cost * arcs_capacities[my_input.arcs[j].index];
                    
                    if (first == 0)
                    {
                        counter++;
                        if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                        lazy_cut += "- "  + to_string(my_input.arcs[j].cve_cost) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                        first = 1;
                    }
                    else
                    {
                        counter++;
                        if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                        lazy_cut += " - " + to_string(my_input.arcs[j].cve_cost) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                    }
                }
                
                // Arcs going out from the non reachable set of nodes
                if ((boolean_labelled_indexes[my_input.arcs[j].tail] == 0) &&
                    (boolean_labelled_indexes[my_input.arcs[j].head] == 1))
                {
                    if (first == 0)
                    {
                        counter++;
                        if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                        lazy_cut += "- " + to_string(my_input.arcs[j].cve_cost) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                        first = 1;
                    }
                    else
                    {
                        counter++;
                        if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                        lazy_cut += " - " + to_string(my_input.arcs[j].cve_cost) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                    }
                    expr3 -= my_input.arcs[j].cve_cost * H_[my_input.arcs[j].index];
                    solution_value -= my_input.arcs[j].cve_cost * arcs_capacities[my_input.arcs[j].index];
                }
            }
            
            if (solution_value < 0 )
            {
                add(expr3 >= 0).end();
                
                if (std::count(lazy_cuts.begin(), lazy_cuts.end(), lazy_cut) == 0)
                {
                    lazy_cuts.push_back(lazy_cut);
                }
            }
        }
    }
    double EPS = 0.0000001;
    
    // We need to copy the solution in a vector of capacities
    vector<double> arcs_capacities;  //  This vector will contain the values of H_
    arcs_capacities.assign(my_input.arcs.size(), 0.0);
    
    vector<double> arcs_loads;  //  This vector will contain the values of h_
    arcs_loads.assign(my_input.arcs.size(), 0.0);
    
    IloEnv currentEnv = getEnv();
    
    IloInt i;
    IloInt NUM_ARCS = my_input.N_ARCS;
    
    for (i = 0; i < NUM_ARCS; ++i)
    {
        arcs_capacities[i] = getValue(H_[i]);
        arcs_loads[i] = getValue(h_[i]);
        if (arcs_capacities[i] <= EPS)
        {
            arcs_capacities[i] = 0.0;
        }
        if (arcs_loads[i] <= EPS)
        {
            arcs_loads[i] = 0.0;
        }
    }
    
    // Auxiliary graph
    int aux_N_VERTICES = my_input.N_VERTICES;
    int aux_N_ARCS = my_input.N_ARCS;
    std::vector<std::vector<int> > aux_in_arcs = my_input.in_arcs;  // in_arcs[i] is the vector with the indices of the arcs with head i
    std::vector<std::vector<int> > aux_out_arcs = my_input.out_arcs; // out_arcs[i] is the vector with the indices of the arcs with tail i
    std::vector<arc> aux_arcs = my_input.arcs;
    vector<double> aux_arcs_capacities = arcs_capacities;
    vector<double> aux_arcs_loads = arcs_loads;
    
    // Artificial sink node
    int sink_index = my_input.N_VERTICES;
    std::vector<int> sink_in_arcs;  // Extra space to the edges going into the sink
    aux_in_arcs.push_back(sink_in_arcs);
    std::vector<int> sink_out_arcs; // Extra space to the edges going out from the sink
    aux_out_arcs.push_back(sink_out_arcs);
    aux_N_VERTICES++;
    
    // For each edge e:=(x, y) in H_[i] with positive flow we add one new vertex xy and three new edges, xy->x, y->xy, xy->sink
    for (i = 0; i < NUM_ARCS; ++i)
    {
        //If the arc has positive flow
        if (arcs_capacities[i] > EPS)
        {
            // adds the corresponding space for the new vertex in the in-arcs vector
            std::vector<int> xy_in_arcs;
            aux_in_arcs.push_back(xy_in_arcs);
            
            // adds the corresponding space for the new vertex in the out-arcs vector
            std::vector<int> xy_out_arcs;
            aux_out_arcs.push_back(xy_out_arcs);
            
            arc exy_x;
            exy_x.tail = aux_N_VERTICES;
            exy_x.head = aux_arcs[i].tail;
            exy_x.index = aux_N_ARCS;
            exy_x.cce_cost = aux_arcs[i].tail;
            exy_x.cve_cost = aux_arcs[i].head;
            aux_arcs.push_back(exy_x);
            aux_out_arcs[aux_N_VERTICES].push_back(aux_N_ARCS);
            aux_in_arcs[aux_arcs[i].tail].push_back(aux_N_ARCS);
            aux_arcs_capacities.push_back( std::numeric_limits<double>::max() );
            aux_N_ARCS++;
            
            arc ey_xy;
            ey_xy.tail = aux_arcs[i].head;
            ey_xy.head = aux_N_VERTICES;
            ey_xy.index = aux_N_ARCS;
            ey_xy.cce_cost = aux_arcs[i].tail;
            ey_xy.cve_cost = aux_arcs[i].head;
            aux_arcs.push_back(ey_xy);
            aux_out_arcs[aux_arcs[i].head].push_back(aux_N_ARCS);
            aux_in_arcs[aux_N_VERTICES].push_back(aux_N_ARCS);
            aux_arcs_capacities.push_back(arcs_capacities[i] * my_input.arcs[i].cce_cost / my_input.TMAX);
            aux_N_ARCS++;
            
            arc exy_p;
            exy_p.tail = aux_N_VERTICES;
            exy_p.head = sink_index;
            exy_p.index = aux_N_ARCS;
            exy_p.cce_cost = aux_arcs[i].tail; //
            exy_p.cve_cost = aux_arcs[i].head; //
            aux_arcs.push_back(exy_p);
            aux_out_arcs[aux_N_VERTICES].push_back(aux_N_ARCS);
            aux_in_arcs[sink_index].push_back(aux_N_ARCS);
            aux_arcs_capacities.push_back( arcs_capacities[i]*my_input.arcs[i].cce_cost /my_input.TMAX ) ;
            aux_N_ARCS++;
            
            aux_N_VERTICES++;
        }
    }
    
    vector<double> flow;
    flow.assign(aux_N_ARCS, 0); // Initialize a zero flow vector
    
    FordFulkerson FF;
    vector<int> labelled_nodes;
    labelled_nodes = FF.max_flow(aux_N_VERTICES, aux_N_ARCS, 0, sink_index, aux_in_arcs, aux_out_arcs, aux_arcs, aux_arcs_capacities, flow);
    
    while (true)
    {
        if (labelled_nodes.back() >= my_input.N_VERTICES)
        {
            labelled_nodes.pop_back();
        }
        else
        {
            break;
        }
    }
    
    int labels_counter = 0;
    vector<int> boolean_labelled_indexes;
    boolean_labelled_indexes.assign(my_input.N_VERTICES, 0);
    
    for (int j = 0; j < labelled_nodes.size(); ++j)
    {
        boolean_labelled_indexes[labelled_nodes[j]] = 1;
        labels_counter++;
    }
    
    // Search for new constraints
    bool flag_entering_vehicles = false;
    
    IloExpr expr3(currentEnv);
    double solution_value = 0.0;
    string lazy_cut;
    int first = 0;
    int counter = 0;
    for (int j = 0; j < my_input.N_ARCS; ++j)
    {
        // Arcs going into the non reachable set of nodes
        if ((boolean_labelled_indexes[my_input.arcs[j].tail] == 1) &&
            (boolean_labelled_indexes[my_input.arcs[j].head] == 0))
        {
            expr3 += my_input.TMAX * H_[my_input.arcs[j].index];
            solution_value += my_input.TMAX * arcs_capacities[my_input.arcs[j].index];
            if (first == 0)
            {
                counter++;
                if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                lazy_cut += to_string(my_input.TMAX) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                first = 1;
            }
            else
            {
                counter++;
                if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                lazy_cut += " + " + to_string(my_input.TMAX) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
            }
            flag_entering_vehicles = true;
        }
        
        // Arcs with both extremes in the non reachable set of nodes
        if ((boolean_labelled_indexes[my_input.arcs[j].tail] == 0) &&
            (boolean_labelled_indexes[my_input.arcs[j].head] == 0))
        {
            expr3 -= my_input.arcs[j].cve_cost * H_[my_input.arcs[j].index];
            solution_value -= my_input.arcs[j].cve_cost * arcs_capacities[my_input.arcs[j].index];
            
            if (first == 0)
            {
                counter++;
                if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                lazy_cut += "- " + to_string(my_input.arcs[j].cve_cost) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                first = 1;
            }
            else
            {
                counter++;
                if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                lazy_cut += " - " + to_string(my_input.arcs[j].cve_cost) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
            }
        }
        
        // Arcs going out from the non reachable set of nodes
        if ((boolean_labelled_indexes[my_input.arcs[j].tail] == 0) &&
            (boolean_labelled_indexes[my_input.arcs[j].head] == 1))
        {
            if (first == 0)
            {
                counter++;
                if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                lazy_cut += "- " + to_string(my_input.arcs[j].cve_cost) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
                first = 1;
            }
            else
            {
                counter++;
                if ((counter % 7 == 0) && (counter > 0)) { lazy_cut += "\n            "; }
                lazy_cut += " - " + to_string(my_input.arcs[j].cve_cost) + " H_" + to_string(my_input.arcs[j].tail) + "_" + to_string(my_input.arcs[j].head);
            }
            expr3 -= my_input.arcs[j].cve_cost * H_[my_input.arcs[j].index];
            solution_value -= my_input.arcs[j].cve_cost * arcs_capacities[my_input.arcs[j].index];
        }
    }
    
    if (solution_value < 0)
    {
        add(expr3 >= 0).end();
        
        if (std::count(lazy_cuts.begin(), lazy_cuts.end(), lazy_cut) == 0)
        {
            lazy_cuts.push_back(lazy_cut);
        }
    }
    return;
}

#endif /* calllback_h */
