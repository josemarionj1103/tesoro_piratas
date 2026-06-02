// ============================================================
//  explorador.h  –  Explorador del Grafo (BFS, DFS, Dijkstra)
//  Proyecto Final – Estructuras de Datos
// ============================================================
#ifndef EXPLORADOR_H
#define EXPLORADOR_H

#include "grafo.h"
#include <string>

struct NodoEstructura {
    int dato;               
    NodoEstructura* sgte;   
};

class Explorador {
private:
    NodoEstructura* frenteCola;
    NodoEstructura* finalCola;
    
    void enqueue(int id);
    int dequeue();
    bool isColaEmpty() const;
    void vaciarCola();

    NodoEstructura* topePila;

    void push(int id);
    int pop();
    bool isPilaEmpty() const;
    void vaciarPila();

public:
    Explorador();
    ~Explorador();

    void BFS(Nodo* nd[], int totalNodos, int inicio);
    void DFS(Nodo* nd[], int totalNodos, int inicio);
    int Dijkstra(Nodo* nd[], int totalNodos, int inicio, int destino, int padre[]);
    void imprimirCamino(Nodo* nd[], int padre[], int destino);
};

#endif // EXPLORADOR_H
