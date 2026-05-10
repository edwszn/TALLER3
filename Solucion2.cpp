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
    while (nivelActual < capacidadCabezas - 1) {
        Nodo* viajero = cabezas[nivelActual];
        Nodo* anteriorArriba = nullptr;
        int contador = 0;
        int nodosEnNivelSuperior = 0;

        while (viajero != nullptr) {
            if (contador % k == 0) {
                Nodo* nuevoArriba = new Nodo(viajero->clave, viajero);
                viajero->arriba = nuevoArriba; // CONEXIÓN VITAL
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
        if (nodosEnNivelSuperior <= 1) break; // Si solo queda 1 nodo, terminamos
        nivelActual++;
    }
}
bool buscar(const char* palabraBuscada) {
    if (cabezas[0] == nullptr) return false;

    int nivelActual = numNiveles - 1;
    // Empezar desde el primer nivel que realmente tenga un nodo
    while (nivelActual > 0 && cabezas[nivelActual] == nullptr) nivelActual--;
    
    Nodo* aux = cabezas[nivelActual];

    while (nivelActual >= 0) {
        // Avanzar a la derecha
        while (aux->sig != nullptr && std::strcmp((char*)aux->sig->clave, palabraBuscada) <= 0) {
            aux = aux->sig;
        }

        if (std::strcmp((char*)aux->clave, palabraBuscada) == 0) return true;

        if (nivelActual > 0) {
            aux = aux->abajo;
            if (aux == nullptr) return false; // Seguridad
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

    //M. de Eliminar.
    void eliminar(const char* palabraAEliminar) {
    // 1. Buscar el nodo en el nivel base (L1)
    Nodo* aux = cabezas[0];
    while (aux != nullptr) {
        if (std::strcmp((char*)aux->clave, palabraAEliminar) == 0) break;
        aux = aux->sig;
    }

    // Si no existe en el nivel base, no hay nada que borrar
    if (aux == nullptr) return;

    // 2. REGLA DEL PDF: Buscar la palabra que reemplazará la etiqueta arriba
    // Es la palabra que viene inmediatamente después en el nivel base
    const char* reemplazo = (aux->sig != nullptr) ? (char*)aux->sig->clave : nullptr;

    // 3. Subir por la "columna" de ese nodo hacia arriba
    Nodo* actual = aux;
    int nivel = 0;

    while (actual != nullptr && nivel < numNiveles) {
        // Guardamos el puntero al nodo de arriba antes de cualquier cambio
        Nodo* nodoArriba = actual->arriba;

        if (std::strcmp((char*)actual->clave, palabraAEliminar) == 0) {
            if (reemplazo != nullptr) {
                // CASO A: Hay una palabra siguiente, así que solo actualizamos la "etiqueta"
                // Esto mantiene la estructura de la grilla intacta según el PDF
                actual->clave = (uchar*)reemplazo;
            } else {
                // CASO B: No hay reemplazo (era la última palabra), hay que desconectar el nodo
                if (actual->ant != nullptr) actual->ant->sig = actual->sig;
                if (actual->sig != nullptr) actual->sig->ant = actual->ant;
                if (actual == cabezas[nivel]) cabezas[nivel] = actual->sig;

                // Solo borramos físicamente si NO es el nivel base (el base se borra al final)
                if (nivel > 0) {
                    // Antes de borrar, desconectar de abajo para no dejar punteros colgados
                    if (actual->abajo != nullptr) actual->abajo->arriba = nullptr;
                    delete actual;
                }
            }
        }
        
        actual = nodoArriba; // Subimos al siguiente piso
        nivel++;
    }

    // 4. Borrado físico del nodo en el nivel base (L1)
    if (aux->ant != nullptr) aux->ant->sig = aux->sig;
    if (aux->sig != nullptr) aux->sig->ant = aux->ant;
    if (aux == cabezas[0]) cabezas[0] = aux->sig;
    if (aux == ultimoBase) ultimoBase = aux->ant;

    delete aux;
}
void insertarOrdenado(uchar* palabra) {
    // 1. Buscamos la posición en el nivel base (L1)
    Nodo* aux = cabezas[0];
    Nodo* anterior = nullptr;

    // Avanzamos mientras la palabra actual sea menor a la que queremos insertar
    while (aux != nullptr && std::strcmp((char*)aux->clave, (char*)palabra) < 0) {
        anterior = aux;
        aux = aux->sig;
    }

    // 2. Si la palabra ya existe, no hacemos nada (evita duplicados)
    if (aux != nullptr && std::strcmp((char*)aux->clave, (char*)palabra) == 0) return;

    // 3. Insertar el nuevo nodo en la lista doblemente enlazada
    Nodo* nuevo = new Nodo(palabra);
    nuevo->sig = aux;
    nuevo->ant = anterior;

    if (anterior == nullptr) {
        cabezas[0] = nuevo; // Nueva cabeza si es la menor de todas
    } else {
        anterior->sig = nuevo;
    }

    if (aux != nullptr) {
        aux->ant = nuevo;
    } else {
        ultimoBase = nuevo; // Actualizamos el último si insertamos al final
    }
}
};