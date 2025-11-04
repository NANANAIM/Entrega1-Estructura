// Domino interactivo (hasta 3 rondas), con elección manual de jugadas, pases/robos según jugadores,
// impresión de mesa tras cada turno y acumulación de puntos.

#include <iostream>
#include "DominoCommon.h"
#include "Hand.h"
#include "Boneyard.h"
#include "Board.h"
using namespace std;

void llenar_conjunto_completo(int ids[], int &cantidad) {
    cantidad = 0;
    for (int a=0;a<=6;++a) for (int b=a;b<=6;++b) ids[cantidad++] = codificarCanonica(a,b);
}

void reordenar_ids(int ids[], int n, int numeroRonda, int jugadores, int mezcla) {
    if (n <= 1) return;
    if (n > 28) n = 28; // por seguridad
    int temp[28];
    int k = (numeroRonda * 7 + jugadores * 3 + mezcla) % n; // desplazamiento por ronda, jugadores y mezcla del usuario
    for (int i = 0; i < n; ++i) temp[i] = ids[(i + k) % n];
    for (int i = 0; i < n; ++i) ids[i] = temp[i];
    // pequeña mezcla adicional determinística
    for (int i = 1; i + 1 < n; i += 3) {
        int j = n - 1 - i;
        if (j >= 0 && j < n) { int t = ids[i]; ids[i] = ids[j]; ids[j] = t; }
    }
}

void imprimir_mano_con_indices(NodoMano* h) {
    int idx=0; cout << "Mano: ";
    cout << "{";
    while (h) {
        cout << idx << ":"; imprimirFichaCanonica(h->id); if (h->next) cout << ", ";
        ++idx; h=h->next;
    }
    cout << "}" << "\n";
}

NodoMano* mano_obtener_nodo_en(NodoMano* h, int indice) {
    int i=0; while (h && i<indice) { h=h->next; ++i; } return h;
}

bool puede_jugar_alguna(NodoMano* h, int extremoIzq, int extremoDer) {
    if (extremoIzq == -1) return h != nullptr; // mesa vacia: cualquier ficha
    while (h) {
        int a,b; decodificarCanonica(h->id, a, b);
        if (a==extremoIzq || b==extremoIzq || a==extremoDer || b==extremoDer) return true;
        h = h->next;
    }
    return false;
}

bool intentar_colocar_en_lado(NodoMesa** mesa, int id, char lado) {
    int a,b; decodificarCanonica(id, a, b);
    if (lado=='L' || lado=='l') return mesa_colocar_izquierda(mesa, a,b) || mesa_colocar_izquierda(mesa, b,a);
    if (lado=='R' || lado=='r') return mesa_colocar_derecha(mesa, a,b) || mesa_colocar_derecha(mesa, b,a);
    return false;
}

int jugar_ronda(int jugadores, int &siguienteInicial, int puntajesRonda[4], int numeroRonda, int mezcla) {
    // retorna: indice del jugador que salio (>=0) o -1 si tranca
    // inicializar estructuras
    NodoMano* manos[4]; 
    for (int i=0;i<4;++i) mano_iniciar(&manos[i]);

    NodoPozo* pozo = nullptr; 
    pozo_iniciar(&pozo);

    NodoMesa* mesa = nullptr; 
    mesa_iniciar(&mesa);

    // set y reparto
    int ids[28]; 
    int cnt;
    llenar_conjunto_completo(ids, cnt);
    // Reordenar de forma determinística sin semillas
    reordenar_ids(ids, cnt, numeroRonda, jugadores, mezcla);

    int idx = 0; for (int p=0; p<jugadores; ++p) for (int k=0;k<7;++k) mano_insertar_inicio(&manos[p], ids[idx++]);
    for (; idx<cnt; ++idx) pozo_insertar_inicio(&pozo, ids[idx]);

    // determinar starter y ficha inicial: si alguien tiene [6|6], empieza con esa; si no, ficha más alta (mayor a+b)
    int inicial = 0; bool tieneDobleSeis = false; int fichaInicialId = -1; int sumaInicial = -1;
    for (int p=0; p<jugadores; ++p) {
        for (NodoMano* h = manos[p]; h; h = h->next) {
            int a,b; decodificarCanonica(h->id, a, b);
            if (a==6 && b==6) { inicial = p; tieneDobleSeis = true; fichaInicialId = h->id; break; }
            int suma = a + b;
            if (!tieneDobleSeis && suma > sumaInicial) { sumaInicial = suma; fichaInicialId = h->id; inicial = p; }
        }
        if (tieneDobleSeis) break;
    }

    // Mensaje requerido y jugada automática obligatoria
    if (tieneDobleSeis) {
        cout << "jugador " << (inicial+1) << " tiene la ficha [6|6].\n";
    } else {
        cout << "jugador " << (inicial+1) << " tiene la ficha mas alta "; imprimirFichaCanonica(fichaInicialId); cout << ".\n";
    }

    // Colocar automáticamente la ficha inicial
    int sa, sb; decodificarCanonica(fichaInicialId, sa, sb);
    mesa_colocar_izquierda(&mesa, sa, sb);
    mano_eliminar_primero(&manos[inicial], fichaInicialId);
    cout << "J" << (inicial+1) << " juega "; imprimirFichaCanonica(fichaInicialId); cout << " como primera jugada.\n";
    mesa_imprimir(mesa);

    int actual = (inicial + 1) % jugadores; int pasesSeguidos = 0;
    int ganador = -1;

    while (true) {
        cout << "------------------------------\n";
        cout << "Turno de J" << (actual+1) << "\n";
        int L, R; mesa_obtener_extremos(mesa, L, R);
        cout << "Extremos actuales: "; if (L==-1) cout << "[mesa vacia]"; else cout << "L=" << L << " R=" << R; cout << "\n";
        imprimir_mano_con_indices(manos[actual]);

        bool jugo = false;
        if (puede_jugar_alguna(manos[actual], L, R)) {
            // pedir jugada manual
            while (true) {
                cout << "Elige indice de ficha y lado (L/R). Ej: 0 L  -> ";
                int idxSel; char lado; if (!(cin >> idxSel >> lado)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
                NodoMano* node = mano_obtener_nodo_en(manos[actual], idxSel);
                if (!node) { cout << "Indice invalido.\n"; continue; }
                if (intentar_colocar_en_lado(&mesa, node->id, lado)) {
                    // quitar de mano
                    int idSel = node->id;
                    mano_eliminar_primero(&manos[actual], idSel);
                    cout << "J" << (actual+1) << " juega "; imprimirFichaCanonica(idSel); cout << ( (lado=='L'||lado=='l')?" a la izquierda\n":" a la derecha\n" );
                    jugo = true;
                    break;
                } else {
                    cout << "No encaja en ese lado. Intenta otra combinacion.\n";
                }
            }
        } else {
            // no puede jugar
            if (jugadores <= 3) {
                // robar hasta encontrar jugable o pozo vacio
                bool pudoLuegoDeRobar = false;
                while (!puede_jugar_alguna(manos[actual], L, R)) {
                    int pulled = pozo_extraer_cualquiera(&pozo);
                    if (pulled == -1) break; // pozo vacio
                    mano_insertar_inicio(&manos[actual], pulled);
                    cout << "J" << (actual+1) << " roba del pozo: "; imprimirFichaCanonica(pulled); cout << "\n";
                }
                if (puede_jugar_alguna(manos[actual], L, R)) {
                    // forzar jugada: pedir manualmente cual y lado
                    pudoLuegoDeRobar = true;
                    continue; // reintenta turno con mano actualizada (prompt de arriba)
                }
                if (!pudoLuegoDeRobar) {
                    cout << "J" << (actual+1) << " pasa.\n";
                }
            } else {
                cout << "J" << (actual+1) << " pasa.\n";
            }
        }

        // imprimir mesa tras el turno
        mesa_imprimir(mesa);

        if (jugo && manos[actual] == nullptr) { ganador = actual; cout << "Jugador " << (actual+1) << " se quedo sin fichas. Fin de ronda.\n"; break; }

        if (!jugo) {
            pasesSeguidos++;
            if (pasesSeguidos >= jugadores) { cout << "Tranca. Fin de ronda.\n"; break; }
        } else {
            pasesSeguidos = 0;
        }

        actual = (actual + 1) % jugadores;
    }

    // Puntajes de la ronda
    int minPuntos = 1000000; int minJugador = 0;
    for (int p=0;p<jugadores;++p) {
        int pts = mano_puntos(manos[p]);
        puntajesRonda[p] = pts;
        cout << "Puntaje ronda J" << (p+1) << ": " << pts << "\n";
        if (pts < minPuntos) { minPuntos = pts; minJugador = p; }
    }

    // Siguiente starter
    siguienteInicial = (ganador != -1) ? ganador : minJugador;

    // liberar memoria
    for (int p=0;p<4;++p) mano_liberar(&manos[p]);
    pozo_liberar(&pozo);
    mesa_liberar(&mesa);
    return ganador; // -1 si tranca
}

int main() {
    cout << "Domino (hasta 3 rondas)\n";
    int jugadores;
    cout << "Numero de jugadores (2-4): ";
    cin >> jugadores; 
    if (jugadores < 2) jugadores = 2; 
    if (jugadores > 4) jugadores = 4;

    int puntajesTotales[4] = {0,0,0,0};
    int roundsPlayed = 0;
    int siguienteInicial = 0; // se actualiza al final de cada ronda por jugar_ronda

    // Preguntamos un número de mezcla simple para variar entre ejecuciones (sin semillas ni librerías)
    int mezcla = 0;
    cout << "Ingresa un numero de mezcla (entero cualquiera): ";
    if (!(cin >> mezcla)) { cin.clear(); cin.ignore(10000,'\n'); mezcla = 0; }

    while (roundsPlayed < 3) {
        cout << "\n--- Ronda " << (roundsPlayed+1) << " ---\n";
    int puntajesRonda[4] = {0,0,0,0};
    int ganador = jugar_ronda(jugadores, siguienteInicial, puntajesRonda, roundsPlayed, mezcla);
    for (int p=0;p<jugadores;++p) puntajesTotales[p] += puntajesRonda[p];

    cout << "Acumulado tras ronda " << (roundsPlayed+1) << ":\n";
    for (int p=0;p<jugadores;++p) cout << "J" << (p+1) << ": " << puntajesTotales[p] << ((p==jugadores-1)?"\n":" | ");

        roundsPlayed++;
    if (roundsPlayed >= 3) break;

        // prompt para siguiente ronda
        cout << "Escribe 's' para siguiente ronda o cualquier otra tecla para terminar: ";
        char c; cin >> c; if (c!='s' && c!='S') break;
    }

    // ganador final (menor puntaje)
    int mejor = 0; for (int p=1;p<jugadores;++p) if (puntajesTotales[p] < puntajesTotales[mejor]) mejor = p;
    cout << "\nGanador final: Jugador " << (mejor+1) << " con " << puntajesTotales[mejor] << " puntos.\n";
    return 0;
}
