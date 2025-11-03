// Include guard
#ifndef LISTA_SIMPLE_H
#define LISTA_SIMPLE_H

#include <iostream>
using namespace std;

struct Node {
    int value;
    Node *next;
};
void insertar(Node **p, int value) {
    Node *nuevoNodo = new Node;
    nuevoNodo->value = value;
    nuevoNodo->next = *p;
    *p = nuevoNodo;
}

// End of include guard
#endif // LISTA_SIMPLE_H
void eliminar(Node **p, int value) {
    Node *actual = *p;
    Node *anterior = nullptr;

    while (actual != nullptr) {
        if (actual->value == value) {
            if (anterior == nullptr) {
                *p = actual->next; // Eliminar el primer nodo
            } else {
                anterior->next = actual->next;
            }
            delete actual;
            cout << "Valor " << value << " eliminado.\n";
            return;
        }
        anterior = actual;
        actual = actual->next;
    }
    cout << "Valor " << value << " no encontrado.\n";
}

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

int contar(Node *p) {
    int contador = 0;
    Node *actual = p;
    while (actual != nullptr) {
        contador++;
        actual = actual->next;
    }
    return contador;
}
