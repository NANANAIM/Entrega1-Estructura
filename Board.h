// Header sin include guard (por solicitud)

#include <iostream>
using namespace std;

// Mesa: lista doble no circular con fichas ORIENTADAS (a|b en ese orden)
struct BoardNode { int id; BoardNode* prev; BoardNode* next; };

// Declaraciones mínimas de funciones usadas y definidas en DominoCommon.h
int encodeOriented(int a, int b);
void decodeOriented(int id, int &a, int &b);
void printTileOriented(int id);

// Implementaciones en el header (sin inline/static)
void board_init(BoardNode** head) { *head = nullptr; }

bool board_empty(BoardNode* head) { return head == nullptr; }

void board_get_ends(BoardNode* head, int &outL, int &outR) {
	if (!head) { outL = -1; outR = -1; return; }
	BoardNode* left = head; while (left->prev) left = left->prev;
	BoardNode* right = head; while (right->next) right = right->next;
	int la, lb, ra, rb; decodeOriented(left->id, la, lb); decodeOriented(right->id, ra, rb);
	outL = la; outR = rb;
}

void board_print(BoardNode* head) {
	if (!head) { cout << "[mesa vacia]\n"; return; }
	BoardNode* left = head; while (left->prev) left = left->prev;
	cout << "Mesa: ";
	for (BoardNode* cur = left; cur; cur = cur->next) { printTileOriented(cur->id); cout << " "; }
	cout << "\n";
}

bool board_place_left(BoardNode** head, int a, int b) {
	if (*head == nullptr) {
		BoardNode* n = new BoardNode; n->id = encodeOriented(a,b); n->prev = n->next = nullptr; *head = n; return true;
	}
	BoardNode* left = *head; while (left->prev) left = left->prev;
	int la, lb; decodeOriented(left->id, la, lb);
	if (b == la) {
		BoardNode* n = new BoardNode; n->id = encodeOriented(a,b); n->prev = nullptr; n->next = left; left->prev = n; *head = n; return true;
	} else if (a == la) {
		BoardNode* n = new BoardNode; n->id = encodeOriented(b,a); n->prev = nullptr; n->next = left; left->prev = n; *head = n; return true;
	}
	return false;
}

bool board_place_right(BoardNode** head, int a, int b) {
	if (*head == nullptr) {
		BoardNode* n = new BoardNode; n->id = encodeOriented(a,b); n->prev = n->next = nullptr; *head = n; return true;
	}
	BoardNode* right = *head; while (right->next) right = right->next;
	int ra, rb; decodeOriented(right->id, ra, rb);
	if (a == rb) {
		BoardNode* n = new BoardNode; n->id = encodeOriented(a,b); n->next = nullptr; n->prev = right; right->next = n; return true;
	} else if (b == rb) {
		BoardNode* n = new BoardNode; n->id = encodeOriented(b,a); n->next = nullptr; n->prev = right; right->next = n; return true;
	}
	return false;
}

void board_free(BoardNode** head) {
	BoardNode* left = *head; if (!left) return; while (left->prev) left = left->prev;
	BoardNode* cur = left; while (cur){ BoardNode* nx = cur->next; delete cur; cur = nx; }
	*head = nullptr;
}

// Fin de Board.h (sin guardas)
