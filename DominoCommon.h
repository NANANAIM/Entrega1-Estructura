// Header sin include guard (por solicitud)
#include <iostream>
using namespace std;

// Utilidades para fichas de dominó y RNG simple (sin STL)

// RNG lineal simple
struct RNG { unsigned long state; };

// Codificación canónica (a<=b): id = a*7 + b
int encodeCanonical(int a, int b) {
	if (a > b) { int t = a; a = b; b = t; }
	return a * 7 + b;
}
void decodeCanonical(int id, int &a, int &b) {
	a = id / 7; b = id % 7;
}

// Codificación orientada (a y b en ese orden)
int encodeOriented(int a, int b) { return a * 7 + b; }
void decodeOriented(int id, int &a, int &b) { a = id / 7; b = id % 7; }

int tilePointsFromId(int id) { int a,b; decodeCanonical(id, a, b); return a + b; }
void printTileCanonical(int id) { int a,b; decodeCanonical(id,a,b); cout << "[" << a << "|" << b << "]"; }
void printTileOriented(int id) { int a,b; decodeOriented(id,a,b); cout << "[" << a << "|" << b << "]"; }

void rng_seed(RNG &r, unsigned long s) { r.state = (s ? s : 1u); }
unsigned long rng_next(RNG &r) { r.state = (1103515245u * r.state + 12345u); return r.state; }
unsigned long rng_range(RNG &r, unsigned long n) { return (n == 0) ? 0 : (rng_next(r) % n); }

// Fin de DominoCommon.h (sin guardas)
