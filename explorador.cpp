// ============================================================
//  explorador.cpp  –  Implementación del Explorador
//  Proyecto Final – Estructuras de Datos
// ============================================================
#include "explorador.h"
#include <iostream>

using std::cout;
using std::endl;

const int INFINITO = 9999999;

Explorador::Explorador() {
    frenteCola = nullptr;
    finalCola  = nullptr;
    topePila   = nullptr;
}

Explorador::~Explorador() {
    vaciarCola();
    vaciarPila();
}

void Explorador::enqueue(int id) {
    NodoEstructura* nuevo = new NodoEstructura;
    nuevo->dato = id;
    nuevo->sgte = nullptr;
    if (isColaEmpty()) { frenteCola = finalCola = nuevo; }
    else { finalCola->sgte = nuevo; finalCola = nuevo; }
}

int Explorador::dequeue() {
    if (isColaEmpty()) return -1;
    NodoEstructura* temp = frenteCola;
    int id = temp->dato;
    frenteCola = frenteCola->sgte;
    if (!frenteCola) finalCola = nullptr;
    delete temp;
    return id;
}

bool Explorador::isColaEmpty() const { return frenteCola == nullptr; }
void Explorador::vaciarCola() { while (!isColaEmpty()) dequeue(); }

void Explorador::push(int id) {
    NodoEstructura* nuevo = new NodoEstructura;
    nuevo->dato = id;
    nuevo->sgte = topePila;
    topePila = nuevo;
}

int Explorador::pop() {
    if (isPilaEmpty()) return -1;
    NodoEstructura* temp = topePila;
    int id = temp->dato;
    topePila = topePila->sgte;
    delete temp;
    return id;
}

bool Explorador::isPilaEmpty() const { return topePila == nullptr; }
void Explorador::vaciarPila() { while (!isPilaEmpty()) pop(); }

void Explorador::BFS(Nodo* nd[], int totalNodos, int inicio) {
    cout << "\n=== EJECUTANDO BFS DESDE EL NODO " << inicio << " ===\n";
    for (int i=0; i<50; i++) if(nd[i]) nd[i]->visitado = false;
    vaciarCola();

    if (!nd[inicio]) { cout << "[ERROR] Nodo origen no encontrado.\n"; return; }

    nd[inicio]->visitado = true;
    enqueue(inicio);
    int orden = 1;
    
    while (!isColaEmpty()) {
        int idActual = dequeue();
        Nodo* actual = nd[idActual];
        cout << orden++ << ". Visitando [" << idActual << "] " << actual->nombre << " -> Pista: " << actual->pista << "\n";
        
        Arista* ady = actual->ady;
        while (ady != nullptr) {
            Nodo* vecino = nd[ady->dest];
            if (vecino && !vecino->visitado) {
                vecino->visitado = true;
                enqueue(ady->dest);
            }
            ady = ady->sgte;
        }
    }
}

void Explorador::DFS(Nodo* nd[], int totalNodos, int inicio) {
    cout << "\n=== EJECUTANDO DFS DESDE EL NODO " << inicio << " ===\n";
    for (int i=0; i<50; i++) if(nd[i]) nd[i]->visitado = false;
    vaciarPila();

    if (!nd[inicio]) { cout << "[ERROR] Nodo origen no encontrado.\n"; return; }

    push(inicio);
    int orden = 1;

    while (!isPilaEmpty()) {
        int idActual = pop();
        Nodo* actual = nd[idActual];

        if (!actual->visitado) {
            actual->visitado = true;
            cout << orden++ << ". Visitando [" << idActual << "] " << actual->nombre << " -> Pista: " << actual->pista << "\n";
            Arista* ady = actual->ady;
            while (ady != nullptr) {
                if (nd[ady->dest] && !nd[ady->dest]->visitado) push(ady->dest);
                ady = ady->sgte;
            }
        }
    }
}

int Explorador::Dijkstra(Nodo* nd[], int totalNodos, int inicio, int destino, int padre[]) {
    int dist[50];
    bool procesado[50];

    for (int i = 0; i < 50; i++) {
        dist[i] = INFINITO;
        padre[i] = -1;
        procesado[i] = false;
    }
    if (!nd[inicio] || !nd[destino]) return -1;

    dist[inicio] = 0;

    for (int i = 0; i < totalNodos; i++) {
        int u = -1;
        int min_dist = INFINITO;
        for (int v = 0; v < 50; v++) {
            if (nd[v] != nullptr && !procesado[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }
        if (u == -1) break;
        procesado[u] = true;
        if (u == destino) break;

        Arista* ady = nd[u]->ady;
        while (ady != nullptr) {
            int v = ady->dest;
            int peso = ady->costo;
            if (nd[v] && !procesado[v] && dist[u] != INFINITO && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                padre[v] = u;
            }
            ady = ady->sgte;
        }
    }
    return (dist[destino] == INFINITO) ? -1 : dist[destino];
}

void Explorador::imprimirCamino(Nodo* nd[], int padre[], int destino) {
    if (padre[destino] == -1 && destino != 0) {
        cout << "[!] No hay camino.\n";
        return;
    }
    vaciarPila();
    int actual = destino;
    while (actual != -1) {
        push(actual);
        actual = padre[actual];
    }
    cout << "\n=== RUTA AL TESORO (DIJKSTRA) ===\n";
    while (!isPilaEmpty()) {
        int idNodo = pop();
        cout << "[" << idNodo << "] " << nd[idNodo]->nombre;
        if (!isPilaEmpty()) cout << " --(viaja)--> ";
    }
    cout << "\n=================================\n";
}
