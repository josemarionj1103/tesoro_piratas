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

// Coordenadas del Mapa Pirata
Point2D posicionesIsla[50] = {
    {150, 130}, // 0: Skull_Rock
    {150, 460}, // 1: Mermaid_Lagoon
    {380, 270}, // 2: Crocodile_Creek
    {400, 500}, // 3: Indian_Camp
    {680, 260}, // 4: Cannibal_Cove
    {720, 500}, // 5: Hanging_Tree
    {550, 420}, // 6: Lost_Boys_Camp
    {880, 130}, // 7: Pegleg_Point
    {920, 400}  // 8: TESORO
};

// Coordenadas del Mapa de Shrek (bien espaciadas)
Point2D posicionesShrek[50] = {
    {490, 110}, // 0: Shreks_Swamp
    { 90, 280}, // 1: Far_Far_Away
    {700, 190}, // 2: Duloc
    {860, 330}, // 3: Fionas_Tower
    {740, 450}, // 4: The_Dark_Forest
    {580, 340}, // 5: Jack_Horners_Factory
    {340, 340}, // 6: Pirate_Ship
    {165, 420}, // 7: Del_Mar
    { 90, 510}, // 8: Fairy_Godmothers_Factory
    {290, 510}, // 9: Mama_Lunas_House
    {445, 480}, // 10: Giant_Bean
    {600, 510}, // 11: San_Ricardo
    {870, 550}, // 12: San_Lorenzo
    {880,  90}, // 13: 3_Bears_House
    {195, 140}, // 14: Merlins_House
    {345, 195}  // 15: Worcestershire
};

Point2D* posiciones = posicionesIsla;
int currentMap = 0; // 0 = Pirata, 1 = Shrek

// Colores
Color colorFondo       = {  15,  30,  45, 255 };
Color colorPanel       = {  20,  35,  55, 230 };
Color colorBoton       = {  50, 130, 180, 255 };
Color colorBotonHover  = {  70, 160, 210, 255 };
Color colorAcento      = { 240, 180,  40, 255 };

// ---------------------------------------------------------------------------
// Boton con hover + sombra
// ---------------------------------------------------------------------------
bool DrawButton(Rectangle rect, const char* text, Color base, Color hover) {
    Vector2 mouse = GetMousePosition();
    bool over = CheckCollisionPointRec(mouse, rect);
    // Sombra
    DrawRectangleRounded({rect.x+3, rect.y+4, rect.width, rect.height}, 0.35f, 8, Fade(BLACK, 0.45f));
    // Fondo
    DrawRectangleRounded(rect, 0.35f, 8, over ? hover : base);
    // Borde hover
    if (over) DrawRectangleRoundedLines(rect, 0.35f, 8, Fade(WHITE, 0.45f));
    // Texto centrado
    int fs = 16, tw = MeasureText(text, fs);
    DrawText(text, (int)(rect.x + rect.width/2 - tw/2), (int)(rect.y + rect.height/2 - fs/2), fs, WHITE);
    return over && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

// ---------------------------------------------------------------------------
// Dibujar texto formateado (reemplaza _ con espacio)
// ---------------------------------------------------------------------------
string fmtName(const string& s) {
    string r = s;
    for (char& c : r) if (c == '_') c = ' ';
    return r;
}

// ---------------------------------------------------------------------------
// Dibuja un cartel de texto con fondo, bien centrado encima/debajo del nodo
// ---------------------------------------------------------------------------
void DrawLabel(int x, int y, const string& text, int fs = 13) {
    int tw = MeasureText(text.c_str(), fs);
    int px = 5, py = 3;
    DrawRectangle(x - tw/2 - px, y - py, tw + px*2, fs + py*2, Fade(BLACK, 0.72f));
    DrawText(text.c_str(), x - tw/2, y, fs, WHITE);
}

// ===========================================================================
int main() {
    const int SW = 1280, SH = 720;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SW, SH, "El Tesoro del Pirata - Visual Premium");
    SetTargetFPS(60);

    // Cargar texturas de fondo
    Texture2D texFondoPirata = LoadTexture("fondo_pirata.png");
    Texture2D texFondoShrek  = LoadTexture("fondo_shrek.png");
    Texture2D texIsla        = LoadTexture("nodo_isla.png");
    Texture2D texCastillo    = LoadTexture("nodo_castillo.png");
    Texture2D texTesoro      = LoadTexture("nodo_tesoro.png");

    // ---- Backend ----
    Nodo* nd[50]; int totalNodos = 0;
    for (int i = 0; i < 50; i++) nd[i] = nullptr;
    cargarGrafo("mapa_isla.txt", nd, totalNodos);
    cargarPistas("pistas.txt",   nd, totalNodos);

    DiccionarioHash dicc; dicc.cargarDesdeArchivo("diccionario_camba.txt");
    ArbolDecisiones arbol; arbol.construirArbol();
    Explorador explo;

    int padre[50];
    int destinoDijkstra = 8, nodoActual = 0;
    bool mostrarDijkstra = false;

    string mensajeSistema = "Bienvenido a Neverland. Haz clic en una isla.";
    string mensajePista   = "";

    enum Vista { MAPA, CONSOLA, INPUT_TEXTO, INPUT_DOBLE };
    Vista vistaActual = MAPA;
    string consolaOutput = "";

    string inputTitulo = "", inputText = "", inputText2 = "";
    int inputModoAccion = 0;
    bool editandoPrimerInput = true;

    // Animacion pulso del nodo activo
    float pulso = 0.0f;

    // ---------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        pulso += GetFrameTime() * 3.0f;
        float wave = (float)sin(pulso) * 5.0f;  // oscila +-5 px

        // ---- Logica INPUT ----
        if (vistaActual == INPUT_TEXTO || vistaActual == INPUT_DOBLE) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125) {
                    if (editandoPrimerInput  && (int)inputText.size()  < 30)  inputText  += (char)toupper(key);
                    if (!editandoPrimerInput && (int)inputText2.size() < 100) inputText2 += (char)key;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (editandoPrimerInput  && !inputText.empty())  inputText.pop_back();
                if (!editandoPrimerInput && !inputText2.empty()) inputText2.pop_back();
            }
            if (IsKeyPressed(KEY_TAB) && vistaActual == INPUT_DOBLE)
                editandoPrimerInput = !editandoPrimerInput;

            if (IsKeyPressed(KEY_ENTER)) {
                consolaOutput = "";
                if (inputModoAccion == 1) {
                    string sig = dicc.buscar(inputText);
                    consolaOutput = sig.empty()
                        ? "Palabra NO encontrada en el diccionario."
                        : "Significado de " + inputText + ":\n\n" + sig;
                    vistaActual = CONSOLA;
                } else if (inputModoAccion == 2) {
                    consolaOutput = dicc.eliminar(inputText)
                        ? "La palabra " + inputText + " fue eliminada."
                        : "Error: Palabra no encontrada.";
                    vistaActual = CONSOLA;
                } else if (inputModoAccion == 3 && !inputText.empty() && !inputText2.empty()) {
                    dicc.insertar(inputText, inputText2);
                    consolaOutput = "La palabra " + inputText + " fue anadida exitosamente.";
                    vistaActual = CONSOLA;
                }
            }
            if (IsKeyPressed(KEY_ESCAPE)) vistaActual = MAPA;
        }
        else if (vistaActual == MAPA) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouse.x < 995) {
                for (int i = 0; i < 50; i++) {
                    if (!nd[i]) continue;
                    float dx = mouse.x - posiciones[i].x, dy = mouse.y - posiciones[i].y;
                    if (sqrtf(dx*dx + dy*dy) <= 36.0f) {
                        nodoActual = i;
                        mostrarDijkstra = false;
                        mensajeSistema = "Viajaste a: " + fmtName(nd[i]->nombre);
                        string dp = arbol.interpretarPista(nd[i]->pista);
                        mensajePista = nd[i]->pista.empty() ? "" :
                            "Pista: " + nd[i]->pista + "  |  Destino sugerido: " + fmtName(dp);
                        break;
                    }
                }
            }
        }
        else if (vistaActual == CONSOLA) {
            if (IsKeyPressed(KEY_ESCAPE)) vistaActual = MAPA;
        }

        // =====================================================================
        BeginDrawing();
        ClearBackground(colorFondo);

        // ===== ZONA MAPA =====
        if (vistaActual == MAPA) {
            // Fondo textureado
            Texture2D& texFondo = (currentMap == 0) ? texFondoPirata : texFondoShrek;
            if (texFondo.id > 0) {
                // Escalar para cubrir los 1000x720
                float scaleX = 1000.0f / texFondo.width;
                float scaleY =  720.0f / texFondo.height;
                (void)scaleX; (void)scaleY; // solo para cubrir los 1000x720
                DrawTexturePro(texFondo,
                    {0,0,(float)texFondo.width,(float)texFondo.height},
                    {0,0,1000,720}, {0,0}, 0.0f, WHITE);
                // Overlay semi-transparente para legibilidad
                DrawRectangle(0, 0, 1000, 720, Fade(BLACK, 0.30f));
            } else {
                // Fallback degradado si no carga la textura
                if (currentMap == 0)
                    DrawRectangleGradientV(0, 0, 1000, 720, {20,100,140,255}, {15,30,45,255});
                else
                    DrawRectangleGradientV(0, 0, 1000, 720, {40,70,45,255}, {60,45,25,255});
            }

            // ---- ARISTAS: primero solo líneas ----
            for (int i = 0; i < 50; i++) {
                if (!nd[i]) continue;
                Arista* ady = nd[i]->ady;
                while (ady) {
                    int d = ady->dest;
                    if (d >= 0 && d < 50 && nd[d]) {
                        Vector2 A = {posiciones[i].x, posiciones[i].y};
                        Vector2 B = {posiciones[d].x, posiciones[d].y};
                        // Sombra
                        DrawLineEx({A.x+3, A.y+3}, {B.x+3, B.y+3}, 4.0f, Fade(BLACK, 0.4f));
                        // Linea punteada (dos colores alternados)
                        DrawLineEx(A, B, 3.0f, Fade(WHITE, 0.55f));
                    }
                    ady = ady->sgte;
                }
            }

            // ---- RUTA DIJKSTRA sobre las aristas ----
            if (mostrarDijkstra) {
                int actual = destinoDijkstra;
                while (actual >= 0 && actual < 50 && padre[actual] != -1) {
                    int p = padre[actual];
                    if (p >= 0 && p < 50) {
                        DrawLineEx({posiciones[actual].x, posiciones[actual].y},
                                   {posiciones[p].x, posiciones[p].y},
                                   9.0f, Fade(colorAcento, 0.9f));
                    }
                    actual = p;
                }
            }

            // ---- COSTOS: encima de las líneas, debajo de los nodos ----
            for (int i = 0; i < 50; i++) {
                if (!nd[i]) continue;
                Arista* ady = nd[i]->ady;
                while (ady) {
                    int d = ady->dest;
                    if (d >= 0 && d < 50 && nd[d]) {
                        bool tieneOpuesta = false;
                        Arista* op = nd[d]->ady;
                        while (op) {
                            if (op->dest == i) { tieneOpuesta = true; break; }
                            op = op->sgte;
                        }
                        if (!(tieneOpuesta && i > d)) {
                            int mx = (int)((posiciones[i].x + posiciones[d].x) / 2);
                            int my = (int)((posiciones[i].y + posiciones[d].y) / 2);
                            // Caja de costo bien legible
                            const char* costStr = TextFormat("%d", ady->costo);
                            int fs = 15;
                            int tw = MeasureText(costStr, fs);
                            DrawRectangle(mx - tw/2 - 7, my - 11, tw + 14, 22, {30, 30, 30, 220});
                            DrawRectangleLines(mx - tw/2 - 7, my - 11, tw + 14, 22, colorAcento);
                            DrawText(costStr, mx - tw/2, my - 7, fs, colorAcento);
                        }
                    }
                    ady = ady->sgte;
                }
            }

            // ---- NODOS: íconos + anillo de selección ----
            for (int i = 0; i < 50; i++) {
                if (!nd[i]) continue;

                float cx = posiciones[i].x;
                float cy = posiciones[i].y;
                bool esTesoro  = (i == destinoDijkstra);
                bool esActual  = (i == nodoActual);

                // Anillo de seleccion animado
                if (esActual) {
                    DrawCircle((int)cx, (int)cy, (int)(38 + wave), Fade(GREEN, 0.35f));
                    DrawCircleLines((int)cx, (int)cy, (int)(38 + wave), Fade(GREEN, 0.85f));
                } else if (esTesoro) {
                    DrawCircle((int)cx, (int)cy, 38, Fade(colorAcento, 0.25f));
                    DrawCircleLines((int)cx, (int)cy, 38, Fade(colorAcento, 0.80f));
                }

                // Elegir textura del nodo
                Texture2D& tex = esTesoro ? texTesoro :
                                 (currentMap == 1 ? texCastillo : texIsla);

                if (tex.id > 0) {
                    // Sombra del icono
                    DrawCircle((int)cx+4, (int)cy+5, 30, Fade(BLACK, 0.55f));
                    // Icono escalado a 60x60 centrado
                    float sz = 60.0f;
                    DrawTexturePro(tex,
                        {0,0,(float)tex.width,(float)tex.height},
                        {cx - sz/2, cy - sz/2, sz, sz},
                        {0,0}, 0.0f, WHITE);
                } else {
                    // Fallback circulo
                    Color col = esActual ? GREEN : (esTesoro ? colorAcento : LIGHTGRAY);
                    DrawCircle((int)cx+4, (int)cy+4, 32, Fade(BLACK, 0.5f));
                    DrawCircle((int)cx, (int)cy, 32, col);
                    DrawCircleLines((int)cx, (int)cy, 32, Fade(WHITE, 0.35f));
                }

                // Etiqueta del nombre debajo del nodo
                string dn = fmtName(nd[i]->nombre);
                DrawLabel((int)cx, (int)(cy + 36), dn, 13);
            }

            // ---- Panel inferior de pista ----
            DrawRectangle(0, 625, 1000, 95, Fade(BLACK, 0.88f));
            DrawLine(0, 625, 1000, 625, Fade(colorAcento, 0.7f));
            if (!mensajeSistema.empty())
                DrawText(mensajeSistema.c_str(), 18, 637, 17, LIGHTGRAY);
            if (!mensajePista.empty())
                DrawText(mensajePista.c_str(), 18, 662, 16, colorAcento);
        }
        // ===== PANTALLA CONSOLA =====
        else if (vistaActual == CONSOLA) {
            DrawRectangleGradientV(0, 0, 1000, 720, {10,10,15,255}, {20,20,30,255});
            DrawText("PANTALLA DE DATOS  (ESC = volver al mapa)", 30, 28, 20, GRAY);
            DrawLine(30, 55, 970, 55, Fade(GREEN, 0.4f));
            // Dibujar output en lineas
            int lineH = 22, y = 70;
            string line;
            istringstream ss(consolaOutput);
            while (getline(ss, line)) {
                DrawText(line.c_str(), 30, y, 18, {80, 255, 120, 255});
                y += lineH;
                if (y > 700) break;
            }
        }
        // ===== PANTALLAS INPUT =====
        else if (vistaActual == INPUT_TEXTO || vistaActual == INPUT_DOBLE) {
            DrawRectangleGradientV(0, 0, 1000, 720, {8,8,15,255}, {20,20,40,255});
            // Cartel titulo
            int tw = MeasureText(inputTitulo.c_str(), 28);
            DrawRectangle(90, 155, tw + 30, 42, Fade(colorAcento, 0.20f));
            DrawRectangleLines(90, 155, tw + 30, 42, Fade(colorAcento, 0.6f));
            DrawText(inputTitulo.c_str(), 106, 165, 28, colorAcento);

            // Caja 1
            bool f1 = editandoPrimerInput;
            Color bc1 = f1 ? Color{50,130,180,255} : Color{40,40,55,255};
            DrawRectangleRounded({90, 240, 820, 52}, 0.25f, 8, bc1);
            DrawRectangleRoundedLines({90, 240, 820, 52}, 0.25f, 8, f1 ? colorAcento : Fade(WHITE, 0.3f));
            string t1 = "Palabra: " + inputText + (f1 ? "_" : "");
            DrawText(t1.c_str(), 108, 255, 20, WHITE);

            if (vistaActual == INPUT_DOBLE) {
                bool f2 = !editandoPrimerInput;
                Color bc2 = f2 ? Color{50,130,180,255} : Color{40,40,55,255};
                DrawRectangleRounded({90, 315, 820, 52}, 0.25f, 8, bc2);
                DrawRectangleRoundedLines({90, 315, 820, 52}, 0.25f, 8, f2 ? colorAcento : Fade(WHITE, 0.3f));
                string t2 = "Significado: " + inputText2 + (f2 ? "_" : "");
                DrawText(t2.c_str(), 108, 330, 20, WHITE);
                DrawText("[TAB] cambiar campo   [ENTER] guardar   [ESC] cancelar", 90, 390, 15, GRAY);
            } else {
                DrawText("[ENTER] confirmar   [ESC] cancelar", 90, 315, 15, GRAY);
            }
        }

        // ===== HUD LATERAL =====
        DrawRectangle(1000, 0, 280, 720, colorPanel);
        DrawLine(1000, 0, 1000, 720, Fade(colorAcento, 0.5f));

        // Titulo HUD
        DrawRectangle(1000, 0, 280, 58, Fade(BLACK, 0.5f));
        DrawLine(1000, 58, 1280, 58, Fade(colorAcento, 0.6f));
        int htw = MeasureText("MENU DE COMANDOS", 20);
        DrawText("MENU DE COMANDOS", 1000 + 140 - htw/2, 18, 20, colorAcento);

        int bx = 1010, bw = 260, by = 70;

        // --- NAVEGACION ---
        DrawText("NAVEGACION", bx, by, 13, GRAY); by += 22;
        if (DrawButton({(float)bx, (float)by, (float)bw, 38}, "Ruta Dijkstra al Tesoro", colorBoton, colorBotonHover)) {
            explo.Dijkstra(nd, totalNodos, nodoActual, destinoDijkstra, padre);
            mostrarDijkstra = true;
            mensajeSistema = "Ruta optima desde " + fmtName(nd[nodoActual]->nombre) + " al objetivo";
            mensajePista   = "";
            vistaActual = MAPA;
        } by += 46;
        if (DrawButton({(float)bx, (float)by, 122, 38}, "BFS", colorBoton, colorBotonHover)) {
            stringstream buf; auto* old = cout.rdbuf(buf.rdbuf());
            explo.BFS(nd, totalNodos, 0); cout.rdbuf(old);
            consolaOutput = "=== BFS desde nodo 0 ===\n" + buf.str();
            vistaActual = CONSOLA;
        }
        if (DrawButton({(float)(bx+130), (float)by, 130, 38}, "DFS", colorBoton, colorBotonHover)) {
            stringstream buf; auto* old = cout.rdbuf(buf.rdbuf());
            explo.DFS(nd, totalNodos, 0); cout.rdbuf(old);
            consolaOutput = "=== DFS desde nodo 0 ===\n" + buf.str();
            vistaActual = CONSOLA;
        } by += 46;
        if (DrawButton({(float)bx, (float)by, (float)bw, 38}, "Ver Mapa", colorBoton, colorBotonHover))
            vistaActual = MAPA;
        by += 54;

        // --- DICCIONARIO ---
        DrawLine(bx, by, bx+bw, by, Fade(WHITE, 0.15f)); by += 10;
        DrawText("DICCIONARIO CAMBA", bx, by, 13, GRAY); by += 22;
        if (DrawButton({(float)bx, (float)by, (float)bw, 38}, "Buscar Palabra", colorBoton, colorBotonHover)) {
            vistaActual = INPUT_TEXTO; inputModoAccion = 1;
            inputText = ""; inputText2 = "";
            editandoPrimerInput = true;
            inputTitulo = "BUSCAR PALABRA EN EL DICCIONARIO";
        } by += 46;
        if (DrawButton({(float)bx, (float)by, (float)bw, 38}, "Anadir Palabra", colorBoton, colorBotonHover)) {
            vistaActual = INPUT_DOBLE; inputModoAccion = 3;
            inputText = ""; inputText2 = ""; editandoPrimerInput = true;
            inputTitulo = "AGREGAR NUEVA PALABRA AL DICCIONARIO";
        } by += 46;
        if (DrawButton({(float)bx, (float)by, (float)bw, 38}, "Eliminar Palabra", {180,50,50,255}, {210,70,70,255})) {
            vistaActual = INPUT_TEXTO; inputModoAccion = 2;
            inputText = ""; inputText2 = "";
            editandoPrimerInput = true;
            inputTitulo = "ELIMINAR PALABRA DEL DICCIONARIO";
        } by += 46;
        if (DrawButton({(float)bx, (float)by, (float)bw, 38}, "Estadisticas de Tabla", colorBoton, colorBotonHover)) {
            stringstream buf; auto* old = cout.rdbuf(buf.rdbuf());
            dicc.mostrarEstadisticas(); cout.rdbuf(old);
            consolaOutput = "=== ESTADISTICAS DE LA TABLA HASH ===\n" + buf.str();
            vistaActual = CONSOLA;
        } by += 54;

        // --- SISTEMA ---
        DrawLine(bx, by, bx+bw, by, Fade(WHITE, 0.15f)); by += 10;
        DrawText("SISTEMA", bx, by, 13, GRAY); by += 22;
        if (DrawButton({(float)bx, (float)by, (float)bw, 38}, "Guardar Ruta", colorBoton, colorBotonHover)) {
            if (mostrarDijkstra) {
                int camino[50], lon = 0, act = destinoDijkstra;
                while (act != -1) { camino[lon++] = act; act = padre[act]; }
                for (int i = 0; i < lon/2; i++) { int t = camino[i]; camino[i] = camino[lon-1-i]; camino[lon-1-i] = t; }
                stringstream buf; auto* old = cout.rdbuf(buf.rdbuf());
                guardarRuta("resultado_ruta_visual.txt", camino, lon, nd, 0);
                cout.rdbuf(old);
                mensajeSistema = "Ruta guardada en resultado_ruta_visual.txt";
            } else {
                mensajeSistema = "[!] Ejecuta Dijkstra primero.";
            }
            vistaActual = MAPA;
        } by += 54;

        // --- CAMBIAR MAPA ---
        DrawLine(bx, by, bx+bw, by, Fade(colorAcento, 0.4f)); by += 10;
        const char* lblMapa = (currentMap == 0) ? "Mapa Mundo de Shrek" : "Mapa Isla Pirata";
        if (DrawButton({(float)bx, (float)by, (float)bw, 44}, lblMapa, {140,80,30,255}, {180,110,50,255})) {
            for (int i = 0; i < 50; i++) {
                if (!nd[i]) continue;
                Arista* a = nd[i]->ady;
                while (a) { Arista* t = a; a = a->sgte; delete t; }
                delete nd[i]; nd[i] = nullptr;
            }
            totalNodos = 0;
            mostrarDijkstra = false;
            mensajePista = "";
            if (currentMap == 0) {
                currentMap = 1; posiciones = posicionesShrek;
                cargarGrafo("mapa_shrek.txt", nd, totalNodos);
                cargarPistas("pistas_shrek.txt", nd, totalNodos);
                destinoDijkstra = 1; nodoActual = 0;
                mensajeSistema = "Bienvenido al Mundo de Shrek. Haz clic en una ubicacion.";
            } else {
                currentMap = 0; posiciones = posicionesIsla;
                cargarGrafo("mapa_isla.txt", nd, totalNodos);
                cargarPistas("pistas.txt",   nd, totalNodos);
                destinoDijkstra = 8; nodoActual = 0;
                mensajeSistema = "Bienvenido a Neverland. Haz clic en una isla.";
            }
            vistaActual = MAPA;
        }

        EndDrawing();
    }

    // Liberar texturas
    UnloadTexture(texFondoPirata);
    UnloadTexture(texFondoShrek);
    UnloadTexture(texIsla);
    UnloadTexture(texCastillo);
    UnloadTexture(texTesoro);

    // Liberar grafo
    for (int i = 0; i < 50; i++) {
        if (!nd[i]) continue;
        Arista* a = nd[i]->ady;
        while (a) { Arista* t = a; a = a->sgte; delete t; }
        delete nd[i];
    }
    CloseWindow();
    return 0;
}
