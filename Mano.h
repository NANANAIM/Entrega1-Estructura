#include <iostream>
using namespace std;

// Mano basada en lista simple: almacena fichas canónicas (id a<=b)
struct NodoMano { 
    int id; 
    NodoMano* next; 
};

// Declaraciones mínimas de funciones usadas (definidas en DominoCommon.h)
/*
Motivo: en este proyecto las implementaciones están en .h . Si incluyéramos DominoCommon.h desde varios .h , aparecerían MÚLTIPLES DEFINICIONES al enlazar 
Solución: declarar solo las firmas que necesitamos
Con esto el compilador conoce los tipos y puede generar las llamadas, y las definiciones reales las aporta otro archivo (main.cpp) que sí incluye DominoCommon.h.  
Esta explicacion tambien aplica en Mesa.h que hay declaraciones minimas
*/
int puntosDeFicha(int idFicha);

void imprimirFichaCanonica(int idFicha);

// Implementaciones de funciones de mano
void mano_iniciar(NodoMano** h){ 
    *h = nullptr; 
}

void mano_insertar_inicio(NodoMano** h, int id){
    NodoMano* n = new NodoMano;
    n->id = id; 
    n->next = *h; 
    *h = n;
}

bool mano_eliminar_primero(NodoMano** h, int id) {
        NodoMano* cur = *h; 
        NodoMano* prev = nullptr;
        while (cur) {
                if (cur->id == id) {
                        if (prev) prev->next = cur->next; 
                        else *h = cur->next;
                        delete cur; 
                        return true;
                }
                prev = cur; 
                cur = cur->next;
        }
        return false;
}

bool mano_contiene(NodoMano* h, int id) {
        while (h) { 
            if (h->id == id) return true; 
            h = h->next; 
        } 
        return false;
}

int mano_contar(NodoMano* h) { 
    int c=0; 
    while (h){
        ++c; 
        h=h->next;
    } 
    return c; 
}

int mano_puntos(NodoMano* h) { 
    int s=0; 
    while (h){ 
        s += puntosDeFicha(h->id); 
        h=h->next;
    } 
    return s; 
}

void mano_imprimir(NodoMano* h) {
        cout << "{";
        bool first=true;
        while (h) {
                if (!first) cout << ", ";
                imprimirFichaCanonica(h->id);
                first=false; 
                h=h->next;
        }
        cout << "}";
}

void mano_liberar(NodoMano** h) {
    NodoMano* cur = *h; 
    while (cur){ 
        NodoMano* nx = cur->next; 
        delete cur; 
        cur = nx; 
    } 
    *h=nullptr;
}


