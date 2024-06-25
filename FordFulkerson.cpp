#include <stdio.h>
#include "FordFulkerson.h"
#include <stdio.h>
#include <string>
#include <limits>
using namespace std;

vector<int > FordFulkerson::max_flow(int N_VERTICES, int N_ARCS, int source, int sink,std::vector<std::vector<int> > in_arcs, std::vector<std::vector<int> > out_arcs, std::vector<arc> arcs, std::vector<double> arcs_capacities, std::vector<double >flow_given )
{
    int dummy_counter1 = 0;
    vector<double> flow = flow_given;
    vector<int> labelled_nodes;
    bool better = true;
    
    while(better)
    {
        vector<int> vertices_to_scan;
        vector<label> augmenting_paths;
        vector<label> labels;
        labels.resize(N_VERTICES);
        for(dummy_counter1 = 0; dummy_counter1 < N_VERTICES; ++dummy_counter1)
        {
            label dummy_label;
            dummy_label.length = 0;
            dummy_label.previous_node = 0;
            dummy_label.previous_arc = -1;
            dummy_label.labelled = false;
            labels[dummy_counter1]= dummy_label;
        }
        
        int current_vertex;
        vertices_to_scan.push_back(source);
        labels[source].labelled = true;
        labels[source].length = 0;
        labels[source].previous_node = source;
        labels[source].previous_arc = -1;
        
        /* Main loop */
        while(vertices_to_scan.size() > 0)
        {
            // Pick an unscanned vertex
            current_vertex = vertices_to_scan.back();
            
            // Erase it from the vertices_to_scan
            vertices_to_scan.pop_back();
            
            // Scan out_edges
            for(dummy_counter1 = 0; dummy_counter1 < out_arcs[current_vertex].size(); ++dummy_counter1)
            {
                // If head's arc is not labelled and the arc is not saturated
                if (  flow[arcs[out_arcs[current_vertex][dummy_counter1]].index ] < arcs_capacities[arcs[out_arcs[current_vertex][dummy_counter1]].index]  &&
                    labels[ arcs[out_arcs[current_vertex][dummy_counter1]].head ].labelled == false)
                {
                    // If the head is not the sink vertex
                    if ( arcs[out_arcs[current_vertex][dummy_counter1]].head != sink )
                    {
                        // Label the vertex
                        labels[ arcs[out_arcs[current_vertex][dummy_counter1]].head ].labelled = true;
                        labels[ arcs[out_arcs[current_vertex][dummy_counter1]].head].previous_node = current_vertex;
                        labels[ arcs[out_arcs[current_vertex][dummy_counter1]].head].previous_arc = arcs[out_arcs[current_vertex][dummy_counter1]].index;
                        labels[arcs[out_arcs[current_vertex][dummy_counter1]].head ].length = labels[ current_vertex ].length + 1;
                        
                        // Add the head to the vertices to scan
                        vertices_to_scan.push_back( arcs[out_arcs[current_vertex][dummy_counter1]].head );
                    }
                    else // If the head is the sink vertex
                    {
                        label dummy_label;
                        dummy_label.previous_node = current_vertex;
                        dummy_label.previous_arc = arcs[out_arcs[current_vertex][dummy_counter1]].index;
                        dummy_label.length = labels[ current_vertex ].length + 1;
                        
                        // Store the final label in the vector of augmenting paths
                        augmenting_paths.push_back(dummy_label);
                    }
                }
            }
            
            // Scan in_edges
            for(dummy_counter1 = 0; dummy_counter1 < in_arcs[current_vertex].size(); ++dummy_counter1)
            {
                
                // If tail's arc is not labelled and the arc is a non saturated backward arc
                if (  flow[arcs[in_arcs[current_vertex][dummy_counter1]].index ] > 0  &&
                    labels[ arcs[in_arcs[current_vertex][dummy_counter1]].tail ].labelled == false)
                {
                    // If the head is not the sink vertex
                    if ( arcs[in_arcs[current_vertex][dummy_counter1]].tail != sink )
                    {
                        labels[ arcs[in_arcs[current_vertex][dummy_counter1]].tail ].labelled = true;
                        labels[ arcs[in_arcs[current_vertex][dummy_counter1]].tail ].previous_node = current_vertex;
                        labels[ arcs[in_arcs[current_vertex][dummy_counter1]].tail].previous_arc = arcs[in_arcs[current_vertex][dummy_counter1]].index;
                        labels[ arcs[in_arcs[current_vertex][dummy_counter1]].tail ].length = labels[ current_vertex ].length + 1;
                        
                        vertices_to_scan.push_back( arcs[in_arcs[current_vertex][dummy_counter1]].tail  );
                    }
                    else // If the tail is the sink vertex
                    {
                        label dummy_label;
                        dummy_label.previous_node = current_vertex;
                        dummy_label.previous_arc = arcs[in_arcs[current_vertex][dummy_counter1]].index;
                        dummy_label.length = labels[ current_vertex ].length + 1;
                        
                        // Store the final label in the vector of augmenting paths
                        augmenting_paths.push_back(dummy_label);
                    }
                }
            }
        } // End of main loop
        
        // Select an augmenting path with minimum number of arcs
        if(augmenting_paths.size() > 0)
        {
            label shortest_augmenting_path = augmenting_paths[0];
            for(dummy_counter1 = 0; dummy_counter1 < augmenting_paths.size(); ++dummy_counter1)
            {
                if(augmenting_paths[dummy_counter1].length < shortest_augmenting_path.length  )
                {
                    shortest_augmenting_path = augmenting_paths[dummy_counter1];
                }
            }
            
            labels[sink].previous_node = shortest_augmenting_path.previous_node;
            current_vertex = sink;
            int current_arc = shortest_augmenting_path.previous_arc;
            double flow_augmentation = std::numeric_limits<double>::max();

            vector<pair<int, int> > arcs_to_push_flow;  // First number is the index of the arc, the second is 1 for forward arcs and -1 for backards arcs
            while(current_vertex != source)
            {
                // If current arc it's a forward arc
                if ( arcs[current_arc].head == current_vertex )
                {
                    arcs_to_push_flow.push_back(make_pair(current_arc, 1) );
                    if (flow_augmentation > arcs_capacities[current_arc] - flow[current_arc] )
                    {
                        flow_augmentation = arcs_capacities[current_arc] - flow[current_arc] ;
                    }
                }
                
                // If current arc it's a backward arc
                if ( arcs[current_arc].tail == current_vertex )
                {
                    arcs_to_push_flow.push_back(make_pair(current_arc, -1) );
                    if (flow_augmentation > flow[current_arc] )
                    {
                        flow_augmentation = flow[current_arc];
                    }
                }
                current_vertex = labels[current_vertex].previous_node;
                current_arc = labels[current_vertex].previous_arc;
            }

            if (flow_augmentation == 0)
            {
                better = false;
                for (dummy_counter1 = 0; dummy_counter1 < labels.size(); ++dummy_counter1)
                {
                    if (labels[dummy_counter1].labelled == true)
                    {
                        labelled_nodes.push_back(dummy_counter1);
                    }
                }
                break;
            }
            for(pair<int,int> &e : arcs_to_push_flow )
            {
                flow[e.first] += e.second*flow_augmentation;
            }
        }
        else
        {
            better = false;
            for (dummy_counter1 = 0; dummy_counter1 < labels.size(); ++dummy_counter1)
            {
                if (labels[dummy_counter1].labelled == true)
                {
                    labelled_nodes.push_back(dummy_counter1);
                }
            }
            break;
        }
    }
    
    return labelled_nodes;
}
