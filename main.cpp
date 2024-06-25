#include "Input.h"
#include "Model.h"
#include "Tarjan.h"
#include "FordFulkerson.h"
#include "calllback.h"
#include <iomanip>

using namespace std;
vector <string> lazy_cuts;

int main(int argc, char** argv)
{
    Input my_input;
    int option = 0;
    
    do
    {
        cout << "Enter : \n   0 to read the last instance created \n   1 to create a new instance" << endl;
        cout << "\n\n   Selection: ";
        cin >> option;
        cout << endl;
    } while (option != 0 && option != 1);
    
    if ( option == 0)
    {
        // Code to read instances
        my_input.read_vertices_data("vertices_data.txt");
        my_input.read_arcs_data("arcs_data.txt");
    }
    
    if (option == 1)
    {
        // Code to create new instances
        my_input.create_data(100, 100, 20, 0.25, 10, 1, 1);
        my_input.write_vertices_data("vertices_data.txt");
        my_input.write_arcs_data("arcs_data.txt");
    }
    
    // Check strong connectivity (Tarjan's algorithm)
    Tarjan verificador;
    if(verificador.strong_components() == 1)
    {
        cout << "\nThe input graph is strongly connected\n" << endl;
    }
    else
    {
        cout << "Current graph is not strongly connected, please verify input data" << endl;
        return 1;
    }
    
    // Define parameters CAP and TMAX with default values
    my_input.CAP = 10;
    my_input.TMAX = 250;
    
    if(argc > 3) // If we give at least three arguments
    {
        my_input.CAP = atoi(argv[1]); // first argument is the capacity
        my_input.TMAX = atoi(argv[2]); // second argument is the time available for eache vehicle
    }
    
    my_input.draw_instance();
    
    Model my_model(&my_input) ;
    
    my_model.createSolve(my_input.CAP, my_input.TMAX);
    
    my_model.cplex_.setParam(IloCplex::Param::Preprocessing::Presolve, IloFalse);
    
    my_model.cplex_.setParam(IloCplex::Param::Threads, 1);
    
    my_model.cplex_.setParam(IloCplex::Param::MIP::Strategy::Search, IloCplex::Traditional);
    
    my_model.cplex_.use(newFlowCutsCallback(my_model.env_, my_model.H_, my_model.h_, my_model.cplex_, my_input));
    
    ofstream lazy_cuts_file;
    lazy_cuts_file.open("lazy_cuts.txt");
    
    
    // MODEL SOLUTION
    IloBool ok = my_model.cplex_.solve();
    
    if (ok == IloTrue)
    {
        cout << "Program solved " << endl;
        my_model.cplex_.exportModel("model_with_cuts.lp");
        
        // Display solution
        int dummy_counter = 0;
        cout << "\nOptimal cost= " << my_model.cplex_.getObjValue() << endl;
        cout << "\nVariable values: \n" << endl;
        cout << setw(8) << "  A R C" << setw(6) << "H" << setw(5) << "h" << endl;
        for (int i = 0; i < my_input.N_ARCS; ++i)
        {
            if (my_model.cplex_.getValue(my_model.H_[i]) > 0)
            {
                cout << setw(3) << my_input.arcs[i].tail <<  " ->"  << setw(3) << my_input.arcs[i].head << setw(5) << my_model.cplex_.getValue(my_model.H_[i]) << setw(5) << my_model.cplex_.getValue(my_model.h_[i]) << endl;
                dummy_counter++;
            }
        }
        cout << endl;
        cout << "All other variables are 0.\n" << endl;
        
        // Export the solution to a text file
        ofstream solution_edges_file;
        solution_edges_file.open("solution_edges_data.txt");
        
        solution_edges_file << dummy_counter << endl;
        for (int i = 0; i < my_input.N_ARCS ; ++i)
        {
            if (my_model.cplex_.getValue(my_model.H_[i]) > 0)
            {
                solution_edges_file << my_input.arcs[i].tail << " " << my_input.arcs[i].head << " " << my_model.cplex_.getValue(my_model.H_[i]) << " " << my_model.cplex_.getValue(my_model.h_[i]) << endl;
            }
        }
        
        std::ofstream outfile;
        outfile.open("lazy_cuts.txt", std::ios_base::app); // append instead of overwrite
        
        cout << "Lazy Constraints";
        outfile << "Lazy Constraints";
        for (int i = 0; i < lazy_cuts.size(); ++i)
        {
            cout << "\nlazy_cut_" << to_string(i) << ":  ";
            outfile << "\nlazy_cut_" << to_string(i) << ":  ";

            for (int j = 0; j < lazy_cuts[i].size(); ++j)
            {
                if (lazy_cuts[i].at(j) == char('\n') )
                {
                    cout << lazy_cuts[i][j];
                    outfile << lazy_cuts[i][j];
                    if (j < lazy_cuts.size())
                    {
                        for (int k = 0; k < to_string(i).size(); ++k)
                        {
                            cout << " ";
                            outfile << " ";
                        }
                    }
                }
                else
                {
                    cout << lazy_cuts[i][j];
                    outfile << lazy_cuts[i][j];
                }
                
            }
            cout << " >=  0";
            outfile << " >=  0";
        }
        outfile.close();
        cout << endl;
        
        // Draw solution
        system("python Drawing_solution.py");
    }
    else
    {
        cout << "Program not solved " << endl;
        my_model.cplex_.exportModel("model_with_cuts.lp");
    }
    
    return 0;
}// END main
