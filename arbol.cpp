// ============================================================
//  arbol.cpp  –  Implementación del Árbol de Decisiones
//  Proyecto Final – Estructuras de Datos
// ============================================================
#include "arbol.h"
#include <iostream>

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
