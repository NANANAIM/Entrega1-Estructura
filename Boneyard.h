// Header sin include guard (por solicitud)

using namespace std;

// Pozo basado en lista circular simple. Nodo apunta circularmente.
struct BNode { int id; BNode* next; };

// Implementaciones en el header (sin inline/static)
void boneyard_init(BNode** p) { *p = nullptr; }

bool boneyard_empty(BNode* p) { return p == nullptr; }

void boneyard_push_front(BNode** p, int id) {
	BNode* n = new BNode; n->id = id;
	if (*p == nullptr) { n->next = n; *p = n; }
	else {
		BNode* last = *p; while (last->next != *p) last = last->next;
		n->next = *p; last->next = n; *p = n;
	}
}

int boneyard_pop_any(BNode** p) {
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

int boneyard_count(BNode* p) {
	if (!p) return 0; int c=0; BNode* cur = p; do { ++c; cur = cur->next; } while (cur != p); return c;
}

void boneyard_free(BNode** p) {
	if (*p == nullptr) return;
	BNode* start = *p; BNode* cur = start->next;
	while (cur != start) { BNode* nx = cur->next; delete cur; cur = nx; }
	delete start; *p = nullptr;
}

// Fin de Boneyard.h (sin guardas)
