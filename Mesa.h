#include <iostream>
using namespace std;

// Mesa: lista doble no circular con fichas ORIENTADAS (a|b en ese orden)
struct NodoMesa { 
	int id; 
	NodoMesa* prev; 
	NodoMesa* next; 
};

// Declaraciones mínimas de funciones usadas (definidas en DominoCommon.h)
int codificarOrientada(int a, int b);

void decodificarOrientada(int idFicha, int &a, int &b);

void imprimirFichaOrientada(int idFicha);

// funciones de mesa
void mesa_iniciar(NodoMesa** head) { 
    *head = nullptr; 
}

bool mesa_vacia(NodoMesa* head) {
	return head == nullptr; 
}

void mesa_obtener_extremos(NodoMesa* head, int &outL, int &outR) {
	if (!head){ 
		outL = -1; 
		outR = -1; 
		return; 
	}
	NodoMesa* left = head; 
	while (left->prev) left = left->prev;

	NodoMesa* right = head; 
	while (right->next) right = right->next;

	int la, lb, ra, rb; 
	decodificarOrientada(left->id, la, lb); 
	decodificarOrientada(right->id, ra, rb);
	outL = la; outR = rb;
}

void mesa_imprimir(NodoMesa* head) {
	if (!head) { 
		cout << "[mesa vacia]\n"; 
		return; 
	}
	NodoMesa* left = head; 
	while (left->prev) left = left->prev;
	cout << "Mesa: ";
	for (NodoMesa* cur = left; cur; cur = cur->next) { 
		imprimirFichaOrientada(cur->id); cout << " "; 
	}
	cout << "\n";
}

bool mesa_colocar_izquierda(NodoMesa** head, int a, int b) {
	if (*head == nullptr) {
		NodoMesa* n = new NodoMesa; 
		n->id = codificarOrientada(a,b); 
		n->prev = n->next = nullptr; 
		*head = n; 
		return true;
	}
	NodoMesa* left = *head; while (left->prev) left = left->prev;
	int la, lb; 
	decodificarOrientada(left->id, la, lb);
	if (b == la) {
		NodoMesa* n = new NodoMesa; 
		n->id = codificarOrientada(a,b); 
		n->prev = nullptr; 
		n->next = left; 
		left->prev = n; 
		*head = n; 
		return true;
	} else if (a == la) {
		NodoMesa* n = new NodoMesa; 
		n->id = codificarOrientada(b,a); 
		n->prev = nullptr; 
		n->next = left; 
		left->prev = n; 
		*head = n; 
		return true;
	}
	return false;
}

bool mesa_colocar_derecha(NodoMesa** head, int a, int b) {
	if (*head == nullptr) {
		NodoMesa* n = new NodoMesa; 
		n->id = codificarOrientada(a,b); 
		n->prev = n->next = nullptr; 
		*head = n; 
		return true;
	}
	NodoMesa* right = *head; 
	while (right->next) right = right->next;
	int ra, rb; 
	decodificarOrientada(right->id, ra, rb);
	if (a == rb) {
		NodoMesa* n = new NodoMesa; 
		n->id = codificarOrientada(a,b); 
		n->next = nullptr; 
		n->prev = right; 
		right->next = n; 
		return true;
	} else if (b == rb) {
		NodoMesa* n = new NodoMesa; 
		n->id = codificarOrientada(b,a); 
		n->next = nullptr; 
		n->prev = right; 
		right->next = n; 
		return true;
	}
	return false;
}

void mesa_liberar(NodoMesa** head) {
	NodoMesa* left = *head; 
	if (!left) return; 
	while (left->prev) left = left->prev;
	NodoMesa* cur = left; 
	while (cur){ 
		NodoMesa* nx = cur->next; 
		delete cur; 
		cur = nx; 
	}
	*head = nullptr;
}

