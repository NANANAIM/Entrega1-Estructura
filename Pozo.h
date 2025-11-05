#include <iostream>
using namespace std;

// Pozo basado en lista circular simple. Nodo apunta circularmente.
struct NodoPozo { 
	int id; 
	NodoPozo* next; 
};


//funciones de pozo
void pozo_iniciar(NodoPozo** p){ 
	*p = nullptr; 
}

bool pozo_vacio(NodoPozo* p) { 
	return p == nullptr; 
}

void pozo_insertar_inicio(NodoPozo** p, int id) {
	NodoPozo* n = new NodoPozo; 
	n->id = id;
	if (*p == nullptr) { 
		n->next = n; 
		*p = n; 
	}
	else {
		NodoPozo* last = *p; while (last->next != *p) last = last->next;
		n->next = *p; last->next = n; *p = n;
	}
}

int pozo_extraer_cualquiera(NodoPozo** p) {
	if (*p == nullptr) return -1;
	NodoPozo* first = *p;
	int id = first->id;
	if (first->next == first) { 
		delete first; 
		*p = nullptr; 
	}
	else {
		NodoPozo* last = first; 
		while (last->next != *p) last = last->next;
		*p = first->next; 
		last->next = *p; 
		delete first;
	}
	return id;
}

int pozo_contar(NodoPozo* p) {
	if (!p) return 0; 
	int c=0; 
	NodoPozo* cur = p; 
	do { ++c; cur = cur->next; } while (cur != p); 
	return c;
}

void pozo_liberar(NodoPozo** p) {
	if (*p == nullptr) return;
	NodoPozo* start = *p; 
	NodoPozo* cur = start->next;
	while (cur != start) { 
		NodoPozo* nx = cur->next; 
		delete cur; 
		cur = nx; 
	}
	delete start; 
	*p = nullptr;
}