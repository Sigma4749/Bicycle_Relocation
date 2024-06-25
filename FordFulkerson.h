#ifndef FordFulkerson_h
#define FordFulkerson_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Input.h"

/* Structure for the labels of vertices in the unsaturated paths the graph */
typedef struct label {
    int previous_node; // Previous node in the unsaturated path
    int previous_arc; // Previous arc in the unsaturated path
    int length;       // Lenght of the unsaturated path until currente vertex
    bool labelled;
}label;

class FordFulkerson
{
public:
    std::vector<int> max_flow(int N_VERTICES, int N_ARCS, int source, int sink, std::vector<std::vector<int> > in_arcs, std::vector<std::vector<int> > out_arcs, std::vector<arc > arcs, std::vector<double> arcs_capacities, std::vector<double> flow_given);
};

#endif /* FordFulkerson_h */









