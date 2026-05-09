                                  //aqui va la grilla la lista

#include <iostream>
#include <cstring>

typedef unsigned char uchar; 

struct Nodo {
    uchar* clave;    
    Nodo* sig;          // Puntero al siguiente (en el mismo nivel)
    Nodo* ant;          // Puntero al anterior (lista doblemente enlazada)
    Nodo* abajo;        // Puntero al mismo nodo en el nivel de abajo

    // Constructor
    Nodo(uchar* c, Nodo* abajoPtr = nullptr) {
        clave = c;
        sig = ant = nullptr;
        abajo = abajoPtr;
    }
};

class GrillaNiveles {
private:
    Nodo** cabezas; 
    Nodo* ultimoBase; // Para insertar rápido en L1
    int numNiveles;
    int k;
    int capacidadCabezas; // Para manejar el arreglo de niveles

public:
    GrillaNiveles(int factorK) {
        k = factorK;
        numNiveles = 1; // Empezamos con el nivel base
        capacidadCabezas = 20; // Suficiente para casi cualquier n
        cabezas = new Nodo*[capacidadCabezas];
        
        // Inicializar todas las cabezas en nullptr
        for(int i = 0; i < capacidadCabezas; i++) cabezas[i] = nullptr;
        
        ultimoBase = nullptr;
    }

    void insertarNivelBase(uchar* palabra) {
        Nodo* nuevo = new Nodo(palabra);
        
        if (cabezas[0] == nullptr) {
            cabezas[0] = nuevo;
            ultimoBase = nuevo;
        } else {
            // Usamos ultimoBase en lugar de recorrer toda la lista
            ultimoBase->sig = nuevo;
            nuevo->ant = ultimoBase;
            ultimoBase = nuevo;
        }
    }
    void construirNivelesSuperiores() {
        int nivelActual = 0;
        
        while (true) {
            Nodo* viajero = cabezas[nivelActual];
            Nodo* anteriorArriba = nullptr;
            int contador = 0;
            int nodosEnNivelSuperior = 0;

            while (viajero != nullptr) {
                // Cada k nodos, "promocionamos" uno al nivel de arriba
                if (contador % k == 0) {
                    Nodo* nuevoArriba = new Nodo(viajero->clave, viajero);
                    nodosEnNivelSuperior++;

                    if (cabezas[nivelActual + 1] == nullptr) {
                        cabezas[nivelActual + 1] = nuevoArriba;
                    } else {
                        anteriorArriba->sig = nuevoArriba;
                        nuevoArriba->ant = anteriorArriba;
                    }
                    anteriorArriba = nuevoArriba;
                }
                viajero = viajero->sig;
                contador++;
            }

            numNiveles++;
            // Si el nivel que acabamos de crear tiene k o menos nodos, nos detenemos.
            if (nodosEnNivelSuperior <= k) break;
            nivelActual++;
        }
    }
    bool buscar(const char* palabraBuscada) {
        // Empezamos en la cabeza del nivel más alto
        int nivelActual = numNiveles - 1;
        Nodo* aux = cabezas[nivelActual];

        while (nivelActual >= 0) {
            // Avanzar a la derecha mientras la siguiente palabra sea menor o igual a la buscada
            while (aux->sig != nullptr && std::strcmp((char*)aux->sig->clave, palabraBuscada) <= 0) {
                aux = aux->sig;
            }

            // ¿La encontramos?
            if (std::strcmp((char*)aux->clave, palabraBuscada) == 0) return true;

            // Si no, bajamos un nivel
            if (nivelActual > 0) {
                aux = aux->abajo;
            }
            nivelActual--;
        }
        return false;
    }
    ~GrillaNiveles() {
        for (int i = 0; i < numNiveles; i++) {
            Nodo* aux = cabezas[i];
            while (aux != nullptr) {
                Nodo* prox = aux->sig;
                delete aux;
                aux = prox;
            }
        }
        delete[] cabezas;
    }
};