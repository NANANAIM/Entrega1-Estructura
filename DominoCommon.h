#pragma once

#include <iostream>
using namespace std;

// Utilidades para fichas de dominó y RNG simple (sin STL)

// Codificación canónica (a<=b): id = a*7 + b
inline int encodeCanonical(int a, int b) {
    if (a > b) { int t = a; a = b; b = t; }
    return a * 7 + b;
}

inline void decodeCanonical(int id, int &a, int &b) {
    a = id / 7;
    b = id % 7;
}

// Codificación orientada (a y b en ese orden)
inline int encodeOriented(int a, int b) {
    return a * 7 + b;
}

inline void decodeOriented(int id, int &a, int &b) {
    a = id / 7;
    b = id % 7;
}

inline int tilePointsFromId(int id) {
    int a, b; decodeCanonical(id, a, b); return a + b;
}

inline void printTileCanonical(int id) {
    int a, b; decodeCanonical(id, a, b);
    cout << "[" << a << "|" << b << "]";
}

inline void printTileOriented(int id) {
    int a, b; decodeOriented(id, a, b);
    cout << "[" << a << "|" << b << "]";
}

// RNG lineal simple
struct RNG { unsigned long state; };

inline void rng_seed(RNG &r, unsigned long s) { r.state = (s ? s : 1u); }
inline unsigned long rng_next(RNG &r) {
    // LCG: X_{n+1} = (a*X_n + c) mod 2^32
    r.state = (1103515245u * r.state + 12345u);
    return r.state;
}
inline unsigned long rng_range(RNG &r, unsigned long n) {
    return (n == 0) ? 0 : (rng_next(r) % n);
}
