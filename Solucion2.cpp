                                  //aqui va la grilla la lista

#include <iostream>
#include <cstring>

typedef unsigned char uchar; 

struct Nodo {
    uchar* clave;    
    Nodo* sig;          // Puntero al siguiente (en el mismo nivel)
    Nodo* ant;          // Puntero al anterior (lista doblemente enlazada)
    Nodo* abajo;        // Puntero al mismo nodo en el nivel de abajo
    Nodo* arriba;

    // Constructor
    Nodo(uchar* c, Nodo* abajoPtr = nullptr) {
        clave = c;
        sig = ant = arriba = nullptr;
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

    //Funcion auxiliar para el método eliminar
    Nodo* buscarNodoBase(const char* palabraBuscada) {
        if (cabezas[0] == nullptr || palabraBuscada == nullptr) {
            return nullptr;
        }

        int nivelActual = numNiveles - 1;

        while (nivelActual > 0 && cabezas[nivelActual] == nullptr) {
            nivelActual--;
        }

        Nodo* aux = cabezas[nivelActual];

        while (nivelActual >= 0 && aux != nullptr) {
            while (aux->sig != nullptr && std::strcmp((char*)aux->sig->clave, palabraBuscada) <= 0) {
                aux = aux->sig;
            }

            if (nivelActual == 0) {
                if (std::strcmp((char*)aux->clave, palabraBuscada) == 0) {
                    return aux;
                }
                return nullptr;
            }

            aux = aux->abajo;
            nivelActual--;
        }

        return nullptr;
    }
    void desconectarHorizontal(Nodo* nodo) {
        if (nodo->ant != nullptr) {
            nodo->ant->sig = nodo->sig;
        }

        if (nodo->sig != nullptr) {
            nodo->sig->ant = nodo->ant;
        }

        for (int i = 0; i < numNiveles; i++) {
            if (cabezas[i] == nodo) {
                cabezas[i] = nodo->sig;
            }
        }
        if (ultimoBase == nodo) {
            ultimoBase = nodo->ant;
        }
    }
    void limpiarNivelesSuperiores() {
        for (int i = 1; i < numNiveles; i++) {
            Nodo* aux = cabezas[i];

            while (aux != nullptr) {
                Nodo* siguiente = aux->sig;
                delete aux;
                aux = siguiente;
            }

            cabezas[i] = nullptr;
        }

        Nodo* base = cabezas[0];

        while (base != nullptr) {
            base->arriba = nullptr;
            base = base->sig;
        }

        numNiveles = 1;
    }
    void eliminarNodo(Nodo* nodo) {
        if (nodo == nullptr) return;

        Nodo* nodoArriba = nodo->arriba;

        if (nodoArriba != nullptr) {
            Nodo* limiteBloque = nullptr;

            if (nodoArriba->sig != nullptr) {
                limiteBloque = nodoArriba->sig->abajo;
            }

            Nodo* reemplazo = nodo->sig;

            if (reemplazo != nullptr && reemplazo != limiteBloque) {
                nodoArriba->clave = reemplazo->clave;
                nodoArriba->abajo = reemplazo;
                reemplazo->arriba = nodoArriba;
                nodo->arriba = nullptr;

                Nodo* superior = nodoArriba->arriba;

                while (superior != nullptr) {
                    superior->clave = nodoArriba->clave;
                    superior = superior->arriba;
                }
            } else {
                eliminarNodo(nodoArriba);
            }
        }

        desconectarHorizontal(nodo);

        if (nodo->abajo != nullptr && nodo->abajo->arriba == nodo) {
            nodo->abajo->arriba = nullptr;
        }

        if (nodo->arriba != nullptr && nodo->arriba->abajo == nodo) {
            nodo->arriba->abajo = nullptr;
        }

        delete nodo;

        while (numNiveles > 1 && cabezas[numNiveles - 1] == nullptr) {
            numNiveles--;
        }
    }
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
        Nodo* aux = cabezas[0];
        Nodo* anterior = nullptr;

        while (aux != nullptr && std::strcmp((char*)aux->clave, (char*)palabra) < 0) {
            anterior = aux;
            aux = aux->sig;
        }

        if (aux != nullptr && std::strcmp((char*)aux->clave, (char*)palabra) == 0) {
            return;
        }

        Nodo* nuevo = new Nodo(palabra);

        nuevo->sig = aux;
        nuevo->ant = anterior;

        if (anterior == nullptr) {
            cabezas[0] = nuevo;
        } else {
            anterior->sig = nuevo;
        }

        if (aux != nullptr) {
            aux->ant = nuevo;
        } else {
            ultimoBase = nuevo;
        }
    }
    void construirNivelesSuperiores() {
    int nivelActual = 0;
    while (nivelActual < capacidadCabezas - 1) {
        cabezas[nivelActual + 1] = nullptr;
        Nodo* viajero = cabezas[nivelActual];
        Nodo* anteriorArriba = nullptr;
        int contador = 0;
        int nodosEnNivelSuperior = 0;

        while (viajero != nullptr) {
            if (contador % k == 0) {
                Nodo* nuevoArriba = new Nodo(viajero->clave, viajero);
                viajero->arriba = nuevoArriba; //IMPORTANTEE
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
        if (nodosEnNivelSuperior <= k) break; // si es menor a k
        nivelActual++;
    }
}
    bool buscar(const char* palabraBuscada) {
        return buscarNodoBase(palabraBuscada) != nullptr;
    }
    long memoriaUsada() {
        long total = 0;

        total += sizeof(GrillaNiveles);
        total += capacidadCabezas * sizeof(Nodo*);

        for (int i = 0; i < numNiveles; i++) {
            Nodo* aux = cabezas[i];

            while (aux != nullptr) {
                total += sizeof(Nodo);
                aux = aux->sig;
            }
        }

        return total;
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

    bool eliminar(const char* palabraAEliminar) {
        Nodo* nodo = buscarNodoBase(palabraAEliminar);

        if (nodo == nullptr) {
            return false;
        }

        eliminarNodo(nodo);
        return true;
    }
    bool insertarOrdenado(uchar* palabra) {
            Nodo* aux = cabezas[0];
            Nodo* anterior = nullptr;

            while (aux != nullptr && std::strcmp((char*)aux->clave, (char*)palabra) < 0) {
                anterior = aux;
                aux = aux->sig;
            }

            if (aux != nullptr && std::strcmp((char*)aux->clave, (char*)palabra) == 0) {
                return false;
            }

            Nodo* nuevo = new Nodo(palabra);

            nuevo->sig = aux;
            nuevo->ant = anterior;

            if (anterior == nullptr) {
                cabezas[0] = nuevo;
            } else {
                anterior->sig = nuevo;
            }

            if (aux != nullptr) {
                aux->ant = nuevo;
            } else {
                ultimoBase = nuevo;
            }

            limpiarNivelesSuperiores();
            construirNivelesSuperiores();

            return true;
        }

};