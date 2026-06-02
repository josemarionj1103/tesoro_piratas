// ============================================================
//  hash.h  –  Diccionario del Habla Popular de Santa Cruz
//  Proyecto Final – Estructuras de Datos
//  Requisito: Tabla Hash con 28 posiciones y colisiones por BST
// ============================================================
#ifndef HASH_H
#define HASH_H

#include <string>
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

#endif // HASH_H
