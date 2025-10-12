#pragma once

#include <iostream>
#include "DominoCommon.h"
using namespace std;

// Mesa: lista doble no circular con fichas ORIENTADAS (a|b en ese orden)
struct BoardNode { int id; BoardNode* prev; BoardNode* next; };

inline void board_init(BoardNode** head) { *head = nullptr; }

inline bool board_empty(BoardNode* head) { return head == nullptr; }

// Retorna extremos actuales (a la izquierda y derecha) en outL y outR; -1 si vacio
inline void board_get_ends(BoardNode* head, int &outL, int &outR) {
    if (!head) { outL = -1; outR = -1; return; }
    BoardNode* left = head; while (left->prev) left = left->prev;
    BoardNode* right = head; while (right->next) right = right->next;
    int la, lb, ra, rb; decodeOriented(left->id, la, lb); decodeOriented(right->id, ra, rb);
    outL = la; outR = rb;
}

inline void board_print(BoardNode* head) {
    if (!head) { cout << "[mesa vacia]\n"; return; }
    BoardNode* left = head; while (left->prev) left = left->prev;
    cout << "Mesa: ";
    for (BoardNode* cur = left; cur; cur = cur->next) { printTileOriented(cur->id); cout << " "; }
    cout << "\n";
}

// Coloca ficha al extremo izquierdo si encaja. Devuelve true si se coloco.
inline bool board_place_left(BoardNode** head, int a, int b) {
    // Queremos que el lado derecho del nuevo nodo coincida con extremo izquierdo actual
    if (*head == nullptr) {
        BoardNode* n = new BoardNode; n->id = encodeOriented(a,b); n->prev = n->next = nullptr; *head = n; return true;
    }
    BoardNode* left = *head; while (left->prev) left = left->prev;
    int la, lb; decodeOriented(left->id, la, lb);
    // extremo izquierdo visible es la
    // Necesitamos b == la si ponemos (a|b), o a == la si giramos (b|a)
    if (b == la) {
        BoardNode* n = new BoardNode; n->id = encodeOriented(a,b); n->prev = nullptr; n->next = left; left->prev = n; *head = n; return true;
    } else if (a == la) {
        BoardNode* n = new BoardNode; n->id = encodeOriented(b,a); n->prev = nullptr; n->next = left; left->prev = n; *head = n; return true;
    }
    return false;
}

// Coloca ficha al extremo derecho si encaja. Devuelve true si se coloco.
inline bool board_place_right(BoardNode** head, int a, int b) {
    if (*head == nullptr) {
        BoardNode* n = new BoardNode; n->id = encodeOriented(a,b); n->prev = n->next = nullptr; *head = n; return true;
    }
    BoardNode* right = *head; while (right->next) right = right->next;
    int ra, rb; decodeOriented(right->id, ra, rb);
    // extremo derecho visible es rb
    if (a == rb) {
        BoardNode* n = new BoardNode; n->id = encodeOriented(a,b); n->next = nullptr; n->prev = right; right->next = n; return true;
    } else if (b == rb) {
        BoardNode* n = new BoardNode; n->id = encodeOriented(b,a); n->next = nullptr; n->prev = right; right->next = n; return true;
    }
    return false;
}

inline void board_free(BoardNode** head) {
    BoardNode* left = *head; if (!left) return; while (left->prev) left = left->prev;
    BoardNode* cur = left; while (cur){ BoardNode* nx = cur->next; delete cur; cur = nx; }
    *head = nullptr;
}
