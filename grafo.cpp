// ============================================================
//  grafo.cpp  –  Implementación del Grafo Dirigido con Pesos
//  Proyecto Final – Estructuras de Datos
//  Sin STL: todo con arreglos y punteros propios
// ============================================================
#include "grafo.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

// ============================================================
//  Función auxiliar privada: nuevaArista
//  Reserva memoria en el heap para una nueva arista y la
//  inicializa con el destino y costo indicados.
// ============================================================
Arista* Grafo::nuevaArista(int dest, int costo) {
    Arista* a = new Arista;  // reserva en memoria dinámica
    a->dest  = dest;
    a->costo = costo;
    a->sgte  = nullptr;      // por defecto, no hay siguiente
    return a;
}

// ============================================================
//  Constructor
//  Inicializa el grafo con 0 nodos.
//  Pone en nullptr las listas de adyacencia de todos los nodos
//  del arreglo estático para evitar basura en memoria.
// ============================================================
Grafo::Grafo() : totalNodos(0) {
    for (int i = 0; i < MAX_NODOS; i++) {
        nodos[i].ady      = nullptr;
        nodos[i].visitado = false;
        nodos[i].id       = -1;  // -1 indica posición vacía
    }
}

// ============================================================
//  Destructor
//  Recorre cada nodo y libera toda su lista enlazada de aristas
//  para evitar memory leaks.
// ============================================================
Grafo::~Grafo() {
    for (int i = 0; i < totalNodos; i++) {
        Arista* actual = nodos[i].ady;
        while (actual != nullptr) {
            Arista* siguiente = actual->sgte;  // guarda el puntero antes de borrar
            delete actual;
            actual = siguiente;
        }
        nodos[i].ady = nullptr;
    }
}

// ============================================================
//  addNodo
//  Agrega un nodo nuevo al grafo si:
//    1) No se superó el límite MAX_NODOS
//    2) No existe ya un nodo con el mismo id
//  El nodo se guarda en la posición [totalNodos] del arreglo.
// ============================================================
bool Grafo::addNodo(int id, const string& nombre, const string& pista) {
    // Verificar que hay espacio disponible
    if (totalNodos >= MAX_NODOS) {
        cout << "[ERROR] Capacidad maxima de nodos alcanzada (" << MAX_NODOS << ")\n";
        return false;
    }

    // Verificar que el id no esté duplicado
    if (buscarNodo(id) != nullptr) {
        cout << "[AVISO] El nodo con id=" << id << " ya existe.\n";
        return false;
    }

    // Insertar el nuevo nodo en la siguiente posición libre
    nodos[totalNodos].id       = id;
    nodos[totalNodos].nombre   = nombre;
    nodos[totalNodos].pista    = pista;
    nodos[totalNodos].visitado = false;
    nodos[totalNodos].ady      = nullptr;

    totalNodos++;  // actualizar contador
    return true;
}

// ============================================================
//  addArista
//  Agrega una arista dirigida: origen ──(costo)──► dest
//  La arista se inserta AL FRENTE de la lista de adyacencia
//  del nodo origen (inserción O(1)).
//  Retorna false si alguno de los dos nodos no existe.
// ============================================================
bool Grafo::addArista(int origen, int dest, int costo) {
    // Verificar que ambos nodos existen
    Nodo* nOrigen = buscarNodo(origen);
    Nodo* nDest   = buscarNodo(dest);

    if (nOrigen == nullptr) {
        cout << "[ERROR] Nodo origen " << origen << " no encontrado.\n";
        return false;
    }
    if (nDest == nullptr) {
        cout << "[ERROR] Nodo destino " << dest << " no encontrado.\n";
        return false;
    }

    // Crear la nueva arista
    Arista* nueva = nuevaArista(dest, costo);

    // Insertar al frente de la lista de adyacencia de origen
    nueva->sgte   = nOrigen->ady;
    nOrigen->ady  = nueva;

    return true;
}

// ============================================================
//  buscarNodo
//  Búsqueda lineal O(n) sobre el arreglo de nodos.
//  Retorna puntero al nodo cuyo id coincide, o nullptr.
// ============================================================
Nodo* Grafo::buscarNodo(int id) {
    for (int i = 0; i < totalNodos; i++) {
        if (nodos[i].id == id) {
            return &nodos[i];  // referencia directa al arreglo
        }
    }
    return nullptr;  // no encontrado
}

// ============================================================
//  mostrarGrafo
//  Imprime en consola la lista de adyacencia completa:
//  Para cada nodo muestra sus aristas salientes con el costo.
// ============================================================
void Grafo::mostrarGrafo() const {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout <<   "║      MAPA DE LA ISLA – LISTA ADY.        ║\n";
    cout <<   "╚══════════════════════════════════════════╝\n";

    for (int i = 0; i < totalNodos; i++) {
        // Encabezado del nodo
        cout << "[" << nodos[i].id << "] "
             << nodos[i].nombre
             << "\n    Pista: " << nodos[i].pista << "\n";

        // Recorrer lista de adyacencia del nodo
        Arista* actual = nodos[i].ady;
        if (actual == nullptr) {
            cout << "    (sin conexiones salientes)\n";
        }
        while (actual != nullptr) {
            // Buscar el nombre del destino para mostrarlo
            string nombreDest = "?";
            for (int j = 0; j < totalNodos; j++) {
                if (nodos[j].id == actual->dest) {
                    nombreDest = nodos[j].nombre;
                    break;
                }
            }
            cout << "    ──(" << actual->costo << ")──► "
                 << "[" << actual->dest << "] " << nombreDest << "\n";
            actual = actual->sgte;
        }
        cout << "\n";
    }
}

// ============================================================
//  resetVisitados
//  Pone la bandera visitado=false en todos los nodos.
//  Debe llamarse antes de cada BFS o DFS para empezar limpio.
// ============================================================
void Grafo::resetVisitados() {
    for (int i = 0; i < totalNodos; i++) {
        nodos[i].visitado = false;
    }
}

// ============================================================
//  getTotalNodos  –  Getter del contador de nodos
// ============================================================
int Grafo::getTotalNodos() const {
    return totalNodos;
}

// ============================================================
//  getNodo  –  Acceso directo al nodo en la posición i
// ============================================================
Nodo* Grafo::getNodo(int i) {
    if (i < 0 || i >= totalNodos) return nullptr;
    return &nodos[i];
}

// ============================================================
//  cargarDesdeArchivo
//
//  Lee el grafo desde un archivo de texto con el formato:
//
//    NODOS
//    <id> <Nombre_sin_espacios> "<pista entre comillas dobles>"
//    ...
//    ARISTAS
//    <origen> <destino> <costo>
//    ...
//
//  Las líneas que comienzan con '#' se ignoran (comentarios).
//  Retorna true si la carga fue exitosa, false si hubo error.
// ============================================================
bool Grafo::cargarDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo.c_str());  // abrir archivo

    if (!archivo.is_open()) {
        cout << "[ERROR] No se pudo abrir el archivo: " << nombreArchivo << "\n";
        return false;
    }

    string linea;
    bool seccionNodos   = false;  // bandera: estamos leyendo nodos
    bool seccionAristas = false;  // bandera: estamos leyendo aristas

    while (std::getline(archivo, linea)) {

        // Ignorar líneas vacías y comentarios
        if (linea.empty() || linea[0] == '#') continue;

        // ── Detectar encabezados de sección ──────────────────
        if (linea == "NODOS") {
            seccionNodos   = true;
            seccionAristas = false;
            continue;
        }
        if (linea == "ARISTAS") {
            seccionNodos   = false;
            seccionAristas = true;
            continue;
        }

        // ── Parsear un NODO ───────────────────────────────────
        if (seccionNodos) {
            int    id = -1;
            string nombre;
            string pista;

            // Leer id
            int pos = 0;
            // Saltar espacios iniciales
            while (pos < (int)linea.size() && linea[pos] == ' ') pos++;

            // Leer número (id)
            string numStr;
            while (pos < (int)linea.size() && linea[pos] != ' ') {
                numStr += linea[pos++];
            }
            if (numStr.empty()) continue;  // línea mal formada

            // Convertir string a int (sin usar stoi para compatibilidad)
            id = 0;
            for (int k = 0; k < (int)numStr.size(); k++) {
                id = id * 10 + (numStr[k] - '0');
            }

            // Saltar espacio después del id
            while (pos < (int)linea.size() && linea[pos] == ' ') pos++;

            // Leer nombre (hasta el siguiente espacio)
            while (pos < (int)linea.size() && linea[pos] != ' ') {
                nombre += linea[pos++];
            }

            // Saltar espacio antes de la pista
            while (pos < (int)linea.size() && linea[pos] == ' ') pos++;

            // Leer pista (entre comillas dobles)
            if (pos < (int)linea.size() && linea[pos] == '"') {
                pos++;  // saltar la comilla de apertura
                while (pos < (int)linea.size() && linea[pos] != '"') {
                    pista += linea[pos++];
                }
                // pos apunta a la comilla de cierre, no es necesario avanzar
            } else {
                // Si no hay comillas, tomar el resto de la línea como pista
                while (pos < (int)linea.size()) {
                    pista += linea[pos++];
                }
            }

            // Agregar el nodo al grafo
            addNodo(id, nombre, pista);
        }

        // ── Parsear una ARISTA ────────────────────────────────
        else if (seccionAristas) {
            int origen = -1, dest = -1, costo = 0;
            int pos = 0;

            // ── Función lambda local: leer entero desde pos ──
            // (Como no podemos usar streams fácilmente aquí,
            //  lo hacemos manualmente)
            auto leerInt = [&]() -> int {
                while (pos < (int)linea.size() && linea[pos] == ' ') pos++;
                string num;
                while (pos < (int)linea.size() && linea[pos] != ' ') {
                    num += linea[pos++];
                }
                if (num.empty()) return -1;
                int val = 0;
                for (int k = 0; k < (int)num.size(); k++) {
                    val = val * 10 + (num[k] - '0');
                }
                return val;
            };

            origen = leerInt();
            dest   = leerInt();
            costo  = leerInt();

            if (origen >= 0 && dest >= 0 && costo >= 0) {
                addArista(origen, dest, costo);
            }
        }
    }  // fin while getline

    archivo.close();

    cout << "[OK] Grafo cargado desde '" << nombreArchivo << "': "
         << totalNodos << " nodos.\n";
    return true;
}
