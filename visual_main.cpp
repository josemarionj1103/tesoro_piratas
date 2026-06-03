#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

#include "estructuras.h"

using namespace std;

// Estructura de posiciones para el mapa
struct Point2D {
    float x, y;
};

// Coordenadas mejoradas (adaptadas al nuevo ancho y solucionando el overlap)
Point2D posicionesIsla[50] = {
    {150, 150}, // 0: Skull_Rock
    {150, 480}, // 1: Mermaid_Lagoon
    {380, 280}, // 2: Crocodile_Creek
    {380, 520}, // 3: Indian_Camp (subido)
    {700, 280}, // 4: Cannibal_Cove
    {700, 520}, // 5: Hanging_Tree (subido)
    {540, 450}, // 6: Lost_Boys_Camp
    {900, 150}, // 7: Pegleg_Point
    {950, 420}  // 8: TESORO
};

Point2D posicionesShrek[50] = {
    {500, 120}, // 0: Shreks_Swamp
    {100, 300}, // 1: Far_Far_Away
    {700, 200}, // 2: Duloc
    {850, 350}, // 3: Fionas_Tower
    {750, 450}, // 4: The_Dark_Forest
    {600, 350}, // 5: Jack_Horners_Factory
    {350, 350}, // 6: Pirate_Ship
    {180, 420}, // 7: Del_Mar
    {100, 500}, // 8: Fairy_Godmothers_Factory
    {300, 520}, // 9: Mama_Lunas_House
    {450, 480}, // 10: Giant_Bean
    {600, 520}, // 11: San_Ricardo
    {880, 550}, // 12: San_Lorenzo
    {900, 100}, // 13: 3_Bears_House
    {200, 150}, // 14: Merlins_House
    {350, 200}  // 15: Worcestershire
};

Point2D* posiciones = posicionesIsla; // Puntero al mapa actual
int currentMap = 0; // 0: Isla Pirata, 1: Mundo de Shrek

// Paleta de colores Premium
Color colorFondo = { 15, 30, 45, 255 };       // Azul marino muy oscuro
Color colorOceano = { 20, 100, 140, 255 };    // Azul oceano vibrante
Color colorPantano = { 40, 70, 45, 255 };     // Verde pantano para Shrek
Color colorTierra = { 80, 55, 35, 255 };      // Marron tierra para Shrek
Color colorPanel = { 30, 45, 65, 255 };       // Gris azulado para paneles
Color colorBoton = { 50, 130, 180, 255 };     // Azul claro
Color colorBotonHover = { 70, 160, 210, 255 };
Color colorTexto = { 230, 240, 255, 255 };    // Blanco puro
Color colorAcento = { 240, 180, 40, 255 };    // Dorado tesoro

// Función auxiliar para botones
bool DrawButton(Rectangle rect, const char* text, Color baseColor, Color hoverColor) {
    bool clicked = false;
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);
    
    // Sombra suave
    DrawRectangleRounded((Rectangle){rect.x + 3, rect.y + 4, rect.width, rect.height}, 0.3f, 8, Fade(BLACK, 0.4f));
    
    // Fondo del boton
    Color current = hover ? hoverColor : baseColor;
    DrawRectangleRounded(rect, 0.3f, 8, current);
    
    // Borde brillante si hace hover
    if (hover) {
        DrawRectangleRoundedLines(rect, 0.3f, 8, Fade(WHITE, 0.5f));
    }
    
    // Texto
    int fontSize = 16;
    int tw = MeasureText(text, fontSize);
    DrawText(text, rect.x + rect.width/2 - tw/2, rect.y + rect.height/2 - fontSize/2, fontSize, WHITE);
    
    if (hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        clicked = true;
    }
    return clicked;
}

// Interfaz del juego
int main() {
    // Dimensiones mas anchas para el HUD
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    // Habilitar antialiasing para lineas mas suaves
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "El Tesoro del Pirata - Visual Premium");
    SetTargetFPS(60);

    // ==========================================
    // Inicializar Backend
    // ==========================================
    Nodo* nd[50];
    int totalNodos = 0;
    for (int i = 0; i < 50; i++) nd[i] = nullptr;

    cargarGrafo("mapa_isla.txt", nd, totalNodos);
    cargarPistas("pistas.txt", nd, totalNodos);

    DiccionarioHash dicc;
    dicc.cargarDesdeArchivo("diccionario_camba.txt");

    ArbolDecisiones arbol;
    arbol.construirArbol();
    Explorador explo;
    
    int padre[50];
    int destinoDijkstra = 8;
    int nodoActual = 0;
    bool mostrarDijkstra = false;
    
    string mensajePista = "Haz clic en una isla para viajar y leer su pista.";
    string mensajeSistema = "Bienvenido a Neverland.";
    
    // Modos de Vista
    enum Vista { MAPA, CONSOLA, INPUT_TEXTO, INPUT_DOBLE };
    Vista vistaActual = MAPA;
    string consolaOutput = "";
    
    // Variables para input
    string inputTitulo = "";
    string inputText = "";
    string inputText2 = "";
    int inputModoAccion = 0; // 1: Buscar, 2: Eliminar, 3: Agregar
    bool editandoPrimerInput = true; // Para agregar palabra (Palabra vs Significado)

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();
        
        // ==========================================
        // LOGICA DE INPUT TEXTO
        // ==========================================
        if (vistaActual == INPUT_TEXTO || vistaActual == INPUT_DOBLE) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125)) { // Caracteres imprimibles
                    if (editandoPrimerInput && inputText.length() < 30) inputText += (char)toupper(key); // Forzar mayus
                    else if (!editandoPrimerInput && inputText2.length() < 100) inputText2 += (char)key;
                }
                key = GetCharPressed();
            }
            
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (editandoPrimerInput && inputText.length() > 0) inputText.pop_back();
                else if (!editandoPrimerInput && inputText2.length() > 0) inputText2.pop_back();
            }
            
            if (IsKeyPressed(KEY_TAB) && vistaActual == INPUT_DOBLE) {
                editandoPrimerInput = !editandoPrimerInput; // Alternar campo
            }
            
            if (IsKeyPressed(KEY_ENTER)) {
                // Ejecutar accion
                consolaOutput = "";
                if (inputModoAccion == 1) { // Buscar
                    string sig = dicc.buscar(inputText);
                    if (sig == "") consolaOutput = "Palabra NO encontrada en el diccionario.";
                    else consolaOutput = "Significado de " + inputText + ":\n\n" + sig;
                    vistaActual = CONSOLA;
                }
                else if (inputModoAccion == 2) { // Eliminar
                    if (dicc.eliminar(inputText)) consolaOutput = "La palabra " + inputText + " fue eliminada.";
                    else consolaOutput = "Error: Palabra no encontrada.";
                    vistaActual = CONSOLA;
                }
                else if (inputModoAccion == 3) { // Agregar
                    if (inputText != "" && inputText2 != "") {
                        dicc.insertar(inputText, inputText2);
                        consolaOutput = "La palabra " + inputText + " fue añadida exitosamente.";
                        vistaActual = CONSOLA;
                    }
                }
            }
            
            if (IsKeyPressed(KEY_ESCAPE)) {
                vistaActual = MAPA;
            }
        }
        // ==========================================
        // LOGICA DEL MAPA
        // ==========================================
        else if (vistaActual == MAPA) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mousePoint.x < 1000) { // Si hace clic en la zona del mapa
                for (int i = 0; i < 50; i++) {
                    if (nd[i] != nullptr) {
                        float dist = sqrt(pow(mousePoint.x - posiciones[i].x, 2) + pow(mousePoint.y - posiciones[i].y, 2));
                        if (dist <= 35.0f) {
                            nodoActual = i;
                            mensajeSistema = "Viajaste a: " + nd[i]->nombre;
                            string destPista = arbol.interpretarPista(nd[i]->pista);
                            mensajePista = "Pista: " + nd[i]->pista + "\n(Destino sugerido por el arbol: " + destPista + ")";
                            mostrarDijkstra = false;
                            break;
                        }
                    }
                }
            }
        }
        else if (vistaActual == CONSOLA) {
            if (IsKeyPressed(KEY_ESCAPE)) vistaActual = MAPA;
        }

        // ==========================================
        // DRAW
        // ==========================================
        BeginDrawing();
        ClearBackground(colorFondo);

        // --- ZONA DEL MAPA (Ancho 1000) ---
        if (vistaActual == MAPA) {
            // Fondo dinamico segun el mapa
            if (currentMap == 0) {
                DrawRectangleGradientV(0, 0, 1000, 720, colorOceano, colorFondo);
            } else {
                DrawRectangleGradientV(0, 0, 1000, 720, colorPantano, colorTierra);
            }
            
            // Rejilla nautica de adorno
            for(int i=0; i<1000; i+=100) DrawLineEx((Vector2){(float)i, 0}, (Vector2){(float)i, 720}, 1, Fade(WHITE, 0.05f));
            for(int j=0; j<720; j+=100) DrawLineEx((Vector2){0, (float)j}, (Vector2){1000, (float)j}, 1, Fade(WHITE, 0.05f));

            // Dibujar aristas (Caminos)
            for (int i = 0; i < 50; i++) {
                if (nd[i] != nullptr) {
                    Arista* ady = nd[i]->ady;
                    while (ady != nullptr) {
                        int d = ady->dest;
                        if (d >= 0 && d < 50 && nd[d] != nullptr) {
                            // Sombra de la linea
                            DrawLineEx(Vector2{posiciones[i].x+3, posiciones[i].y+3}, Vector2{posiciones[d].x+3, posiciones[d].y+3}, 4.0f, Fade(BLACK, 0.3f));
                            // Linea principal
                            DrawLineEx(Vector2{posiciones[i].x, posiciones[i].y}, Vector2{posiciones[d].x, posiciones[d].y}, 3.0f, Fade(WHITE, 0.5f));
                            
                            // Costo
                            int midX = (posiciones[i].x + posiciones[d].x) / 2;
                            int midY = (posiciones[i].y + posiciones[d].y) / 2;
                            DrawCircle(midX, midY, 14, Fade(BLACK, 0.7f));
                            DrawText(TextFormat("%d", ady->costo), midX - MeasureText(TextFormat("%d", ady->costo), 14)/2, midY - 7, 14, WHITE);
                        }
                        ady = ady->sgte;
                    }
                }
            }
            
            // Dibujar ruta Dijkstra si esta activa
            if (mostrarDijkstra) {
                int actual = destinoDijkstra;
                while (actual >= 0 && actual < 50 && padre[actual] != -1) {
                    int p = padre[actual];
                    if (p >= 0 && p < 50) {
                        DrawLineEx(Vector2{posiciones[actual].x, posiciones[actual].y}, Vector2{posiciones[p].x, posiciones[p].y}, 8.0f, Fade(colorAcento, 0.8f));
                    }
                    actual = p;
                }
            }

            // Dibujar nodos (Islas)
            for (int i = 0; i < 50; i++) {
                if (nd[i] != nullptr) {
                    Color col = (i == destinoDijkstra) ? colorAcento : LIGHTGRAY;
                    if (i == nodoActual) col = GREEN;
                    
                    // Sombra
                    DrawCircle(posiciones[i].x + 4, posiciones[i].y + 4, 32, Fade(BLACK, 0.5f));
                    // Circulo principal
                    DrawCircle(posiciones[i].x, posiciones[i].y, 32, col);
                    // Borde interior
                    DrawCircleLines(posiciones[i].x, posiciones[i].y, 32, Fade(BLACK, 0.4f));
                    DrawCircleLines(posiciones[i].x, posiciones[i].y, 30, Fade(WHITE, 0.3f));
                    
                    // Formatear texto (reemplazar '_' por espacio)
                    string displayName = nd[i]->nombre;
                    for (int c = 0; c < (int)displayName.length(); c++) {
                        if (displayName[c] == '_') displayName[c] = ' ';
                    }
                    
                    int tw = MeasureText(displayName.c_str(), 14);
                    
                    // Fondo negro para el texto para legibilidad
                    DrawRectangle(posiciones[i].x - tw/2 - 4, posiciones[i].y + 40, tw + 8, 20, Fade(BLACK, 0.7f));
                    DrawText(displayName.c_str(), posiciones[i].x - tw/2, posiciones[i].y + 43, 14, WHITE);
                }
            }
            
            // Panel inferior de pista
            DrawRectangle(0, 620, 1000, 100, Fade(BLACK, 0.85f));
            DrawText(mensajeSistema.c_str(), 20, 635, 18, GRAY);
            DrawText(mensajePista.c_str(), 20, 665, 20, colorAcento);
        }
        else if (vistaActual == CONSOLA) {
            DrawRectangleGradientV(0, 0, 1000, 720, (Color){10,10,15,255}, (Color){20,20,30,255});
            DrawText("--- PANTALLA DE DATOS --- (Pulsa ESC para volver al mapa)", 30, 30, 20, GRAY);
            
            // Dibujar el output de consola
            // Como puede ser multilinea, lo dibujamos todo.
            DrawText(consolaOutput.c_str(), 30, 80, 18, GREEN);
        }
        else if (vistaActual == INPUT_TEXTO || vistaActual == INPUT_DOBLE) {
            DrawRectangleGradientV(0, 0, 1000, 720, Fade(BLACK, 0.8f), Fade(BLACK, 0.9f));
            DrawText(inputTitulo.c_str(), 100, 200, 30, colorAcento);
            
            // Caja 1 (Palabra)
            Color box1Col = editandoPrimerInput ? colorBotonHover : DARKGRAY;
            DrawRectangle(100, 260, 800, 50, box1Col);
            DrawRectangleLines(100, 260, 800, 50, RAYWHITE);
            DrawText(("Palabra: " + inputText + (editandoPrimerInput ? "_" : "")).c_str(), 115, 275, 20, WHITE);
            
            if (vistaActual == INPUT_DOBLE) {
                // Caja 2 (Significado)
                Color box2Col = !editandoPrimerInput ? colorBotonHover : DARKGRAY;
                DrawRectangle(100, 340, 800, 50, box2Col);
                DrawRectangleLines(100, 340, 800, 50, RAYWHITE);
                DrawText(("Significado: " + inputText2 + (!editandoPrimerInput ? "_" : "")).c_str(), 115, 355, 20, WHITE);
                DrawText("[TAB] para cambiar de caja. [ENTER] para guardar. [ESC] Cancelar.", 100, 420, 16, GRAY);
            } else {
                DrawText("[ENTER] para confirmar. [ESC] Cancelar.", 100, 340, 16, GRAY);
            }
        }

        // --- ZONA DEL HUD (Ancho 280, X = 1000) ---
        DrawRectangle(1000, 0, 280, 720, colorPanel);
        DrawLine(1000, 0, 1000, 720, Fade(WHITE, 0.2f));
        
        DrawText("MENÚ DE COMANDOS", 1025, 20, 22, colorAcento);
        DrawLine(1020, 50, 1260, 50, Fade(WHITE, 0.2f));
        
        DrawText("NAVEGACIÓN", 1020, 70, 14, GRAY);
        if (DrawButton((Rectangle){1020, 95, 240, 40}, "Ruta Dijkstra al Tesoro", colorBoton, colorBotonHover)) {
            explo.Dijkstra(nd, totalNodos, nodoActual, destinoDijkstra, padre);
            mostrarDijkstra = true;
            mensajeSistema = "Calculando la ruta optima desde " + nd[nodoActual]->nombre;
            vistaActual = MAPA;
        }
        if (DrawButton((Rectangle){1020, 145, 115, 40}, "BFS", colorBoton, colorBotonHover)) {
            // Capturar cout
            stringstream buffer;
            streambuf* oldCout = cout.rdbuf(buffer.rdbuf());
            explo.BFS(nd, totalNodos, 0);
            cout.rdbuf(oldCout);
            consolaOutput = buffer.str();
            vistaActual = CONSOLA;
        }
        if (DrawButton((Rectangle){1020 + 125, 145, 115, 40}, "DFS", colorBoton, colorBotonHover)) {
            stringstream buffer;
            streambuf* oldCout = cout.rdbuf(buffer.rdbuf());
            explo.DFS(nd, totalNodos, 0);
            cout.rdbuf(oldCout);
            consolaOutput = buffer.str();
            vistaActual = CONSOLA;
        }
        if (DrawButton((Rectangle){1020, 195, 240, 40}, "Ver Mapa", colorBoton, colorBotonHover)) {
            vistaActual = MAPA;
        }

        DrawText("DICCIONARIO CAMBA", 1020, 270, 14, GRAY);
        if (DrawButton((Rectangle){1020, 295, 240, 40}, "Buscar Palabra", colorBoton, colorBotonHover)) {
            vistaActual = INPUT_TEXTO;
            inputModoAccion = 1;
            inputText = "";
            inputTitulo = "BUSCAR PALABRA EN EL DICCIONARIO";
        }
        if (DrawButton((Rectangle){1020, 345, 240, 40}, "Añadir Palabra", colorBoton, colorBotonHover)) {
            vistaActual = INPUT_DOBLE;
            inputModoAccion = 3;
            inputText = "";
            inputText2 = "";
            editandoPrimerInput = true;
            inputTitulo = "AGREGAR NUEVA PALABRA AL DICCIONARIO";
        }
        if (DrawButton((Rectangle){1020, 395, 240, 40}, "Eliminar Palabra", (Color){180, 50, 50, 255}, (Color){210, 70, 70, 255})) {
            vistaActual = INPUT_TEXTO;
            inputModoAccion = 2;
            inputText = "";
            inputTitulo = "ELIMINAR PALABRA DEL DICCIONARIO";
        }
        if (DrawButton((Rectangle){1020, 445, 240, 40}, "Estadísticas de Tabla", colorBoton, colorBotonHover)) {
            stringstream buffer;
            streambuf* oldCout = cout.rdbuf(buffer.rdbuf());
            dicc.mostrarEstadisticas();
            cout.rdbuf(oldCout);
            consolaOutput = buffer.str();
            vistaActual = CONSOLA;
        }

        DrawText("SISTEMA", 1020, 530, 14, GRAY);
        if (DrawButton((Rectangle){1020, 555, 240, 40}, "Guardar Ruta a Archivo", colorBoton, colorBotonHover)) {
            if (mostrarDijkstra) {
                // Generar array de ruta invertido como en consola
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
                
                stringstream buffer;
                streambuf* oldCout = cout.rdbuf(buffer.rdbuf());
                // Llamar a guardar ruta (costo total = 0 simplificado para este ejemplo o recalculado)
                int costoFinal = 0; // Se podria calcular sumando aristas
                guardarRuta("resultado_ruta_visual.txt", camino, longitud, nd, costoFinal);
                cout.rdbuf(oldCout);
                
                mensajeSistema = "Ruta Dijkstra guardada en resultado_ruta_visual.txt";
            } else {
                mensajeSistema = "[!] Ejecuta Dijkstra primero para guardar.";
            }
            vistaActual = MAPA;
        }
        
        DrawLine(1020, 610, 1260, 610, Fade(WHITE, 0.2f));
        if (DrawButton((Rectangle){1020, 630, 240, 40}, currentMap == 0 ? "Cambiar a Mapa de Shrek" : "Cambiar a Mapa Pirata", (Color){180, 100, 50, 255}, (Color){210, 130, 70, 255})) {
            // Liberar memoria actual
            for (int i = 0; i < 50; i++) {
                if (nd[i] != nullptr) {
                    Arista* ady = nd[i]->ady;
                    while (ady != nullptr) {
                        Arista* temp = ady;
                        ady = ady->sgte;
                        delete temp;
                    }
                    delete nd[i];
                    nd[i] = nullptr;
                }
            }
            
            // Cambiar de mapa
            totalNodos = 0;
            if (currentMap == 0) {
                currentMap = 1;
                posiciones = posicionesShrek;
                cargarGrafo("mapa_shrek.txt", nd, totalNodos);
                cargarPistas("pistas_shrek.txt", nd, totalNodos);
                destinoDijkstra = 1; // Far Far Away
                nodoActual = 0; // Shreks Swamp
                mensajeSistema = "Bienvenido a Muy Muy Lejano.";
            } else {
                currentMap = 0;
                posiciones = posicionesIsla;
                cargarGrafo("mapa_isla.txt", nd, totalNodos);
                cargarPistas("pistas.txt", nd, totalNodos);
                destinoDijkstra = 8; // Tesoro
                nodoActual = 0; // Skull Rock
                mensajeSistema = "Bienvenido a Neverland.";
            }
            mostrarDijkstra = false;
            mensajePista = "Haz clic en una ubicacion para viajar.";
            vistaActual = MAPA;
        }

        EndDrawing();
    }

    // Limpieza
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

    CloseWindow();
    return 0;
}
