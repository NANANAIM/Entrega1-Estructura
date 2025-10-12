#include <iostream>
using namespace std;

struct Node {
    int value;
    Node *prev;
    Node *next;
};

// Inserta al inicio de la lista doble
void insertar(Node **p, int value) {
    Node *nuevo = new Node;
    nuevo->value = value;
    nuevo->prev = nullptr;
    nuevo->next = *p;

    if (*p != nullptr) {
        (*p)->prev = nuevo;
    }
    *p = nuevo;
}

// Elimina la primera aparicion de value
void eliminar(Node **p, int value) {
    Node *actual = *p;

    while (actual != nullptr) {
        if (actual->value == value) {
            // Reenlazar
            if (actual->prev != nullptr) {
                actual->prev->next = actual->next;
            } else {
                // actual es cabeza
                *p = actual->next;
            }
            if (actual->next != nullptr) {
                actual->next->prev = actual->prev;
            }

            delete actual;
            cout << "Valor " << value << " eliminado.\n";
            return;
        }
        actual = actual->next;
    }

    cout << "Valor " << value << " no encontrado.\n";
}

// Indica si value esta en la lista
bool esta(Node *p, int value) {
    Node *actual = p;
    while (actual != nullptr) {
        if (actual->value == value) {
            return true;
        }
        actual = actual->next;
    }
    return false;
}

// Cuenta los nodos de la lista
int contar(Node *p) {
    int contador = 0;
    Node *actual = p;
    while (actual != nullptr) {
        contador++;
        actual = actual->next;
    }
    return contador;
}
