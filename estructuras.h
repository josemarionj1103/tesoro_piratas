#include <fstream>
#include <iostream>
#include <string>

// --- BEGIN grafo.h ---
// ============================================================
//  grafo.h  –  Grafo Dirigido con Pesos: Mapa de la Isla (Neverland)
//  Proyecto Final – Estructuras de Datos
//  Sin STL: todo con arreglos y punteros propios
// ============================================================

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


// --- END grafo.h ---

// --- BEGIN hash.h ---
// ============================================================
//  hash.h  –  Diccionario del Habla Popular de Santa Cruz
//  Proyecto Final – Estructuras de Datos
//  Requisito: Tabla Hash con 28 posiciones y colisiones por BST
// ============================================================

using std::string;

// ============================================================
//  ESTRUCTURAS DEFINIDAS SEGÚN REQUERIMIENTO
// ============================================================

// Para lista enlazada (versión básica - dejada por completitud estructural)
struct EntradaHash {
    string palabra;
    string significado;
    EntradaHash* sgte;
};

// Para BST (versión mejorada)  
struct NodoBST {
    string palabra;
    string significado;
    NodoBST* izq;
    NodoBST* der;
};

// Tabla Hash (arreglo de 28 posiciones)
struct TablaHashEstructura {
    NodoBST* tabla[28];   // cada posición es raíz de un BST
};

// ============================================================
//  CLASE DICCIONARIO HASH (Maneja la Tabla Hash)
// ============================================================
class DiccionarioHash {
private:
    TablaHashEstructura ht;

    // Métodos auxiliares recursivos para el BST
    NodoBST* insertarBST(NodoBST* nodo, const string& palabra, const string& significado);
    NodoBST* buscarBST(NodoBST* nodo, const string& palabra) const;
    void mostrarBST(NodoBST* nodo) const;
    void liberarBST(NodoBST* nodo);
    int  contarNodosBST(NodoBST* nodo) const;
    
    // Auxiliares para eliminación en BST
    NodoBST* eliminarBST(NodoBST* nodo, const string& palabra, bool& eliminado);
    NodoBST* encontrarMinimo(NodoBST* nodo) const;

public:
    DiccionarioHash();
    ~DiccionarioHash();

    // 1. Calcula la posición en base al valor ASCII % 28
    int funcionHash(string palabra) const;

    // 2. Lee "PALABRA: significado" desde un archivo
    void cargarDesdeArchivo(string archivo);

    // 3. Inserta una palabra y su significado
    void insertar(string palabra, string significado);

    // 4. Busca el significado de una palabra
    string buscar(string palabra) const;

    // 5. Elimina una palabra (maneja las rotaciones/borrados del BST)
    bool eliminar(string palabra);

    // 6. Muestra todas las posiciones con sus palabras
    void mostrarTabla() const;

    // 7. Muestra cuántas palabras hay por cada posición
    void mostrarEstadisticas() const;
};


// --- END hash.h ---

// --- BEGIN arbol.h ---
// ============================================================
//  arbol.h  –  Árbol Binario de Decisiones para Pistas
//  Proyecto Final – Estructuras de Datos
// ============================================================

using std::string;

// ============================================================
//  NODO DEL ÁRBOL DE DECISIONES
// ============================================================
struct NodoArbol {
    string palabraClave;   // Palabra a buscar si es un nodo interno
    string ubicacion;      // Resultado del lugar si es nodo hoja
    bool   esHoja;         // Bandera para diferenciar tipo de nodo
    NodoArbol* izq;        // Rama SI (pista contiene la palabra)
    NodoArbol* der;        // Rama NO (pista no contiene la palabra)
};

// ============================================================
//  CLASE ÁRBOL DE DECISIONES
// ============================================================
class ArbolDecisiones {
private:
    NodoArbol* raiz;

    // Métodos privados para manejo de nodos y memoria
    NodoArbol* crearNodoInterno(const string& palabra);
    NodoArbol* crearHoja(const string& ubicacion);
    void destruirArbol(NodoArbol* nodo);
    
    // Función auxiliar para buscar subcadenas sin requerir <algorithm>
    bool contienePalabra(const string& texto, const string& subcadena) const;

public:
    ArbolDecisiones();
    ~ArbolDecisiones();

    // --------------------------------------------------------
    // 1. Construcción manual del árbol según reglas cambas
    // --------------------------------------------------------
    void construirArbol();

    // --------------------------------------------------------
    // 2. interpretarPista: Recorre el árbol y devuelve ubicacion
    // --------------------------------------------------------
    string interpretarPista(NodoArbol* nodo, const string& pista) const;

    // Wrapper para no exponer la raíz desde afuera
    string interpretarPista(const string& pista) const;

    // --------------------------------------------------------
    // 3. mostrarArbol: Imprime visualmente el árbol
    // --------------------------------------------------------
    void mostrarArbol(NodoArbol* nodo, int nivel) const;
    
    // Wrapper para imprimir desde la raíz
    void mostrarArbol() const;
};


// --- END arbol.h ---

// --- BEGIN explorador.h ---
// ============================================================
//  explorador.h  –  Explorador del Grafo (BFS, DFS, Dijkstra)
//  Proyecto Final – Estructuras de Datos
// ============================================================


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


// --- END explorador.h ---

// --- BEGIN archivos.h ---
// ============================================================
//  archivos.h  –  Módulo de Manejo de Archivos
//  Proyecto Final – Estructuras de Datos
//  Requisito: Uso estricto de ifstream/ofstream sin contenedores STL
// ============================================================


using std::string;

// ------------------------------------------------------------
//  1. Cargar el grafo desde un archivo de texto
//  Lee "mapa_isla.txt". El arreglo de punteros `nd` se llenará
//  con los nodos creados en memoria dinámica.
// ------------------------------------------------------------
bool cargarGrafo(string archivo, Nodo* nd[], int &total);

// ------------------------------------------------------------
//  2. Cargar pistas desde un archivo
//  Lee "pistas.txt" (formato: NombreNodo|pista completa)
// ------------------------------------------------------------
bool cargarPistas(string archivo, Nodo* nd[], int total);

// ------------------------------------------------------------
//  3. Guardar ruta encontrada por Dijkstra
//  Guarda fecha, hora, camino paso a paso, pistas y costo.
// ------------------------------------------------------------
bool guardarRuta(string archivo, int camino[], int longitud, Nodo* nd[], int costoTotal);

// ------------------------------------------------------------
//  4. Exportar el diccionario hash a un archivo
// ------------------------------------------------------------
bool guardarDiccionario(string archivo, TablaHashEstructura &tabla);


// --- END archivos.h ---

// --- BEGIN grafo.cpp ---
// ============================================================
//  grafo.cpp  –  Implementación del Grafo Dirigido con Pesos
//  Proyecto Final – Estructuras de Datos
//  Sin STL: todo con arreglos y punteros propios
// ============================================================
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

// --- END grafo.cpp ---

// --- BEGIN hash.cpp ---
// ============================================================
//  hash.cpp  –  Implementación del Diccionario Camba
//  Proyecto Final – Estructuras de Datos
// ============================================================

using std::cout;
using std::endl;
using std::ifstream;

// ============================================================
//  Constructor y Destructor
// ============================================================
DiccionarioHash::DiccionarioHash() {
    for (int i = 0; i < 28; i++) {
        ht.tabla[i] = nullptr;
    }
}

DiccionarioHash::~DiccionarioHash() {
    for (int i = 0; i < 28; i++) {
        liberarBST(ht.tabla[i]);
        ht.tabla[i] = nullptr;
    }
}

void DiccionarioHash::liberarBST(NodoBST* nodo) {
    if (nodo != nullptr) {
        liberarBST(nodo->izq);
        liberarBST(nodo->der);
        delete nodo;
    }
}

// ============================================================
//  1. funcionHash: Suma los ASCII y devuelve módulo 28
// ============================================================
int DiccionarioHash::funcionHash(string palabra) const {
    int suma = 0;
    for (int i = 0; i < (int)palabra.length(); i++) {
        suma += (unsigned char)palabra[i];
    }
    return suma % 28;
}

// ============================================================
//  2. cargarDesdeArchivo: Lee "PALABRA: significado"
// ============================================================
void DiccionarioHash::cargarDesdeArchivo(string archivo) {
    ifstream file(archivo.c_str());
    if (!file.is_open()) {
        cout << "[ERROR] No se pudo abrir el archivo de diccionario: " << archivo << "\n";
        return;
    }

    string linea;
    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        // Buscamos el separador ':'
        int posDosPuntos = -1;
        for (int i = 0; i < (int)linea.length(); i++) {
            if (linea[i] == ':') {
                posDosPuntos = i;
                break;
            }
        }

        if (posDosPuntos != -1) {
            // Extraer palabra
            string palabra = "";
            for (int i = 0; i < posDosPuntos; i++) {
                palabra += linea[i];
            }

            // Extraer significado, saltando espacios iniciales si los hay
            int inicioSig = posDosPuntos + 1;
            while (inicioSig < (int)linea.length() && linea[inicioSig] == ' ') {
                inicioSig++;
            }

            string significado = "";
            for (int i = inicioSig; i < (int)linea.length(); i++) {
                if (linea[i] != '\r') {
                    significado += linea[i];
                }
            }

            // Insertar en la tabla hash
            insertar(palabra, significado);
        }
    }
    file.close();
    cout << "[OK] Diccionario cargado exitosamente desde " << archivo << "\n";
}

// ============================================================
//  3. insertar: Agrega una palabra y su significado
// ============================================================
void DiccionarioHash::insertar(string palabra, string significado) {
    int pos = funcionHash(palabra);
    ht.tabla[pos] = insertarBST(ht.tabla[pos], palabra, significado);
}

NodoBST* DiccionarioHash::insertarBST(NodoBST* nodo, const string& palabra, const string& significado) {
    if (nodo == nullptr) {
        NodoBST* nuevo = new NodoBST;
        nuevo->palabra = palabra;
        nuevo->significado = significado;
        nuevo->izq = nullptr;
        nuevo->der = nullptr;
        return nuevo;
    }

    // Ordenamiento alfabético en el BST
    if (palabra < nodo->palabra) {
        nodo->izq = insertarBST(nodo->izq, palabra, significado);
    } else if (palabra > nodo->palabra) {
        nodo->der = insertarBST(nodo->der, palabra, significado);
    } else {
        // Si la palabra ya existe, simplemente actualiza el significado
        nodo->significado = significado;
    }

    return nodo;
}

// ============================================================
//  4. buscar: Devuelve significado o "No encontrada"
// ============================================================
string DiccionarioHash::buscar(string palabra) const {
    int pos = funcionHash(palabra);
    NodoBST* res = buscarBST(ht.tabla[pos], palabra);
    if (res != nullptr) {
        return res->significado;
    }
    return "No encontrada";
}

NodoBST* DiccionarioHash::buscarBST(NodoBST* nodo, const string& palabra) const {
    if (nodo == nullptr) return nullptr;

    if (palabra == nodo->palabra) return nodo;
    else if (palabra < nodo->palabra) return buscarBST(nodo->izq, palabra);
    else return buscarBST(nodo->der, palabra);
}

// ============================================================
//  5. eliminar: Elimina una palabra y arregla el BST
// ============================================================
bool DiccionarioHash::eliminar(string palabra) {
    int pos = funcionHash(palabra);
    bool eliminado = false;
    ht.tabla[pos] = eliminarBST(ht.tabla[pos], palabra, eliminado);
    return eliminado;
}

// Función auxiliar para encontrar el nodo con el valor más pequeño
NodoBST* DiccionarioHash::encontrarMinimo(NodoBST* nodo) const {
    NodoBST* actual = nodo;
    while (actual && actual->izq != nullptr) {
        actual = actual->izq;
    }
    return actual;
}

NodoBST* DiccionarioHash::eliminarBST(NodoBST* nodo, const string& palabra, bool& eliminado) {
    if (nodo == nullptr) return nullptr; // No se encontró la palabra

    if (palabra < nodo->palabra) {
        nodo->izq = eliminarBST(nodo->izq, palabra, eliminado);
    } else if (palabra > nodo->palabra) {
        nodo->der = eliminarBST(nodo->der, palabra, eliminado);
    } else {
        // Encontramos el nodo a eliminar
        eliminado = true;

        // Caso 1: Sin hijos o un hijo
        if (nodo->izq == nullptr) {
            NodoBST* temp = nodo->der;
            delete nodo;
            return temp;
        } else if (nodo->der == nullptr) {
            NodoBST* temp = nodo->izq;
            delete nodo;
            return temp;
        }

        // Caso 2: Dos hijos
        // Encontrar el sucesor en inorden (el menor del subárbol derecho)
        NodoBST* temp = encontrarMinimo(nodo->der);
        
        // Copiar los datos del sucesor al nodo actual
        nodo->palabra = temp->palabra;
        nodo->significado = temp->significado;
        
        // Eliminar el sucesor
        bool dummy = false;
        nodo->der = eliminarBST(nodo->der, temp->palabra, dummy);
    }
    return nodo;
}

// ============================================================
//  6. mostrarTabla: Muestra posiciones con sus palabras
// ============================================================
void DiccionarioHash::mostrarTabla() const {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout <<   "║     DICCIONARIO CAMBA (TABLA HASH)       ║\n";
    cout <<   "╚══════════════════════════════════════════╝\n";
    for (int i = 0; i < 28; i++) {
        if (ht.tabla[i] != nullptr) {
            cout << "[Posicion " << i << "]:\n";
            mostrarBST(ht.tabla[i]);
            cout << "\n";
        }
    }
}

void DiccionarioHash::mostrarBST(NodoBST* nodo) const {
    if (nodo != nullptr) {
        mostrarBST(nodo->izq);
        cout << "   - " << nodo->palabra << ": " << nodo->significado << "\n";
        mostrarBST(nodo->der);
    }
}

// ============================================================
//  7. mostrarEstadisticas: Distribución de la tabla
// ============================================================
void DiccionarioHash::mostrarEstadisticas() const {
    cout << "\n=== ESTADISTICAS DE LA TABLA HASH ===\n";
    int totalPalabras = 0;
    
    for (int i = 0; i < 28; i++) {
        int cantidad = contarNodosBST(ht.tabla[i]);
        cout << "Posicion " << i << ": " << cantidad << " palabras.\n";
        totalPalabras += cantidad;
    }
    
    cout << "-------------------------------------\n";
    cout << "Total de palabras en el diccionario: " << totalPalabras << "\n\n";
}

int DiccionarioHash::contarNodosBST(NodoBST* nodo) const {
    if (nodo == nullptr) return 0;
    return 1 + contarNodosBST(nodo->izq) + contarNodosBST(nodo->der);
}

// --- END hash.cpp ---

// --- BEGIN arbol.cpp ---
// ============================================================
//  arbol.cpp  –  Implementación del Árbol de Decisiones
//  Proyecto Final – Estructuras de Datos
// ============================================================

using std::cout;
using std::endl;

// ============================================================
//  Constructor y Destructor
// ============================================================
ArbolDecisiones::ArbolDecisiones() : raiz(nullptr) {}

ArbolDecisiones::~ArbolDecisiones() {
    destruirArbol(raiz);
}

void ArbolDecisiones::destruirArbol(NodoArbol* nodo) {
    if (nodo != nullptr) {
        destruirArbol(nodo->izq);
        destruirArbol(nodo->der);
        delete nodo;
    }
}

// ============================================================
//  Funciones para crear nodos de forma dinámica
// ============================================================
NodoArbol* ArbolDecisiones::crearNodoInterno(const string& palabra) {
    NodoArbol* nuevo = new NodoArbol;
    nuevo->palabraClave = palabra;
    nuevo->ubicacion    = "";
    nuevo->esHoja       = false;
    nuevo->izq          = nullptr;
    nuevo->der          = nullptr;
    return nuevo;
}

NodoArbol* ArbolDecisiones::crearHoja(const string& ubicacion) {
    NodoArbol* nuevo = new NodoArbol;
    nuevo->palabraClave = "";
    nuevo->ubicacion    = ubicacion;
    nuevo->esHoja       = true;
    nuevo->izq          = nullptr;
    nuevo->der          = nullptr;
    return nuevo;
}

// ============================================================
//  1. Construcción manual del árbol (Reglas de decisión)
// ============================================================
void ArbolDecisiones::construirArbol() {
    if (raiz != nullptr) destruirArbol(raiz);

    // Creamos las hojas que serán los destinos finales
    NodoArbol* hSkull     = crearHoja("Skull_Rock");
    NodoArbol* hMermaid   = crearHoja("Mermaid_Lagoon");
    NodoArbol* hCrocodile = crearHoja("Crocodile_Creek");
    NodoArbol* hIndian    = crearHoja("Indian_Camp");
    NodoArbol* hCannibal  = crearHoja("Cannibal_Cove");
    NodoArbol* hTree      = crearHoja("Hanging_Tree");
    NodoArbol* hLostBoys  = crearHoja("Lost_Boys_Camp");
    NodoArbol* hPegleg    = crearHoja("Pegleg_Point");
    NodoArbol* hTesoro    = crearHoja("TESORO");
    NodoArbol* hDesc      = crearHoja("Ubicacion_Desconocida");

    // Construimos el árbol anidando las palabras clave.
    // Si la pista contiene la palabra, va a la IZQ. Si no, a la DER.
    
    // Grupo: Skull_Rock
    NodoArbol* nCalavera = crearNodoInterno("calavera");
    NodoArbol* nHueso    = crearNodoInterno("hueso");
    nCalavera->izq = hSkull;
    nCalavera->der = nHueso;
    nHueso->izq    = hSkull;
    
    // Grupo: Mermaid_Lagoon
    NodoArbol* nAgua   = crearNodoInterno("agua");
    NodoArbol* nCanta  = crearNodoInterno("canta");
    NodoArbol* nSirena = crearNodoInterno("sirena");
    nHueso->der   = nAgua;  // Conectamos el grupo anterior con este
    nAgua->izq    = hMermaid;
    nAgua->der    = nCanta;
    nCanta->izq   = hMermaid;
    nCanta->der   = nSirena;
    nSirena->izq  = hMermaid;

    // Grupo: Crocodile_Creek
    NodoArbol* nAcecha    = crearNodoInterno("acecha");
    NodoArbol* nCorre     = crearNodoInterno("corre");
    NodoArbol* nCocodrilo = crearNodoInterno("cocodrilo");
    nSirena->der    = nAcecha;
    nAcecha->izq    = hCrocodile;
    nAcecha->der    = nCorre;
    nCorre->izq     = hCrocodile;
    nCorre->der     = nCocodrilo;
    nCocodrilo->izq = hCrocodile;

    // Grupo: Indian_Camp
    NodoArbol* nHumo   = crearNodoInterno("humo");
    NodoArbol* nTambor = crearNodoInterno("tambor");
    NodoArbol* nColina = crearNodoInterno("colina");
    nCocodrilo->der = nHumo;
    nHumo->izq      = hIndian;
    nHumo->der      = nTambor;
    nTambor->izq    = hIndian;
    nTambor->der    = nColina;
    nColina->izq    = hIndian;

    // Grupo: Cannibal_Cove
    NodoArbol* nBahia = crearNodoInterno("bahia");
    NodoArbol* nBarco = crearNodoInterno("barco");
    NodoArbol* nEvita = crearNodoInterno("evita");
    nColina->der  = nBahia;
    nBahia->izq   = hCannibal;
    nBahia->der   = nBarco;
    nBarco->izq   = hCannibal;
    nBarco->der   = nEvita;
    nEvita->izq   = hCannibal;

    // Grupo: Hanging_Tree
    NodoArbol* nArbol  = crearNodoInterno("arbol");
    NodoArbol* nSombra = crearNodoInterno("sombra");
    NodoArbol* nCuelga = crearNodoInterno("cuelga");
    nEvita->der   = nArbol;
    nArbol->izq   = hTree;
    nArbol->der   = nSombra;
    nSombra->izq  = hTree;
    nSombra->der  = nCuelga;
    nCuelga->izq  = hTree;

    // Grupo: Lost_Boys_Camp
    NodoArbol* nRisa  = crearNodoInterno("risa");
    NodoArbol* nNino  = crearNodoInterno("niño");
    NodoArbol* nJuego = crearNodoInterno("juego");
    nCuelga->der  = nRisa;
    nRisa->izq    = hLostBoys;
    nRisa->der    = nNino;
    nNino->izq    = hLostBoys;
    nNino->der    = nJuego;
    nJuego->izq   = hLostBoys;

    // Grupo: Pegleg_Point
    NodoArbol* nTierra = crearNodoInterno("tierra");
    NodoArbol* nFin    = crearNodoInterno("fin");
    NodoArbol* nPunta  = crearNodoInterno("punta");
    nJuego->der   = nTierra;
    nTierra->izq  = hPegleg;
    nTierra->der  = nFin;
    nFin->izq     = hPegleg;
    nFin->der     = nPunta;
    nPunta->izq   = hPegleg;

    // Grupo: TESORO
    NodoArbol* nX      = crearNodoInterno("X");
    NodoArbol* nTesoro = crearNodoInterno("tesoro");
    NodoArbol* nMarca  = crearNodoInterno("marca");
    nPunta->der   = nX;
    nX->izq       = hTesoro;
    nX->der       = nTesoro;
    nTesoro->izq  = hTesoro;
    nTesoro->der  = nMarca;
    nMarca->izq   = hTesoro;

    // Y si no machea ninguna de las anteriores...
    nMarca->der   = hDesc;

    // Establecer la raíz
    raiz = nCalavera;
}

// ============================================================
//  Auxiliar: Busca si la subcadena existe dentro del texto
// ============================================================
bool ArbolDecisiones::contienePalabra(const string& texto, const string& subcadena) const {
    if (subcadena.empty()) return false;
    
    // Implementación manual de búsqueda de subcadena sin <algorithm>
    for (int i = 0; i <= (int)texto.length() - (int)subcadena.length(); i++) {
        bool match = true;
        for (int j = 0; j < (int)subcadena.length(); j++) {
            // Comparación simple, asume que están en el mismo case
            if (texto[i + j] != subcadena[j]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

// ============================================================
//  2. interpretarPista: Recorre el árbol hasta una hoja
// ============================================================
string ArbolDecisiones::interpretarPista(NodoArbol* nodo, const string& pista) const {
    if (nodo == nullptr) {
        return "ERROR_ARBOL_VACIO";
    }

    if (nodo->esHoja) {
        // Encontramos el destino final
        return nodo->ubicacion;
    } else {
        // Nodo interno: preguntar si la pista contiene la palabra clave
        if (contienePalabra(pista, nodo->palabraClave)) {
            // Se encontró la palabra -> Vamos a la izquierda
            return interpretarPista(nodo->izq, pista);
        } else {
            // No se encontró -> Vamos a la derecha
            return interpretarPista(nodo->der, pista);
        }
    }
}

string ArbolDecisiones::interpretarPista(const string& pista) const {
    return interpretarPista(raiz, pista);
}

// ============================================================
//  3. mostrarArbol: Imprime visualmente (Pre-Order modificado)
// ============================================================
void ArbolDecisiones::mostrarArbol(NodoArbol* nodo, int nivel) const {
    if (nodo == nullptr) return;

    // Indentación según la profundidad
    for (int i = 0; i < nivel; i++) {
        cout << "    ";
    }

    if (nodo->esHoja) {
        cout << "└─ [UBICACION] " << nodo->ubicacion << "\n";
    } else {
        cout << "├─ ¿Contiene '" << nodo->palabraClave << "'?\n";
        
        // Rama Izquierda (Sí contiene)
        for (int i = 0; i < nivel; i++) cout << "    ";
        cout << "   SI ->\n";
        mostrarArbol(nodo->izq, nivel + 1);

        // Rama Derecha (No contiene)
        for (int i = 0; i < nivel; i++) cout << "    ";
        cout << "   NO ->\n";
        mostrarArbol(nodo->der, nivel + 1);
    }
}

void ArbolDecisiones::mostrarArbol() const {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout <<   "║     ÁRBOL DE DECISIONES DE PISTAS        ║\n";
    cout <<   "╚══════════════════════════════════════════╝\n";
    mostrarArbol(raiz, 0);
    cout << "\n";
}

// --- END arbol.cpp ---

// --- BEGIN explorador.cpp ---
// ============================================================
//  explorador.cpp  –  Implementación del Explorador
//  Proyecto Final – Estructuras de Datos
// ============================================================

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

// --- END explorador.cpp ---

// --- BEGIN archivos.cpp ---
// ============================================================
//  archivos.cpp  –  Implementación del Manejo de Archivos
//  Proyecto Final – Estructuras de Datos
// ============================================================

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

// ============================================================
//  1. cargarGrafo
// ============================================================
bool cargarGrafo(string archivo, Nodo* nd[], int &total) {
    ifstream file(archivo.c_str());
    if (!file.is_open()) {
        cout << "[ERROR] No se pudo abrir: " << archivo << "\n";
        return false;
    }

    // Inicializar el arreglo de punteros a nullptr
    for (int i = 0; i < 50; i++) {
        nd[i] = nullptr;
    }
    total = 0;

    string linea;
    bool seccionNodos = false;
    bool seccionAristas = false;

    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        if (linea == "NODOS") {
            seccionNodos = true;
            seccionAristas = false;
            continue;
        } else if (linea == "ARISTAS") {
            seccionNodos = false;
            seccionAristas = true;
            continue;
        }

        if (seccionNodos) {
            // Formato esperado: <id> <Nombre> "<pista>"
            int pos = 0;
            // 1. Leer ID
            int id = 0;
            while (pos < (int)linea.length() && linea[pos] >= '0' && linea[pos] <= '9') {
                id = id * 10 + (linea[pos] - '0');
                pos++;
            }
            
            // Saltar espacios
            while (pos < (int)linea.length() && linea[pos] == ' ') pos++;

            // 2. Leer Nombre
            string nombre = "";
            while (pos < (int)linea.length() && linea[pos] != ' ') {
                nombre += linea[pos++];
            }

            // Saltar espacios
            while (pos < (int)linea.length() && linea[pos] == ' ') pos++;

            // 3. Leer Pista (puede estar entre comillas o no)
            string pista = "";
            if (pos < (int)linea.length() && linea[pos] == '"') pos++; // saltar comilla inicial
            while (pos < (int)linea.length() && linea[pos] != '"') {
                pista += linea[pos++];
            }

            // Crear el nodo dinámicamente
            Nodo* nuevo = new Nodo;
            // Nota: Se asume que el struct Nodo de grafo.h soporta 'id' o que usas el id como índice.
            // Si el struct exacto del usuario no tiene 'id', guardamos en la posición del array directamente.
            nuevo->nombre = nombre;
            nuevo->pista = pista;
            nuevo->visitado = false;
            nuevo->ady = nullptr;

            nd[id] = nuevo;
            total++;

        } else if (seccionAristas) {
            // Formato esperado: <origen> <destino> <costo>
            int pos = 0;
            
            // Helper lambda para leer entero (sin std::stoi)
            auto leerEntero = [&]() -> int {
                while (pos < (int)linea.length() && linea[pos] == ' ') pos++;
                if (pos >= (int)linea.length()) return -1;
                int val = 0;
                while (pos < (int)linea.length() && linea[pos] >= '0' && linea[pos] <= '9') {
                    val = val * 10 + (linea[pos] - '0');
                    pos++;
                }
                return val;
            };

            int origen = leerEntero();
            int dest = leerEntero();
            int costo = leerEntero();

            if (origen != -1 && dest != -1 && costo != -1) {
                if (nd[origen] != nullptr) {
                    // Crear nueva arista e insertarla al inicio de la lista de adyacencia
                    Arista* nueva = new Arista;
                    nueva->dest = dest;
                    nueva->costo = costo;
                    nueva->sgte = nd[origen]->ady;
                    nd[origen]->ady = nueva;
                }
            }
        }
    }

    file.close();
    cout << "[OK] Grafo cargado exitosamente (" << total << " nodos).\n";
    return true;
}

// ============================================================
//  2. cargarPistas
// ============================================================
bool cargarPistas(string archivo, Nodo* nd[], int total) {
    ifstream file(archivo.c_str());
    if (!file.is_open()) {
        cout << "[ERROR] No se pudo abrir el archivo de pistas: " << archivo << "\n";
        return false;
    }

    string linea;
    int pistasCargadas = 0;

    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        // Formato: NombreNodo|pista completa en camba
        int posPipe = -1;
        for (int i = 0; i < (int)linea.length(); i++) {
            if (linea[i] == '|') {
                posPipe = i;
                break;
            }
        }

        if (posPipe != -1) {
            string nombreBuscado = "";
            for (int i = 0; i < posPipe; i++) nombreBuscado += linea[i];

            string pistaCamba = "";
            for (int i = posPipe + 1; i < (int)linea.length(); i++) pistaCamba += linea[i];

            // Buscar el nodo y actualizar su pista (búsqueda lineal O(N))
            for (int i = 0; i < 50; i++) { // Suponemos un máx de 50 (MAX_NODOS)
                if (nd[i] != nullptr && nd[i]->nombre == nombreBuscado) {
                    nd[i]->pista = pistaCamba;
                    pistasCargadas++;
                    break;
                }
            }
        }
    }

    file.close();
    cout << "[OK] Pistas cargadas exitosamente (" << pistasCargadas << " actualizadas).\n";
    return true;
}

// ============================================================
//  3. guardarRuta
// ============================================================
bool guardarRuta(string archivo, int camino[], int longitud, Nodo* nd[], int costoTotal) {
    ofstream file(archivo.c_str());
    if (!file.is_open()) {
        cout << "[ERROR] No se pudo crear el archivo de resultado: " << archivo << "\n";
        return false;
    }

    // Obtener fecha y hora del sistema
    time_t ahora = time(0);
    char* dt = ctime(&ahora); // convierte a string formato local

    file << "========================================================\n";
    file << "              BITÁCORA DE LA RUTA AL TESORO             \n";
    file << "========================================================\n";
    file << "Fecha de expedición: " << dt << "\n";
    file << "Costo total de la travesía: " << costoTotal << " días/monedas.\n\n";

    file << "--- CAMINO RECORRIDO ---\n";
    for (int i = 0; i < longitud; i++) {
        int idNodo = camino[i];
        if (nd[idNodo] != nullptr) {
            file << "Paso " << i + 1 << ": [" << nd[idNodo]->nombre << "]\n";
            file << "   Pista hallada: \"" << nd[idNodo]->pista << "\"\n\n";
        }
    }

    file << "========================================================\n";
    file << "¡El tesoro ha sido reclamado!\n";
    
    file.close();
    cout << "[OK] Ruta guardada exitosamente en " << archivo << ".\n";
    return true;
}

// ============================================================
//  4. guardarDiccionario
// ============================================================
// Función auxiliar para recorrer in-order y escribir en archivo
void escribirBST(ofstream& file, NodoBST* nodo) {
    if (nodo != nullptr) {
        escribirBST(file, nodo->izq);
        file << nodo->palabra << ": " << nodo->significado << "\n";
        escribirBST(file, nodo->der);
    }
}

bool guardarDiccionario(string archivo, TablaHashEstructura &tabla) {
    ofstream file(archivo.c_str());
    if (!file.is_open()) {
        cout << "[ERROR] No se pudo crear el archivo del diccionario: " << archivo << "\n";
        return false;
    }

    file << "################################################\n";
    file << "# DICCIONARIO DEL HABLA POPULAR DE SANTA CRUZ  #\n";
    file << "################################################\n\n";

    for (int i = 0; i < 28; i++) {
        if (tabla.tabla[i] != nullptr) {
            escribirBST(file, tabla.tabla[i]);
        }
    }

    file.close();
    cout << "[OK] Diccionario exportado exitosamente en " << archivo << ".\n";
    return true;
}

// --- END archivos.cpp ---

// --- BEGIN main.cpp ---
// ============================================================
//  main.cpp  –  EL TESORO DEL PIRATA
//  Proyecto Final – Estructuras de Datos
// ============================================================


using namespace std;

// ============================================================
//  Imprimir ASCII Art de Pirata
// ============================================================
void mostrarAsciiArt() {
    cout << "\n";
    cout << "       .---.          \n";
    cout << "      /     \\        \n";
    cout << "      \\.@-@./        \n";
    cout << "      /`\\_/`\\      \n";
    cout << "     //  _  \\\\     EL TESORO DEL PIRATA \n";
    cout << "    | \\     / |     PROYECTO FINAL ED \n";
    cout << "   /`\\_`>  <_/`\\   \n";
    cout << "   \\__/'---'\\__/   \n\n";
}

// ============================================================
//  Imprimir Mapa Visual de Neverland (ASCII)
// ============================================================
void mostrarMapaVisual(Nodo* nd[]) {
    // Si el nodo 0 no existe, no está cargado
    if (nd[0] == nullptr) return;

    cout << "\n========================================================================\n";
    cout << "                    MAPA VISUAL DE NEVERLAND                            \n";
    cout << "========================================================================\n\n";

    cout << "   [0] Skull_Rock                          [7] Pegleg_Point             \n";
    cout << "         | (Costo: 4)                             | (Costo: 8)          \n";
    cout << "         v                                        v                     \n";
    cout << "   [2] Crocodile_Creek ------------------> [4] Cannibal_Cove -> [8] TESORO\n";
    cout << "         ^                     (Costo: 7)         ^             (Costo: 2)\n";
    cout << "         | (Costo: 3)                             |                     \n";
    cout << "   [1] Mermaid_Lagoon -------------------> [6] Lost_Boys_Camp           \n";
    cout << "         ^                     (Costo: 3)         |                     \n";
    cout << "         | (Costo: 5)                             | (Costo: 4)          \n";
    cout << "         |                                        v                     \n";
    cout << "   [3] Indian_Camp <---------------------- [5] Hanging_Tree             \n";
    cout << "                               (Costo: 6)                               \n";
    cout << "\n========================================================================\n";
}

int consola_main() {
    // Requisito: Consola verde hacker/pirata
    system("color 0a");
    mostrarAsciiArt();

    // ========================================================
    //  ESTADO DEL PROGRAMA
    // ========================================================
    Nodo* nd[50];
    for (int i = 0; i < 50; i++) nd[i] = nullptr;
    int totalNodos = 0;
    bool mapaCargado = false;

    DiccionarioHash dicc;
    bool diccCargado = false;

    ArbolDecisiones arbol;
    arbol.construirArbol(); // Construir reglas de pistas en memoria

    Explorador explo;

    // Variables para Dijkstra
    int padre[50];
    int ultimoCosto = -1;
    bool rutaEncontrada = false;
    int destinoDijkstra = 8; // TESORO

    int opcion;
    do {
        cout << "\n=== EL TESORO DEL PIRATA ===\n";
        cout << "--- MAPA DE LA ISLA ---\n";
        cout << "1. Cargar mapa desde archivo\n";
        cout << "2. Mostrar mapa\n";
        cout << "3. Explorar con BFS\n";
        cout << "4. Explorar con DFS\n";
        cout << "5. Encontrar ruta mas corta (Dijkstra)\n";
        cout << "6. Interpretar pista de una ubicacion\n";
        cout << "7. Guardar ruta al tesoro\n\n";
        
        cout << "--- DICCIONARIO CAMBA ---\n";
        cout << "8.  Cargar diccionario desde archivo\n";
        cout << "9.  Buscar palabra\n";
        cout << "10. Agregar palabra\n";
        cout << "11. Eliminar palabra\n";
        cout << "12. Mostrar diccionario completo\n";
        cout << "13. Mostrar estadisticas de la tabla hash\n";
        cout << "0.  Salir\n";
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        // ====================================================
        //  Validaciones
        // ====================================================
        if (opcion >= 2 && opcion <= 7 && !mapaCargado) {
            cout << "\n[!] ERROR: Debes cargar el mapa primero (Opcion 1).\n";
            continue;
        }
        if (opcion >= 9 && opcion <= 13 && !diccCargado) {
            cout << "\n[!] ERROR: Debes cargar el diccionario primero (Opcion 8).\n";
            continue;
        }

        // ====================================================
        //  Acciones
        // ====================================================
        switch (opcion) {
            case 1: {
                if (cargarGrafo("mapa_isla.txt", nd, totalNodos)) {
                    cargarPistas("pistas.txt", nd, totalNodos);
                    mapaCargado = true;
                }
                break;
            }
            case 2: {
                cout << "\n=== MAPA ACTUAL DE NEVERLAND ===\n";
                for (int i = 0; i < 50; i++) {
                    if (nd[i] != nullptr) {
                        cout << "[" << i << "] " << nd[i]->nombre << " -> Pista: " << nd[i]->pista << "\n";
                        Arista* ady = nd[i]->ady;
                        while (ady != nullptr) {
                            int d = ady->dest;
                            string nomDest = nd[d] ? nd[d]->nombre : "?";
                            cout << "    --(Costo: " << ady->costo << ")--> [" << d << "] " << nomDest << "\n";
                            ady = ady->sgte;
                        }
                    }
                }
                
                // Mostrar diagrama ASCII de la isla
                mostrarMapaVisual(nd);
                break;
            }
            case 3:
                explo.BFS(nd, totalNodos, 0); // Empieza de Skull_Rock (0)
                break;
            case 4:
                explo.DFS(nd, totalNodos, 0); // Empieza de Skull_Rock (0)
                break;
            case 5: {
                ultimoCosto = explo.Dijkstra(nd, totalNodos, 0, destinoDijkstra, padre);
                if (ultimoCosto != -1) {
                    cout << "\n[EXITO] El costo minimo para llegar al TESORO es: " << ultimoCosto << "\n";
                    explo.imprimirCamino(nd, padre, destinoDijkstra);
                    rutaEncontrada = true;

                    // Extraer el camino para guardarlo automaticamente
                    int camino[50];
                    int longitud = 0;
                    int actual = destinoDijkstra;
                    while(actual != -1) {
                        camino[longitud++] = actual;
                        actual = padre[actual];
                    }
                    // Invertir arreglo para guardarlo origen->destino
                    for(int i = 0; i < longitud/2; i++){
                        int temp = camino[i];
                        camino[i] = camino[longitud - 1 - i];
                        camino[longitud - 1 - i] = temp;
                    }
                    guardarRuta("resultado_ruta.txt", camino, longitud, nd, ultimoCosto);
                } else {
                    cout << "\n[FALLO] No se encontro ruta al TESORO usando Dijkstra.\n";
                    rutaEncontrada = false;
                }
                break;
            }
            case 6: {
                string nomUbicacion;
                cout << "Ingrese el nombre del lugar sin espacios (Ej. Skull_Rock): ";
                cin >> nomUbicacion;
                bool hallado = false;
                for (int i = 0; i < 50; i++) {
                    if (nd[i] != nullptr && nd[i]->nombre == nomUbicacion) {
                        cout << "Interpretando pista: \"" << nd[i]->pista << "\"\n";
                        string dest = arbol.interpretarPista(nd[i]->pista);
                        cout << "==> El arbol dice que debes dirigirte a: " << dest << "\n";
                        hallado = true;
                        break;
                    }
                }
                if (!hallado) cout << "[!] Ubicacion no encontrada en el grafo.\n";
                break;
            }
            case 7: {
                if (!rutaEncontrada) {
                    cout << "[!] Debes ejecutar Dijkstra primero (Opcion 5) para tener una ruta.\n";
                } else {
                    int camino[50];
                    int longitud = 0;
                    int actual = destinoDijkstra;
                    while(actual != -1) {
                        camino[longitud++] = actual;
                        actual = padre[actual];
                    }
                    for(int i = 0; i < longitud/2; i++){
                        int temp = camino[i];
                        camino[i] = camino[longitud - 1 - i];
                        camino[longitud - 1 - i] = temp;
                    }
                    guardarRuta("resultado_ruta_manual.txt", camino, longitud, nd, ultimoCosto);
                }
                break;
            }
            case 8: {
                // Para simplificar la firma que teniamos, usamos la funcion miembro
                dicc.cargarDesdeArchivo("diccionario_camba.txt");
                diccCargado = true;
                break;
            }
            case 9: {
                string palabra;
                cout << "Ingrese palabra a buscar (en mayusculas preferentemente): ";
                cin >> palabra;
                cout << "Significado: " << dicc.buscar(palabra) << "\n";
                break;
            }
            case 10: {
                string palabra, sig;
                cout << "Ingrese palabra nueva: ";
                cin >> palabra;
                cout << "Ingrese significado (una sola palabra para simplificar cin, o use '_'): ";
                cin >> sig;
                dicc.insertar(palabra, sig);
                cout << "Palabra insertada exitosamente.\n";
                break;
            }
            case 11: {
                string palabra;
                cout << "Ingrese palabra a eliminar: ";
                cin >> palabra;
                if (dicc.eliminar(palabra)) cout << "Eliminada correctamente.\n";
                else cout << "No se encontro la palabra para eliminar.\n";
                break;
            }
            case 12:
                dicc.mostrarTabla();
                break;
            case 13:
                dicc.mostrarEstadisticas();
                break;
            case 0:
                cout << "Saliendo... ¡Que el mar te sea leve, marinero!\n";
                break;
            default:
                cout << "Opcion invalida.\n";
                break;
        }

    } while (opcion != 0);

    // Limpieza de memoria
    for (int i = 0; i < 50; i++) {
        if (nd[i] != nullptr) {
            Arista* ady = nd[i]->ady;
            while (ady != nullptr) {
                Arista* temp = ady;
                ady = ady->sgte;
                delete temp;
            }
            delete nd[i];
        }
    }

    return 0;
}

// --- END main.cpp ---

