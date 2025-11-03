// Header sin include guard (por solicitud)

#include <iostream>
using namespace std;

// Mano basada en lista simple: almacena fichas canónicas (id a<=b)
struct HandNode { int id; HandNode* next; };

// Declaraciones mínimas de funciones usadas y definidas en DominoCommon.h
int tilePointsFromId(int id);
void printTileCanonical(int id);

// Implementaciones en el header (sin inline/static)
void hand_init(HandNode** h) { *h = nullptr; }

void hand_insert_front(HandNode** h, int id) {
        HandNode* n = new HandNode; n->id = id; n->next = *h; *h = n;
}

bool hand_remove_first(HandNode** h, int id) {
        HandNode* cur = *h; HandNode* prev = nullptr;
        while (cur) {
                if (cur->id == id) {
                        if (prev) prev->next = cur->next; else *h = cur->next;
                        delete cur; return true;
                }
                prev = cur; cur = cur->next;
        }
        return false;
}

bool hand_contains(HandNode* h, int id) {
        while (h) { if (h->id == id) return true; h = h->next; } return false;
}

int hand_count(HandNode* h) { int c=0; while (h){++c; h=h->next;} return c; }

int hand_points(HandNode* h) { int s=0; while (h){ s += tilePointsFromId(h->id); h=h->next;} return s; }

void hand_print(HandNode* h) {
        cout << "{";
        bool first=true;
        while (h) {
                if (!first) cout << ", ";
                printTileCanonical(h->id);
                first=false; h=h->next;
        }
        cout << "}";
}

void hand_free(HandNode** h) {
        HandNode* cur = *h; while (cur){ HandNode* nx = cur->next; delete cur; cur = nx; } *h=nullptr;
}

// Fin de Hand.h (sin guardas)
