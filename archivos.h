// ============================================================
//  archivos.h  –  Módulo de Manejo de Archivos
//  Proyecto Final – Estructuras de Datos
//  Requisito: Uso estricto de ifstream/ofstream sin contenedores STL
// ============================================================
#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <string>
#include "grafo.h" // Aquí debe estar definido el struct Nodo y Arista
#include "hash.h"  // Aquí debe estar definido el struct TablaHashEstructura y NodoBST

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

#endif // ARCHIVOS_H
