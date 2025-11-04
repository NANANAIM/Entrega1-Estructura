#include <iostream>
using namespace std;

// Utilidades para fichas de dominó y RNG simple (sin STL)

// (Generador y semillas eliminados por simplicidad)

// Codificación canónica (a<=b): id = a*7 + b
int codificarCanonica(int a, int b) {
	if (a > b) { int t = a; a = b; b = t; }
	return a * 7 + b;
}

void decodificarCanonica(int idFicha, int &a, int &b) {
	a = idFicha / 7; b = idFicha % 7;
}

// Codificación orientada (a y b en ese orden)
int codificarOrientada(int a, int b) { 
	return a * 7 + b; 
}

void decodificarOrientada(int idFicha, int &a, int &b) { 
	a = idFicha / 7; 
	b = idFicha % 7; 
}

int puntosDeFicha(int idFicha) { 
	int a,b; 
	decodificarCanonica(idFicha, a, b); 
	return a + b; 
}

void imprimirFichaCanonica(int idFicha) { 
	int a,b; 
	decodificarCanonica(idFicha,a,b); 
	cout << "[" << a << "|" << b << "]"; }

void imprimirFichaOrientada(int idFicha) { 
	int a,b; 
	decodificarOrientada(idFicha,a,b); 
	cout << "[" << a << "|" << b << "]"; }

// (Funciones de semillas y pseudoaleatorio eliminadas)

