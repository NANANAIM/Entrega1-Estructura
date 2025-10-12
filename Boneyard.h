#pragma once

#include <iostream>
#include "DominoCommon.h"
using namespace std;

// Pozo basado en lista circular simple. Nodo apunta circularmente.
struct BNode { int id; BNode* next; };

// Representamos el pozo por un puntero al primero (puede ser cualquiera en circular)
inline void boneyard_init(BNode** p) { *p = nullptr; }

inline bool boneyard_empty(BNode* p) { return p == nullptr; }

inline void boneyard_push_front(BNode** p, int id) {
    BNode* n = new BNode; n->id = id;
    if (*p == nullptr) { n->next = n; *p = n; }
    else {
        BNode* last = *p; while (last->next != *p) last = last->next;
        n->next = *p; last->next = n; *p = n;
    }
}

// Extrae y devuelve un id cualquiera (el primero). Retorna -1 si vacio.
inline int boneyard_pop_any(BNode** p) {
    if (*p == nullptr) return -1;
    BNode* first = *p;
    int id = first->id;
    if (first->next == first) { delete first; *p = nullptr; }
    else {
        BNode* last = first; while (last->next != *p) last = last->next;
        *p = first->next; last->next = *p; delete first;
    }
    return id;
}

inline int boneyard_count(BNode* p) {
    if (!p) return 0; int c=0; BNode* cur = p; do { ++c; cur = cur->next; } while (cur != p); return c;
}

inline void boneyard_free(BNode** p) {
    if (*p == nullptr) return;
    BNode* start = *p; BNode* cur = start->next;
    while (cur != start) { BNode* nx = cur->next; delete cur; cur = nx; }
    delete start; *p = nullptr;
}
