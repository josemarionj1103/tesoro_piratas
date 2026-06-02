// ============================================================
//  hash.cpp  –  Implementación del Diccionario Camba
//  Proyecto Final – Estructuras de Datos
// ============================================================
#include "hash.h"
#include <iostream>
#include <fstream>

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
        suma += palabra[i];
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
                significado += linea[i];
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
