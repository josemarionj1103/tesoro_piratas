// ============================================================
//  archivos.cpp  –  Implementación del Manejo de Archivos
//  Proyecto Final – Estructuras de Datos
// ============================================================
#include "archivos.h"
#include <iostream>
#include <fstream>
#include <ctime>

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
