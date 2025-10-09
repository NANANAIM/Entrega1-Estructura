struct Node {
	int value;
	Node *next;
};

void insertar(Node **p, int value) {
	Node *nuevoNodo = new Node;
	nuevoNodo->value = value;

	if (*p == nullptr) {
		// Lista vacia el nodo se apunta a si mismo
		nuevoNodo->next = nuevoNodo;
		*p = nuevoNodo;
	} else {
		// Insertar al inicio
		Node *temp = *p;

		// Avanzar hasta el ultimo nodo (que apunta al primero)
		while (temp->next != *p) {
			temp = temp->next;
		}

		nuevoNodo->next = *p;
		temp->next = nuevoNodo;
		*p = nuevoNodo; //mantener *p como el nuevo nodo
	}
}

void eliminar(Node **p, int value) {
	if (*p == nullptr) {
		std::cout << "La lista estC! vacC-a.\n";
		return;
	}

	Node *actual = *p;
	Node *anterior = nullptr;

	// Encontrar el nodo al que queremos eliminar
	do {
		if (actual->value == value) {
			break;
		}
		anterior = actual;
		actual = actual->next;
	} while (actual != *p);

	// Si no se encontro el valor
	if (actual->value != value) {
		std::cout << "Valor " << value << " no encontrado.\n";
		return;
	}

	// Caso 1: Solo hay un nodo en la lista
	if (actual->next == actual) {
		delete actual;
		*p = nullptr;
		std::cout << "Valor " << value << " eliminado. Lista vacC-a.\n";
		return;
	}

	// Caso 2: Eliminar el primer nodo
	if (actual == *p) {
		// Encontrar el ultimo nodo
		Node *ultimo = *p;
		while (ultimo->next != *p) {
			ultimo = ultimo->next;
		}

		ultimo->next = actual->next;
		*p = actual->next;
	}
	// Caso 3: Eliminar un nodo intermedio o final
	else {
		anterior->next = actual->next;
	}

	delete actual;
	std::cout << "Valor " << value << " eliminado.\n";
}

bool esta(Node *p, int value) {
	if (p == nullptr) {
		return false;
	}

	Node *actual = p;
	do {
		if (actual->value == value) {
			return true;
		}
		actual = actual->next;
	} while (actual != p);

	return false;
}

int contar(Node *p) {
	if (p == nullptr) {
		return 0;
	}

	int contador = 0;
	Node *actual = p;

	do {
		contador++;
		actual = actual->next;
	} while (actual != p);

	return contador;
}

void mostrarLista(Node *p) {
	if (p == nullptr) {
		std::cout << "Lista vacC-a\n";
		return;
	}

	Node *actual = p;
	std::cout << "Lista cC-clica: ";

	do {
		std::cout << actual->value;
		actual = actual->next;
		if (actual != p) {
			std::cout << " -> ";
		}
	} while (actual != p);

	std::cout << " -> [vuelve al inicio]\n";
}

void liberarLista(Node **p) {
	if (*p == nullptr) {
		return;
	}

	Node *actual = *p;
	Node *siguiente;

	// Romper el ciclo para poder liberar la memoria
	Node *primero = *p;

	do {
		siguiente = actual->next;
		delete actual;
		actual = siguiente;
	} while (actual != primero && actual != nullptr);

	*p = nullptr;
}
