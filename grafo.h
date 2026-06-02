// ============================================================
//  grafo.h  –  Grafo Dirigido con Pesos: Mapa de la Isla (Neverland)
//  Proyecto Final – Estructuras de Datos
//  Sin STL: todo con arreglos y punteros propios
// ============================================================
#ifndef GRAFO_H
#define GRAFO_H

#include <string>
using std::string;

// ------------------------------------------------------------
//  CONSTANTE: límite máximo de nodos en el mapa
// ------------------------------------------------------------
const int MAX_NODOS = 50;

// ============================================================
//  ARISTA  –  Representa una conexión dirigida entre dos nodos
//  Cada arista guarda:
//    dest  : índice del nodo destino
//    costo : peso / distancia de la conexión
//    sgte  : puntero al siguiente nodo en la lista enlazada
// ============================================================
struct Arista {
    int    dest;   // índice del nodo al que apunta esta arista
    int    costo;  // costo (distancia, dificultad, etc.)
    Arista* sgte;  // siguiente arista en la lista de adyacencia
};

// ============================================================
//  NODO  –  Representa una ubicación en el mapa de la isla
//  Cada nodo guarda:
//    id       : identificador único (0 … MAX_NODOS-1)
//    nombre   : nombre del lugar (sin espacios en el archivo)
//    pista    : mensaje/pista que deja el lugar
//    visitado : bandera para algoritmos de recorrido
//    ady      : cabeza de la lista enlazada de aristas
// ============================================================
struct Nodo {
    int     id;
    string  nombre;
    string  pista;
    bool    visitado;
    Arista* ady;   // lista enlazada de adyacencia (aristas salientes)
};

// ============================================================
//  GRAFO  –  Clase que encapsula el grafo dirigido con pesos
// ============================================================
class Grafo {
private:
    Nodo   nodos[MAX_NODOS];  // arreglo estático de nodos
    int    totalNodos;        // cantidad actual de nodos cargados

    // Crea y devuelve una nueva arista en el heap
    Arista* nuevaArista(int dest, int costo);

public:
    // --------------------------------------------------------
    //  Constructor: inicializa el grafo vacío
    // --------------------------------------------------------
    Grafo();

    // --------------------------------------------------------
    //  Destructor: libera toda la memoria de aristas
    // --------------------------------------------------------
    ~Grafo();

    // --------------------------------------------------------
    //  addNodo: agrega un nodo con id, nombre y pista
    //  Retorna true si se pudo agregar, false si ya existe
    //  o se superó MAX_NODOS
    // --------------------------------------------------------
    bool addNodo(int id, const string& nombre, const string& pista);

    // --------------------------------------------------------
    //  addArista: agrega una arista dirigida de origen→dest
    //  con el costo indicado (no verifica duplicados)
    //  Retorna true si ambos nodos existen
    // --------------------------------------------------------
    bool addArista(int origen, int dest, int costo);

    // --------------------------------------------------------
    //  buscarNodo: retorna puntero al nodo con el id dado,
    //  o nullptr si no existe
    // --------------------------------------------------------
    Nodo* buscarNodo(int id);

    // --------------------------------------------------------
    //  mostrarGrafo: imprime en consola todos los nodos y
    //  su lista de adyacencia con costos
    // --------------------------------------------------------
    void mostrarGrafo() const;

    // --------------------------------------------------------
    //  resetVisitados: pone visitado=false en todos los nodos
    //  (útil antes de iniciar BFS / DFS)
    // --------------------------------------------------------
    void resetVisitados();

    // --------------------------------------------------------
    //  getTotalNodos: devuelve la cantidad de nodos cargados
    // --------------------------------------------------------
    int getTotalNodos() const;

    // --------------------------------------------------------
    //  getNodo: acceso directo al nodo en la posición i del
    //  arreglo interno (0 ≤ i < totalNodos)
    // --------------------------------------------------------
    Nodo* getNodo(int i);

    // --------------------------------------------------------
    //  cargarDesdeArchivo: lee el grafo desde un archivo .txt
    //  Formato esperado:
    //    NODOS
    //    <id> <Nombre> "<pista entre comillas>"
    //    ...
    //    ARISTAS
    //    <origen> <dest> <costo>
    //    ...
    //  Retorna true si la carga fue exitosa
    // --------------------------------------------------------
    bool cargarDesdeArchivo(const string& nombreArchivo);
};

#endif // GRAFO_H
