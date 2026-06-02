// ============================================================
//  arbol.h  –  Árbol Binario de Decisiones para Pistas
//  Proyecto Final – Estructuras de Datos
// ============================================================
#ifndef ARBOL_H
#define ARBOL_H

#include <string>
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

#endif // ARBOL_H
