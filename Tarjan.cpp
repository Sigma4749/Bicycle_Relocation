#include "Tarjan.h"
using namespace std;

int Tarjan::visita(int k, int* id, int V, int *val, int *inval, int *nodos_padre_en_arbol_de_busqueda ,nodo* adj[maxV], nodo *z)
{
    nodo *t;
    int m;
    int min;
    
    if (*id < 0)
    {
        *id *= -1;
        inval[*id-1] = (-1 * k) - 1;
    }
    else
    {
        inval[*id - 1] = k + 1;
    }
    val[k]= *id;
    min = (*id);
    (*id)++;
    
    Pila.push_back(k);
    
    for (t = adj[k]; t != z; t = t->siguiente)
    {
        if (val[t->v] == noVisto)
        {
            nodos_padre_en_arbol_de_busqueda[t->v] = k;
            m = visita(t->v, id, V, val, inval, nodos_padre_en_arbol_de_busqueda, adj, z);
            if (m < min)
            {
                min = m;
            }
        }
        else if ( val[t->v] < min)
        {
            min = val[t->v];
        }
    }
    
    if (min == val[k])
    {
        num_componentes_fuertemente_conexas++;
        do
        {
            m = Pila.back();
            Pila.pop_back();
            val[m] = V+1;
        }
        while (m != k);
    }
    return min;
}

void Tarjan::busca(int V, int *val, int *inval, int* nodos_padre_en_arbol_de_busqueda,nodo* adj[maxV], nodo *z)
{
    int k = 0;
    int id = 1;
    for (k = 0; k < V; k++)
    {
        val[k] = noVisto;
        inval[k] = noVisto;
        nodos_padre_en_arbol_de_busqueda[k] = noVisto;
    }
    
    for (k = 0; k < V; k++)
    {
        if (val[k] == noVisto)
        {
            nodos_padre_en_arbol_de_busqueda[k] = k;
            id = ((-1) * k) - 1;
            visita(k, &id, V, val, inval, nodos_padre_en_arbol_de_busqueda ,adj, z);
            
            /* Determines if the root is an articulation vertex */
            int contador_hijos_raiz = 0;
            for (int j = 0; j < V; j++)
            {
                if ( nodos_padre_en_arbol_de_busqueda[j] == k)
                    contador_hijos_raiz++;
                if (contador_hijos_raiz >= 3)
                {
                    break;
                }
            }
        }
    }
}

int Tarjan::strong_components()
{
    // Read file
    ifstream grafoV;
    grafoV.open("vertices_data.txt");
    
    int j, v1, v2; // Auxiliary variables
    int V, E; // Number of vertices and edges in the input graph
    
    /* VERTICES */
    grafoV >> V;//Discard the first data
    grafoV >> V;//Discard the second data
    grafoV >> V;//Third data is the number of vertices
    grafoV.close();
    nodo* adj[maxV]; // Vertices array
    nodo *z;  // Sentinel node
    z = new nodo;
    z->v = -1;
    z->siguiente = z;
    
    // Array initialization
    for (j = 0; j < V; j++)
    {
        adj[j] = z;
    }
    
    nodo *t; // Auxiliar pointer to create new nodes
    int x, y; // Auxiliary variables for the indices of the ends of edges
    
    /* EDGES */
    ifstream grafoE;
    grafoE.open("arcs_data.txt");
    grafoE >> E; // Read number of edges
    
    for (j = 0; j< E; j++)
    {
        // Read an edge with ends v1 v2
        grafoE >> v1 >> v2;

        x = v1;
        y = v2;
        grafoE >> v1 >> v2; // Discard next two pairs of data
        
        // Creates a new node t
        t = new nodo;
        
        t->v = x;  // id of node t
        t->siguiente  = adj[y]; //t->siguiente points to the first node in the linked list adj[y]
        adj[y] = t; // Sets t as the first node of adj[y]
    }
    grafoE.close();
    
    /* Auxiliary structures for the search */
    int* val;
    val = (int*) malloc(V*sizeof(val));
    
    int* inval;
    inval = (int*) malloc(V*sizeof(val));
    
    int* nodos_padre_en_arbol_de_busqueda;
    nodos_padre_en_arbol_de_busqueda = (int*) malloc(V*sizeof(val));
    
    
    /* Depth First Search */
    busca(V, val, inval, nodos_padre_en_arbol_de_busqueda, adj, z);
    
    return num_componentes_fuertemente_conexas;
}
