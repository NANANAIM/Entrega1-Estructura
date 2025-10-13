// Domino interactivo (hasta 3 rondas), con elección manual de jugadas, pases/robos según jugadores,
// impresión de mesa tras cada turno y acumulación de puntos.

#include <iostream>
#include "DominoCommon.h"
#include "Hand.h"
#include "Boneyard.h"
#include "Board.h"
using namespace std;

static void fill_full_set(int ids[], int &count) {
    count = 0;
    for (int a=0;a<=6;++a) for (int b=a;b<=6;++b) ids[count++] = encodeCanonical(a,b);
}

static void shuffle_ids(int ids[], int n, RNG &rng) {
    for (int i=n-1;i>0;--i) {
        int j = (int) (rng_range(rng, (unsigned long)(i+1)));
        int t = ids[i]; ids[i] = ids[j]; ids[j] = t;
    }
}

static void print_hand_with_indices(HandNode* h) {
    int idx=0; cout << "Mano: ";
    cout << "{";
    while (h) {
        cout << idx << ":"; printTileCanonical(h->id); if (h->next) cout << ", ";
        ++idx; h=h->next;
    }
    cout << "}" << "\n";
}

static HandNode* hand_get_node_at(HandNode* h, int index) {
    int i=0; while (h && i<index) { h=h->next; ++i; } return h;
}

static bool can_play_any(HandNode* h, int leftVal, int rightVal) {
    if (leftVal == -1) return h != nullptr; // mesa vacia: cualquier ficha
    while (h) {
        int a,b; decodeCanonical(h->id, a, b);
        if (a==leftVal || b==leftVal || a==rightVal || b==rightVal) return true;
        h = h->next;
    }
    return false;
}

static bool try_place_on_side(BoardNode** board, int id, char side) {
    int a,b; decodeCanonical(id, a, b);
    if (side=='L' || side=='l') return board_place_left(board, a,b) || board_place_left(board, b,a);
    if (side=='R' || side=='r') return board_place_right(board, a,b) || board_place_right(board, b,a);
    return false;
}

static int round_play(int players, int &nextStarter, int roundScores[4]) {
    // retorna: indice del jugador que salio (>=0) o -1 si tranca
    // inicializar estructuras
    HandNode* hands[4]; for (int i=0;i<4;++i) hand_init(&hands[i]);
    BNode* boneyard = nullptr; boneyard_init(&boneyard);
    BoardNode* board = nullptr; board_init(&board);

    // set y reparto
    int ids[28]; int cnt; fill_full_set(ids, cnt);
    RNG rng; rng_seed(rng, 987654321u); // semilla fija; se puede variar por ronda
    shuffle_ids(ids, cnt, rng);

    int idx = 0; for (int p=0; p<players; ++p) for (int k=0;k<7;++k) hand_insert_front(&hands[p], ids[idx++]);
    for (; idx<cnt; ++idx) boneyard_push_front(&boneyard, ids[idx]);

    // determinar starter y ficha inicial: si alguien tiene [6|6], empieza con esa; si no, ficha más alta (mayor a+b)
    int starter = 0; bool hasDoubleSix = false; int startTileId = -1; int startTileSum = -1;
    for (int p=0; p<players; ++p) {
        for (HandNode* h = hands[p]; h; h = h->next) {
            int a,b; decodeCanonical(h->id, a, b);
            if (a==6 && b==6) { starter = p; hasDoubleSix = true; startTileId = h->id; break; }
            int sum = a + b;
            if (!hasDoubleSix && sum > startTileSum) { startTileSum = sum; startTileId = h->id; starter = p; }
        }
        if (hasDoubleSix) break;
    }

    // Mensaje requerido y jugada automática obligatoria
    if (hasDoubleSix) {
        cout << "jugador " << (starter+1) << " tiene la ficha [6|6].\n";
    } else {
        cout << "jugador " << (starter+1) << " tiene la ficha más alta "; printTileCanonical(startTileId); cout << ".\n";
    }

    // Colocar automáticamente la ficha inicial
    int sa, sb; decodeCanonical(startTileId, sa, sb);
    board_place_left(&board, sa, sb);
    hand_remove_first(&hands[starter], startTileId);
    cout << "J" << (starter+1) << " juega "; printTileCanonical(startTileId); cout << " como primera jugada.\n";
    board_print(board);

    int current = (starter + 1) % players; int passesInRow = 0;
    int winner = -1;

    while (true) {
        cout << "------------------------------\n";
        cout << "Turno de J" << (current+1) << "\n";
        int L, R; board_get_ends(board, L, R);
        cout << "Extremos actuales: "; if (L==-1) cout << "[mesa vacia]"; else cout << "L=" << L << " R=" << R; cout << "\n";
        print_hand_with_indices(hands[current]);

        bool played = false;
        if (can_play_any(hands[current], L, R)) {
            // pedir jugada manual
            while (true) {
                cout << "Elige indice de ficha y lado (L/R). Ej: 0 L  -> ";
                int idxSel; char side; if (!(cin >> idxSel >> side)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
                HandNode* node = hand_get_node_at(hands[current], idxSel);
                if (!node) { cout << "Indice invalido.\n"; continue; }
                if (try_place_on_side(&board, node->id, side)) {
                    // quitar de mano
                    int idSel = node->id;
                    hand_remove_first(&hands[current], idSel);
                    cout << "J" << (current+1) << " juega "; printTileCanonical(idSel); cout << ( (side=='L'||side=='l')?" a la izquierda\n":" a la derecha\n" );
                    played = true;
                    break;
                } else {
                    cout << "No encaja en ese lado. Intenta otra combinacion.\n";
                }
            }
        } else {
            // no puede jugar
            if (players <= 3) {
                // robar hasta encontrar jugable o pozo vacio
                bool couldPlayAfterDraw = false;
                while (!can_play_any(hands[current], L, R)) {
                    int pulled = boneyard_pop_any(&boneyard);
                    if (pulled == -1) break; // pozo vacio
                    hand_insert_front(&hands[current], pulled);
                    cout << "J" << (current+1) << " roba del pozo: "; printTileCanonical(pulled); cout << "\n";
                }
                if (can_play_any(hands[current], L, R)) {
                    // forzar jugada: pedir manualmente cual y lado
                    couldPlayAfterDraw = true;
                    continue; // reintenta turno con mano actualizada (prompt de arriba)
                }
                if (!couldPlayAfterDraw) {
                    cout << "J" << (current+1) << " pasa.\n";
                }
            } else {
                cout << "J" << (current+1) << " pasa.\n";
            }
        }

        // imprimir mesa tras el turno
        board_print(board);

        if (played && hands[current] == nullptr) { winner = current; cout << "Jugador " << (current+1) << " se quedo sin fichas. Fin de ronda.\n"; break; }

        if (!played) {
            passesInRow++;
            if (passesInRow >= players) { cout << "Tranca. Fin de ronda.\n"; break; }
        } else {
            passesInRow = 0;
        }

        current = (current + 1) % players;
    }

    // Puntajes de la ronda
    int minPoints = 1000000; int minPlayer = 0;
    for (int p=0;p<players;++p) {
        int pts = hand_points(hands[p]);
        roundScores[p] = pts;
        cout << "Puntaje ronda J" << (p+1) << ": " << pts << "\n";
        if (pts < minPoints) { minPoints = pts; minPlayer = p; }
    }

    // Siguiente starter
    nextStarter = (winner != -1) ? winner : minPlayer;

    // liberar memoria
    for (int p=0;p<4;++p) hand_free(&hands[p]);
    boneyard_free(&boneyard);
    board_free(&board);
    return winner; // -1 si tranca
}

int main() {
    cout << "Domino (hasta 3 rondas)\n";
    int players;
    cout << "Numero de jugadores (2-4): ";
    cin >> players; if (players < 2) players = 2; if (players > 4) players = 4;

    int totalScores[4] = {0,0,0,0};
    int roundsPlayed = 0;
    int nextStarter = 0; // se actualiza al final de cada ronda por round_play

    while (roundsPlayed < 3) {
        cout << "\n--- Ronda " << (roundsPlayed+1) << " ---\n";
        int roundScores[4] = {0,0,0,0};
        int winner = round_play(players, nextStarter, roundScores);
        for (int p=0;p<players;++p) totalScores[p] += roundScores[p];

        cout << "Acumulado tras ronda " << (roundsPlayed+1) << ":\n";
        for (int p=0;p<players;++p) cout << "J" << (p+1) << ": " << totalScores[p] << ((p==players-1)?"\n":" | ");

        roundsPlayed++;
        if (roundsPlayed >= 3) break;

        // prompt para siguiente ronda
        cout << "Escribe 's' para siguiente ronda o cualquier otra tecla para terminar: ";
        char c; cin >> c; if (c!='s' && c!='S') break;
    }

    // ganador final (menor puntaje)
    int bestP = 0; for (int p=1;p<players;++p) if (totalScores[p] < totalScores[bestP]) bestP = p;
    cout << "\nGanador final: Jugador " << (bestP+1) << " con " << totalScores[bestP] << " puntos.\n";
    return 0;
}
