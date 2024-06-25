#include "Input.h"
#include <fstream>
#include <stdlib.h>
#include <limits>
#include <math.h>
#include <ctime>
#include <cmath>

using namespace std;

void Input::read_vertices_data(const string& file_name)
{
    int dummy_counter1;
    int cumulative_sum = 0;
    ifstream vertices_file(file_name);
    
    if (! vertices_file)
    {
        cerr << "couldn’t open: " + string(file_name) << endl;
        exit(EXIT_FAILURE);
        return;
    }
    
    vertices_file >> GRID_WIDTH;  // Reads maximum possible first coordinate of the vertices
    vertices_file >> GRID_HEIGHT; // Reads maximum possible second coordinate of the vertices
    vertices_file >> N_VERTICES;  // Reads the number of vertices
    vertices.resize(N_VERTICES); // Adjust the size of the vertices vector to the exact size
    vertices_status.resize(N_VERTICES); // Adjust the size of the vertices vector to the exact size
    
    for(dummy_counter1 = 0; dummy_counter1 < N_VERTICES; ++dummy_counter1)
    {
        vertices_file >> vertices[dummy_counter1].first;
        vertices_file >> vertices[dummy_counter1].second;
        vertices_file >> vertices_status[dummy_counter1];
        cumulative_sum += vertices_status[dummy_counter1];
    }
    vertices_file.close();
    
    // Checks if surplus equals deficit
    if (cumulative_sum != 0)
    {
        cout << "Total sum of deficits and excesses over all vertices must be equal to 0, this instance is not correct, sum is " << cumulative_sum << endl;
        return;
    }
}

void Input::show_vertices()
{
    int k = 0;
    int aux = 5; //Number of data points that will be displayed by line
    while (k < N_VERTICES)
    {
        // Prints first index of current row
        if (k < 10)
        {
            cout << "   " << k << ":  ";
        }
        else if (k < 100)
        {
            cout << "  " << k << ":  ";
        }
        else
        {
            cout << " " << k << ":  ";
        }
        
        if (N_VERTICES - k < aux)
        {
            aux = N_VERTICES - k;
        }
        
        // Prints first coordinate of each vertex in the current row
        for (int j = k; j < k + aux; j++)
        {
            if (vertices[j].first < 10)
            {
                cout << "   " << vertices[j].first << " ";
            }
            else if (vertices[j].first < 100)
            {
                cout << "  " << vertices[j].first << " ";
            }
            else
            {
                cout << " " << vertices[j].first << " ";
            }
        }
        cout << endl;
        
        // Prints first index of current row
        if (k < 10)
        {
            cout << "   " << " " << "   ";
        }
        else if (k < 100)
        {
            cout << "  " << "  " << "   ";
        }
        else
        {
            cout << " " << "   " << "   ";
        }
        
        // Prints second coordinate of each vertex in the current row
        for (int j = k; j < k + aux; j++)
        {
            if (vertices[j].second< 10)
            {
                cout << "   " << vertices[j].second << " ";
            }
            else if (vertices[j].second < 100)
            {
                cout << "  " << vertices[j].second << " ";
            }
            else
            {
                cout << " " << vertices[j].second << " ";
            }
        }
        cout << endl;
        
        // Prints first index of current row
        if (k < 10)
        {
            cout << "   " << " " << "   ";
        }
        else if (k < 100)
        {
            cout << "  " << "  " << "   ";
        }
        else
        {
            cout << " " << "   " << "   ";
        }
        
        // Prints status of each vertex in the current row
        for (int j = k; j < k + aux; j++)
        {
            if (vertices_status[j]< 10)
            {
                if(vertices_status[j] < 0)
                {
                    cout << "  " << vertices_status[j] << " ";
                }
                else
                {
                    cout << "   " << vertices_status[j] << " ";
                }
            }
            else if (vertices_status[j] < 100)
            {
                if(vertices_status[j] < 0)
                {
                    cout << " " << vertices_status[j] << " ";
                }
                else
                {
                    cout << "  " << vertices_status[j] << " ";
                }
            }
            else
            {
                if(vertices_status[j] < 0)
                {
                    cout  << vertices_status[j] << " ";
                }
                else
                {
                    cout << " " << vertices_status[j] << " ";
                }
            }
        }
        cout << endl << endl;
        
        if (k + aux < N_VERTICES) {
            k += aux;
        }
        else {
            cout << endl;
            break;
        }
    }
}


void Input::read_arcs_data(const string& file_name)
{
    int dummy_counter1;
    int temp_datau;
    int temp_datav;
    int temp_datax;
    int temp_datay;
    ifstream arcs_file(file_name);
    
    if (! arcs_file)
    {
        cerr << "couldn’t open: " + string(file_name) << endl;
        exit(EXIT_FAILURE);
        return;
    }

    arcs_file >> N_ARCS; // Reads the number of arcs
    
    predecessors.assign(N_VERTICES, vector<int >(0, 0)); //Adjust the size of the matrix cce and fill it with zeros
    successors.assign(N_VERTICES, vector<int >(0, 0)); //Adjust the size of the matrix cce and fill it with zeros
    incidence_matrix.assign(N_VERTICES, vector<int >(N_VERTICES, 0)); //Adjust the size of the incidence matrix fill it with zeros
    cce_matrix.assign(N_VERTICES, vector<int >(N_VERTICES, 0)); //Adjust the size of the matrix cce and fill it with zeros
    cve_matrix.assign(N_VERTICES, vector<int >(N_VERTICES, 0)); //Adjust the size of the matrix cve and fill it with zeros
    arcs_indices_matrix.assign(N_VERTICES, vector<int >(N_VERTICES, 0)); //Adjust the size of the matrix fill it with zeros
    out_arcs.assign(N_VERTICES, vector<int >(0, 0)); //Adjust the size of the matrix fill it with zeros
    in_arcs.assign(N_VERTICES, vector<int >(0, 0)); //Adjust the size of the matrix fill it with zeros
    
    for(dummy_counter1 = 0; dummy_counter1 < N_ARCS; ++dummy_counter1)
    {
        arcs_file >> temp_datax;
        arcs_file >> temp_datay;
        predecessors[temp_datay].push_back(temp_datax);
        successors[temp_datax].push_back(temp_datay);
        incidence_matrix[temp_datax][temp_datay] = 1;
        arcs_file >> temp_datau;
        cce_matrix[temp_datax][temp_datay] = temp_datau;
        arcs_file >> temp_datav;
        cve_matrix[temp_datax][temp_datay] = temp_datav;
        arcs_list.push_back(make_pair(temp_datax, temp_datay));
        arcs_indices_matrix[temp_datax][temp_datay] = dummy_counter1;
        
        arc a;
        a.tail = temp_datax;
        a.head = temp_datay;
        a.index = dummy_counter1;
        a.cce_cost = temp_datau;
        a.cve_cost = temp_datav;
        
        out_arcs[temp_datax].push_back(dummy_counter1); // Store the index of the current edge in the entry of its tail
        in_arcs[temp_datay].push_back(dummy_counter1);  // Store the index of the current edge in the entry of its head
        arcs.push_back(a); // Add the arc to the collection of arcs
        
    }
    arcs_file.close();
}





void Input::show_incidence_matrix()
{
    cout << endl << " ";
    for (int i = 0; i < N_VERTICES; ++i)
    {
        std::string s = std::to_string(i);
        int espacio = 4 - int(s.length());
        if (N_VERTICES > 40)
        {
            espacio--;
        }
        for (int j = 0; j < espacio; j++)
        {
            cout << " ";
        }
        cout << i;
    }
    cout << endl << endl;
    
    for (int index1 = 0; index1 < N_VERTICES; ++index1)
    {
        cout << " ";
        for (int index2 = 0; index2 < N_VERTICES; ++index2)
        {
            std::string s = std::to_string(incidence_matrix[index1][index2]);
            int espacio = 4 - int(s.length());
            if (N_VERTICES > 40)
            {
                espacio--;
            }
            for (int j = 0; j < espacio; j++)
            {
                cout << " ";
            }
            if ( incidence_matrix[index1][index2] == 0)
            {
                cout << "-";
            }
            else
            {
                cout << incidence_matrix[index1][index2];
            }
        }
        cout << "  " << index1 << endl;
    }
}


void Input::show_cve_matrix()
{
    cout << endl << " ";
    for (int i = 0; i < N_VERTICES; ++i)
    {
        std::string s = std::to_string(i);
        int espacio = 4 - int(s.length());
        if (N_VERTICES > 40)
        {
            espacio--;
        }
        for (int j = 0; j < espacio; j++)
        {
            cout << " ";
        }
        cout << i;
    }
    cout << endl << endl;
    
    for (int index1 = 0; index1 < N_VERTICES; ++index1)
    {
        cout << " ";
        for (int index2 = 0; index2 < N_VERTICES; ++index2)
        {
            std::string s = std::to_string(cve_matrix[index1][index2]);
            int espacio = 4 - int(s.length());
            if (N_VERTICES > 40)
            {
                espacio--;
            }
            for (int j = 0; j < espacio; j++)
            {
                cout << " ";
            }
            cout << cve_matrix[index1][index2];
        }
        cout << "  " << index1 << endl;
    }
}

void Input::show_cce_matrix()
{
    cout << endl << " ";
    for (int i = 0; i < N_VERTICES; ++i)
    {
        std::string s = std::to_string(i);
        int espacio = 4 - int(s.length());
        if (N_VERTICES > 40)
        {
            espacio--;
        }
        for (int j = 0; j < espacio; j++)
        {
            cout << " ";
        }
        cout << i;
    }
    cout << endl << endl;
    
    for (int index1 = 0; index1 < N_VERTICES; ++index1)
    {
        cout << " ";
        for (int index2 = 0; index2 < N_VERTICES; ++index2)
        {
            std::string s = std::to_string(cce_matrix[index1][index2]);
            int espacio = 4 - int(s.length());
            if (N_VERTICES > 40)
            {
                espacio--;
            }
            for (int j = 0; j < espacio; j++)
            {
                cout << " ";
            }
            cout << cce_matrix[index1][index2];
        }
        cout << "  " << index1 << endl;
    }
}

void Input::draw_instance()
{
    system("python Drawing.py");
}

double distance(pair<int, int> x, pair<int, int> y)
{
    return pow( (x.first-y.first)*(x.first-y.first) +  (x.second-y.second)*(x.second-y.second), 0.5);
}


void Input::create_data(int grid_width = 20, int grid_height = 25, int n_vertices = 50, double sparsity_coefficient= 0.5, int requests_bound = 10, int cce_parameter = 1, int cve_parameter = 1)
{
    
    srand((unsigned) time(0)); // Initialize random seed
    GRID_WIDTH = grid_width;
    GRID_HEIGHT = grid_height;
    N_VERTICES = n_vertices;
    
    
    /* Vertices */
    vertices.resize(N_VERTICES); // Adjust the size of the vertices vector to the exact size
    vertices_status.resize(N_VERTICES); // Adjust the size of the vertices vector to the exact size
    
    vertices_status.assign(N_VERTICES,  0); //Adjust the size of the vector vertices_status and fill it with zeros
    
    int dummy_counter = 0;
    int x;
    int y;
    int dummy_index1 = 0;
    int dummy_index2 = 0;
    
    while(dummy_counter < n_vertices)
    {
        x = rand() % (GRID_WIDTH + 1);
        y = rand() % (GRID_HEIGHT + 1);
        
        // Check if current random point is already in the vertices vector
        int dummy_flag = 0;
        int temp_value = 0;
        int temp_index = 0;
        for (int i = 0; i < dummy_counter; ++i)
        {
            if (vertices[i].first == x && vertices[i].second == y)
            {
                dummy_flag = 1;
                break;
            }
        }
        
        
        if (dummy_flag == 0)
        {
            vertices[dummy_counter] = make_pair(x, y);
            
            // Change the status of the current vertex
            while(true)
            {
                if (dummy_counter % 3 == 0) // Mantain the the current status one third of the time (=> to obtain more neutral vertices)
                {
                    temp_value = 0;
                }
                else
                {
                    temp_value = rand() % (requests_bound + 1) ;
                }
                
                if( vertices_status[dummy_counter] + temp_value <= requests_bound)
                {
                    vertices_status[dummy_counter] += temp_value;
                    break;
                }
            }
            // Change the status of another vertex to restore equilibrium
            while (true)
            {
                temp_index = rand() % N_VERTICES;
                if ( vertices_status[temp_index] - temp_value >= -requests_bound )
                {
                    vertices_status[temp_index] -= temp_value;
                    break;
                }
            }
            dummy_counter += 1;
        }
    }
    int cummulative_sum = 0;
    for (int i = 0; i < N_VERTICES; i++)
    {
        cummulative_sum += vertices_status[i];
    }
    cout << "Sum of the status is " << cummulative_sum << endl;
    
    /* Edges */
    predecessors.assign(N_VERTICES, vector<int >(0, 0)); //Adjust the size of the matrix cce and fill it with zeros
    successors.assign(N_VERTICES, vector<int >(0, 0)); //Adjust the size of the matrix cce and fill it with zeros
    incidence_matrix.assign(N_VERTICES, vector<int >(N_VERTICES, 0)); //Adjust the size of the incidence matrix fill it with zeros
    cce_matrix.assign(N_VERTICES, vector<int >(N_VERTICES, 0)); //Adjust the size of the matrix cce and fill it with zeros
    cve_matrix.assign(N_VERTICES, vector<int >(N_VERTICES, 0)); //Adjust the size of the matrix cve and fill it with zeros
    
    arcs_indices_matrix.assign(N_VERTICES, vector<int >(N_VERTICES, 0)); //Adjust the size of the matrix fill it with zeros
    
    out_arcs.assign(N_VERTICES, vector<int >(0, 0)); //Adjust the size of the matrix fill it with zeros
    in_arcs.assign(N_VERTICES, vector<int >(0, 0)); //Adjust the size of the matrix fill it with zeros
    
    
    /* Greedy heuristic to build an initial hamiltonian cycle and guarantee strong connectivity */
    
    vector <int> used_vertices; // Vector to check the vertices already used
    used_vertices.assign(N_VERTICES,  0); //Adjust the size of the vector used_vertices and fill it with zeros
    
    x = 0; // x will be the the current vertex in each iteration, first vertex is 0
    used_vertices[0] = 1; // Mark first vertex as used
    
    y = 0; // variable y will be used to store the nearest neighbor of x in each iteration
    int current_shortest_distance; // Stores the minimum distance from current vertex x to the nearest neighbor y
    
    for (dummy_counter = 0; dummy_counter < N_VERTICES-1; dummy_counter++)
    {
        y = 1; // Reset y value to 1 (0 is already taken)
        current_shortest_distance = std::numeric_limits<int>::max(); // initialize with the maximum possible int value
        
        // Search the nearest unused neighbor y of the current vertex x (we use the square of the Euclidian distance)
        for (dummy_index1 = 0; dummy_index1 < N_VERTICES; ++dummy_index1)
        {
            if ((vertices[x].first-vertices[dummy_index1].first)*(vertices[x].first-vertices[dummy_index1].first) +  (vertices[x].second-vertices[dummy_index1].second)*(vertices[x].second-vertices[dummy_index1].second) < current_shortest_distance  &&  used_vertices[dummy_index1] == 0)
            {
                current_shortest_distance = (vertices[x].first-vertices[y].first)*(vertices[x].first-vertices[dummy_index1].first) +
                (vertices[x].second-vertices[dummy_index1].second)*(vertices[x].second-vertices[dummy_index1].second);
                y = dummy_index1;
            }
        }
        incidence_matrix[x][y] = 1;
        predecessors[y].push_back(x);
        successors[x].push_back(y);
        used_vertices[y] = 1; // Mark vertex y as used
        x = y; // Set y as the new current vertex
        
    }
    incidence_matrix[y][0] = 1;
    predecessors[0].push_back(y);
    successors[y].push_back(0);
    
    /* Add more edges using the sparsity parameter */
    double f;
    for(dummy_index1 = 0; dummy_index1 < N_VERTICES; ++dummy_index1)
    {
        for(dummy_index2 = 0; dummy_index2 < N_VERTICES; ++dummy_index2)
        {
            if (incidence_matrix[dummy_index1][dummy_index2] == 0 && dummy_index1 != dummy_index2 && ceil(distance(vertices[dummy_index1],vertices[dummy_index2])) < (GRID_WIDTH + GRID_HEIGHT)/4 )
            {
                f = (double)rand() / RAND_MAX;
                
                if (f <= sparsity_coefficient)
                {
                    incidence_matrix[dummy_index1][dummy_index2] = 1;
                    predecessors[dummy_index2].push_back(dummy_index1);
                    successors[dummy_index1].push_back(dummy_index2);
                }
            }
        }
    }
    
    /* cce and cve matrices */
    int k = 0; // Temp index for the arcs

    for(dummy_index1 = 0; dummy_index1 < N_VERTICES; ++dummy_index1)
    {
        int temp_index = 0;
        for(dummy_index2 = 0; dummy_index2 < N_VERTICES; ++dummy_index2)
        {
            if (incidence_matrix[dummy_index1][dummy_index2] == 1)
            {
                cce_matrix[dummy_index1][dummy_index2] = ceil(distance(vertices[dummy_index1], vertices[dummy_index2])) * cce_parameter;
                cve_matrix[dummy_index1][dummy_index2] = ceil(distance(vertices[dummy_index1], vertices[dummy_index2])) * cve_parameter;
                arcs_list.push_back(make_pair(dummy_index1, dummy_index2));
                arcs_indices_matrix[dummy_index1][dummy_index2] = k;
                
                out_arcs[dummy_index1].push_back(k);
                in_arcs[dummy_index2].push_back(k);
                
                arc a;
                a.tail = dummy_index1;
                a.head = dummy_index2;
                a.cce_cost = ceil(distance(vertices[dummy_index1], vertices[dummy_index2])) * cce_parameter;
                a.cve_cost = ceil(distance(vertices[dummy_index1], vertices[dummy_index2])) * cve_parameter;
                a.index = k;
                arcs.push_back(a);
                ++k;
                ++temp_index;
            }
            
        }
    }
    N_ARCS = k;
}

void Input::write_vertices_data(const string& file_name)
{
    ofstream vertices_file;
    vertices_file.open(file_name);

    vertices_file << GRID_WIDTH << endl;
    vertices_file << GRID_HEIGHT << endl;
    vertices_file << N_VERTICES << endl;
    for (int i = 0; i < N_VERTICES; ++i)
    {
        vertices_file << vertices[i].first << " " << vertices[i].second << " "<< vertices_status[i] << endl;
    }
}


void Input::write_arcs_data(const string& file_name)
{
    ofstream arcs_file;
    arcs_file.open(file_name);
    int number_edges = 0;
    for(int i = 0; i < N_VERTICES; ++i)
    {
        for(int j = 0; j < N_VERTICES; ++j)
        {
            if (incidence_matrix[i][j] == 1)
            {
                number_edges++;
            }
        }
    }
    
    arcs_file << number_edges << endl;
    
    for (int i = 0; i < N_VERTICES; ++i)
    {
        for(int j = 0; j < N_VERTICES; ++j)
        {
            if(incidence_matrix[i][j] == 1)
            {
                arcs_file << i << " " << j << " " << cce_matrix[i][j] << " " << cve_matrix[i][j] << endl;
            }
        }
    }
}


void Input::show_arcs_indices_matrix()
{
    cout << endl << " ";
    for (int i = 0; i < N_VERTICES; ++i)
    {
        std::string s = std::to_string(i);
        int espacio = 4 - int(s.length());
        if (N_VERTICES > 40)
        {
            espacio--;
        }
        for (int j = 0; j < espacio; j++)
        {
            cout << " ";
        }
        cout << i;
    }
    cout << endl << endl;
    
    for (int index1 = 0; index1 < N_VERTICES; ++index1)
    {
        cout << " ";
        for (int index2 = 0; index2 < N_VERTICES; ++index2)
        {
            std::string s = std::to_string(arcs_indices_matrix[index1][index2]);
            int espacio = 4 - int(s.length());
            if (N_VERTICES > 40)
            {
                espacio--;
            }
            for (int j = 0; j < espacio; j++)
            {
                cout << " ";
            }
            if (incidence_matrix[index1][index2] == 1)
            {
                cout << arcs_indices_matrix[index1][index2];
            }
            else
            {
                for (int j = 0; j < s.size()-1; j++)
                {
                    cout << " ";
                }
                cout << "-";
            }
        }
        cout << "  " << index1 << endl;
    }
}


void Input::show_predecessors()
{
    for (int i = 0; i < N_VERTICES; i++)
    {
        cout << i << ": ";
        for(int j = 0; j < predecessors[i].size(); j++)
        {
            cout << predecessors[i][j] << " ";
        }
        cout << endl;
    }
}

void Input::show_successors()
{
    for (int i = 0; i < N_VERTICES; i++)
    {
        cout << i << ": ";
        for(int j = 0; j < successors[i].size(); j++)
        {
            cout << successors[i][j] << " ";
        }
        cout << endl;
    }
}


void Input::show_out_arcs()
{
    for (int i = 0; i < N_VERTICES; i++)
    {
        cout << i << ": ";
        for(int j = 0; j < out_arcs[i].size(); j++)
        {
            cout << out_arcs[i][j] << " ";
        }
        cout << endl;
    }
}


void Input::show_in_arcs()
{
    for (int i = 0; i < N_VERTICES; i++)
    {
        cout << i << ": ";
        for(int j = 0; j < in_arcs[i].size(); j++)
        {
            cout << in_arcs[i][j] << " ";
        }
        cout << endl;
    }
}

void Input::show_arcs()
{
    for (int i = 0; i < N_ARCS; i++)
    {
        cout << arcs[i].index << ": (" <<arcs[i].tail << "->" << arcs[i].head
        << "), cce: " << arcs[i].cce_cost << ", cve: " << arcs[i].cve_cost << endl;
    }
}
