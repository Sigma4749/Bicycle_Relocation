#ifndef VSR_FLOW_INPUT_H
#define VSR_FLOW_INPUT_H

#include <iostream>
#include <vector>
#include <string>

/* Structure for the edges in the graph */
typedef struct arc {
    int tail;
    int head;
    int index;
    int cce_cost;
    int cve_cost;
}arc;

class Input
{
public:
    int GRID_WIDTH;
    int GRID_HEIGHT;
    int N_VERTICES;// Number of vertices in current instance
    int N_ARCS;    // Number of arcs in current instance
    int TMAX;     // Time horizon
    int CAP;      // Vehicle capacity
    
    std::vector<std::pair<int,int> > vertices;   // Vector, vertices in the current instance, vertex 0 is the depot vertex [0..N_VERTICES-1]
    std::vector<int> vertices_status; // Vector, vertices_status[v] > 0 indicates an excess, < 0 indicates a deficit, and = 0 indicates a neutral vertex
    std::vector<std::vector<int> > predecessors; // Vector, predecessors[v] contains the tails of the arcs with head v [0..N_VERTICES-1] x [0..in_degree_v]
    std::vector<std::vector<int> > successors;   // Vector, successors[v] contains the heads of the arcs with tail v   [0..N_VERTICES-1] x [0..out_degree_v]
    std::vector<std::vector<int> > incidence_matrix; // Matrix, incidence_matrix[i][j] == 1 indicates an edge from i to j [0..N_VERTICES-1] x [0..N_VERTICES-1]
    std::vector<std::vector<int> > cce_matrix;  // Matrix, carrier costs of the arcs [0..N_VERTICES-1] x [0..N_VERTICES-1]
    std::vector<std::vector<int> > cve_matrix;  // Matrix, vehicle costs of the arcs [0..N_VERTICES-1] x [0..N_VERTICES-1]
    std::vector<std::pair<int,int> > arcs_list;
    std::vector<std::vector<int> > arcs_indices_matrix;
    std::vector<std::vector<int> > in_arcs;  // in_arcs[i] is the vector with the indices of the arcs with head i
    std::vector<std::vector<int> > out_arcs; // out_arcs[i] is the vector with the indices of the arcs with tail i
    std::vector<arc> arcs;     // Vector with the arcs of the digraph
    
    // Prototypes of the methods
    void read_vertices_data(const std::string &file_name);
    void show_vertices();
    void read_arcs_data(const std::string &file_name);
    void show_incidence_matrix();
    void show_cce_matrix();
    void show_cve_matrix();
    void draw_instance();
    void show_arcs_indices_matrix();
    void show_predecessors();
    void show_successors();
    void create_data(int grid_width, int grid_height, int n_vertices, double sparsity_coefficient, int requests_bound, int cce_parameter, int cve_parameter);
    void write_vertices_data(const std::string &file_name);
    void write_arcs_data(const std::string &file_name);
    void show_out_arcs();
    void show_in_arcs();
    void show_arcs();
};

double distance(std::pair<int, int> x, std::pair<int, int> y);

#endif
