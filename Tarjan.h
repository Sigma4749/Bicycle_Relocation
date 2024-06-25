#ifndef Tarjan_h
#define Tarjan_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define maxV 500
#define noVisto 501

typedef struct nodo{
    int v;
    struct nodo *siguiente;
} nodo;

class Tarjan
{
public:
    std::vector <int> Pila;
    int num_componentes_fuertemente_conexas = 0;
    
    int visita(int k, int* id, int V, int *val, int *inval, int *nodos_padre_en_arbol_de_busqueda ,nodo* adj[maxV], nodo *z);
    void busca(int V, int *val, int *inval, int* nodos_padre_en_arbol_de_busqueda,nodo* adj[maxV], nodo *z);
    int strong_components();
};

#endif /* Tarjan_h */
