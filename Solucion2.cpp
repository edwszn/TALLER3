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
    void desconectarHorizontal(Nodo* nodo) {
        if (nodo->ant != nullptr) nodo->ant->sig = nodo->sig;
        if (nodo->sig != nullptr) nodo->sig->ant = nodo->ant;
        for (int i = 0; i < numNiveles; i++) {
            if (cabezas[i] == nodo) cabezas[i] = nodo->sig;
        }
    }

    void repararVertical(Nodo* nodo) {
        if (nodo->abajo != nullptr) nodo->abajo->arriba = nullptr;
        if (nodo->arriba != nullptr) nodo->arriba->abajo = nullptr;
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
            if (aux == nullptr) return false; // por Seguridad
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

    //M. de Eliminar
    void eliminar(const char* palabraAEliminar) {
    //Buscar en nivel base
    Nodo* aux = cabezas[0];
    while (aux != nullptr && std::strcmp((char*)aux->clave, palabraAEliminar) != 0) {
        aux = aux->sig;
    }

    if (aux == nullptr) return; // No se encontró

    //Recorrer la columna hacia arriba y eliminar
    Nodo* actual = aux;
    while (actual != nullptr) {
        Nodo* siguienteNivel = actual->arriba; // Guardamos el de arriba antes de borrar
        
        desconectarHorizontal(actual);
        repararVertical(actual);
        
        delete actual; // Borrado seguro
        actual = siguienteNivel; // Subimos
    }
}

void insertarOrdenado(uchar* palabra) {
    // Buscamos la posición en el nivel base (L1)
    Nodo* aux = cabezas[0];
    Nodo* anterior = nullptr;

    // Avanzamos mientras la palabra actual sea menor a la que queremos insertar
    while (aux != nullptr && std::strcmp((char*)aux->clave, (char*)palabra) < 0) {
        anterior = aux;
        aux = aux->sig;
    }

    // Si la palabra ya existe, no hacemos nada 
    if (aux != nullptr && std::strcmp((char*)aux->clave, (char*)palabra) == 0) return;

    //Insertar el nuevo nodo en la lista doblemente enlazada
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