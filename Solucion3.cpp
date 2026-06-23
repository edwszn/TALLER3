#include <iostream>
#include <cstring>

using namespace std;

typedef unsigned char uchar;

struct NodoK {
    uchar** palabras;
    NodoK** hijos;
    int cantidad;
};

class ArbolK {
private:
    NodoK* raiz;
    int K;
    int cantidadPalabras;

    NodoK* crearNodo() {
        NodoK* nuevo = new NodoK;
        nuevo->cantidad = 0;
        nuevo->palabras = new uchar*[K];
        nuevo->hijos = new NodoK*[K + 1];

        for (int i = 0; i < K; i++) {
            nuevo->palabras[i] = nullptr;
        }

        for (int i = 0; i <= K; i++) {
            nuevo->hijos[i] = nullptr;
        }

        return nuevo;
    }

    int alturaRecursiva(NodoK* nodo) {
        if (nodo == nullptr) {
            return 0;
        }

        int mayor = 0;

        for (int i = 0; i <= K; i++) {
            int alturaHijo = alturaRecursiva(nodo->hijos[i]);
            if (alturaHijo > mayor) {
                mayor = alturaHijo;
            }
        }

        return 1 + mayor;
    }

    int compararPalabras(const uchar* palabra1, const uchar* palabra2) const {
        int i = 0;
        while (palabra1[i] != '\0' && palabra2[i] != '\0' && palabra1[i] == palabra2[i]) {
            i++;
        }
        return palabra1[i] - palabra2[i];
    }

    int obtenerLargo(const uchar* palabra) const {
        if (palabra == nullptr) return 0;

        int largo = 0;
        while (palabra[largo] != '\0') {
            largo++;
        }
        return largo;
    }

    uchar* guardarPalabra(const uchar* palabra) {
        int largo = obtenerLargo(palabra);
        uchar* copia = new uchar[largo + 1];

        for (int i = 0; i <= largo; i++) {
            copia[i] = palabra[i];
        }

        return copia;
    }

    void liberarPalabra(uchar* palabra) {
        if (palabra != nullptr) {
            delete[] palabra;
        }
    }

    int buscarPosicion(NodoK* nodo, const uchar* palabra, bool& encontrada) const {
        int izquierda = 0;
        int derecha = nodo->cantidad - 1;
        int posicion = nodo->cantidad;
        encontrada = false;

        while (izquierda <= derecha) {
            int medio = izquierda + (derecha - izquierda) / 2;
            int comp = compararPalabras(nodo->palabras[medio], palabra);

            if (comp == 0) {
                encontrada = true;
                return medio;
            }

            if (comp < 0) {
                izquierda = medio + 1;
            } else {
                posicion = medio;
                derecha = medio - 1;
            }
        }

        return posicion;
    }

    bool esHoja(NodoK* nodo) const {
        if (nodo == nullptr) return true;

        for (int i = 0; i <= K; i++) {
            if (nodo->hijos[i] != nullptr) {
                return false;
            }
        }

        return true;
    }

    void liberarNodo(NodoK* nodo) {
        if (nodo == nullptr) return;

        for (int i = 0; i <= K; i++) {
            liberarNodo(nodo->hijos[i]);
        }

        for (int i = 0; i < nodo->cantidad; i++) {
            liberarPalabra(nodo->palabras[i]);
        }

        delete[] nodo->palabras;
        delete[] nodo->hijos;
        delete nodo;
    }

    long long memoriaNodo(NodoK* nodo) const {
        if (nodo == nullptr) return 0;

        long long total = sizeof(NodoK);
        total += sizeof(uchar*) * K;
        total += sizeof(NodoK*) * (K + 1);

        for (int i = 0; i < nodo->cantidad; i++) {
            total += (obtenerLargo(nodo->palabras[i]) + 1) * sizeof(uchar);
        }

        for (int i = 0; i <= K; i++) {
            total += memoriaNodo(nodo->hijos[i]);
        }

        return total;
    }

    void separarHijo(NodoK* padre, int indice) {
        NodoK* hijo = padre->hijos[indice];
        NodoK* derecho = crearNodo();

        int medio = K / 2;
        int cantidadDerecha = K - medio - 1;

        derecho->cantidad = cantidadDerecha;

        for (int j = 0; j < cantidadDerecha; j++) {
            derecho->palabras[j] = hijo->palabras[medio + 1 + j];
            hijo->palabras[medio + 1 + j] = nullptr;
        }

        for (int j = 0; j <= cantidadDerecha; j++) {
            derecho->hijos[j] = hijo->hijos[medio + 1 + j];
            hijo->hijos[medio + 1 + j] = nullptr;
        }

        for (int j = padre->cantidad; j >= indice + 1; j--) {
            padre->hijos[j + 1] = padre->hijos[j];
        }
        padre->hijos[indice + 1] = derecho;

        for (int j = padre->cantidad - 1; j >= indice; j--) {
            padre->palabras[j + 1] = padre->palabras[j];
        }
        padre->palabras[indice] = hijo->palabras[medio];
        hijo->palabras[medio] = nullptr;

        hijo->cantidad = medio;
        padre->cantidad++;
    }

    bool insertarNoLleno(NodoK* nodo, const uchar* palabra) {
        bool encontrada = false;
        int posicion = buscarPosicion(nodo, palabra, encontrada);

        if (encontrada) {
            return false;
        }

        if (esHoja(nodo)) {
            for (int j = nodo->cantidad - 1; j >= posicion; j--) {
                nodo->palabras[j + 1] = nodo->palabras[j];
            }

            nodo->palabras[posicion] = guardarPalabra(palabra);
            nodo->cantidad++;
            cantidadPalabras++;
            return true;
        }

        if (nodo->hijos[posicion] == nullptr) {
            nodo->hijos[posicion] = crearNodo();
        }

        if (nodo->hijos[posicion]->cantidad == K) {
            separarHijo(nodo, posicion);

            int comp = compararPalabras(palabra, nodo->palabras[posicion]);

            if (comp == 0) {
                return false;
            }

            if (comp > 0) {
                posicion++;
            }
        }

        return insertarNoLleno(nodo->hijos[posicion], palabra);
    }

    uchar* obtenerMinimo(NodoK* nodo) const {
        NodoK* actual = nodo;

        while (actual != nullptr && actual->hijos[0] != nullptr) {
            actual = actual->hijos[0];
        }

        if (actual == nullptr || actual->cantidad == 0) return nullptr;
        return actual->palabras[0];
    }

    uchar* obtenerMaximo(NodoK* nodo) const {
        NodoK* actual = nodo;

        while (actual != nullptr && actual->hijos[actual->cantidad] != nullptr) {
            actual = actual->hijos[actual->cantidad];
        }

        if (actual == nullptr || actual->cantidad == 0) return nullptr;
        return actual->palabras[actual->cantidad - 1];
    }

    void borrarPalabraDeNodo(NodoK* nodo, int posicion) {
        liberarPalabra(nodo->palabras[posicion]);

        for (int i = posicion; i < nodo->cantidad - 1; i++) {
            nodo->palabras[i] = nodo->palabras[i + 1];
        }

        nodo->palabras[nodo->cantidad - 1] = nullptr;
        nodo->cantidad--;
    }

    bool eliminarRecursivo(NodoK*& nodo, const uchar* palabra) {
        if (nodo == nullptr) return false;

        bool encontrada = false;
        int posicion = buscarPosicion(nodo, palabra, encontrada);

        if (encontrada) {
            if (esHoja(nodo)) {
                borrarPalabraDeNodo(nodo, posicion);
                cantidadPalabras--;

                if (nodo->cantidad == 0) {
                    delete[] nodo->palabras;
                    delete[] nodo->hijos;
                    delete nodo;
                    nodo = nullptr;
                }

                return true;
            }

            if (nodo->hijos[posicion + 1] != nullptr) {
                uchar* sucesor = obtenerMinimo(nodo->hijos[posicion + 1]);

                if (sucesor != nullptr) {
                    liberarPalabra(nodo->palabras[posicion]);
                    nodo->palabras[posicion] = guardarPalabra(sucesor);
                    return eliminarRecursivo(nodo->hijos[posicion + 1], sucesor);
                }
            }

            if (nodo->hijos[posicion] != nullptr) {
                uchar* predecesor = obtenerMaximo(nodo->hijos[posicion]);

                if (predecesor != nullptr) {
                    liberarPalabra(nodo->palabras[posicion]);
                    nodo->palabras[posicion] = guardarPalabra(predecesor);
                    return eliminarRecursivo(nodo->hijos[posicion], predecesor);
                }
            }

            borrarPalabraDeNodo(nodo, posicion);
            cantidadPalabras--;
            return true;
        }

        return eliminarRecursivo(nodo->hijos[posicion], palabra);
    }

    int alturaRecursiva(NodoK* nodo) const {
        if (nodo == nullptr) return 0;

        int mayor = 0;
        for (int i = 0; i <= nodo->cantidad; i++) {
            int h = alturaRecursiva(nodo->hijos[i]);
            if (h > mayor) mayor = h;
        }

        return mayor + 1;
    }

public:

    int altura() {
        return alturaRecursiva(raiz);
    }
    
    ArbolK(int valorK) {
        raiz = nullptr;
        K = valorK;
        cantidadPalabras = 0;
    }

    ~ArbolK() {
        vaciarArbol();
    }

    bool insertar(uchar* palabra) {
        if (palabra == nullptr || palabra[0] == '\0') return false;

        if (raiz == nullptr) {
            raiz = crearNodo();
            raiz->palabras[0] = guardarPalabra(palabra);
            raiz->cantidad = 1;
            cantidadPalabras++;
            return true;
        }

        if (raiz->cantidad == K) {
            NodoK* nuevaRaiz = crearNodo();
            nuevaRaiz->hijos[0] = raiz;
            raiz = nuevaRaiz;
            separarHijo(raiz, 0);
        }

        return insertarNoLleno(raiz, palabra);
    }

    bool buscar(uchar* palabra) const {
        if (palabra == nullptr || palabra[0] == '\0') return false;

        NodoK* actual = raiz;

        while (actual != nullptr) {
            bool encontrada = false;
            int posicion = buscarPosicion(actual, palabra, encontrada);

            if (encontrada) {
                return true;
            }

            actual = actual->hijos[posicion];
        }

        return false;
    }

    bool eliminar(uchar* palabra) {
        if (palabra == nullptr || palabra[0] == '\0' || raiz == nullptr) return false;

        bool eliminado = eliminarRecursivo(raiz, palabra);

        if (raiz != nullptr && raiz->cantidad == 0) {
            NodoK* antiguaRaiz = raiz;
            NodoK* nuevaRaiz = nullptr;

            for (int i = 0; i <= K; i++) {
                if (antiguaRaiz->hijos[i] != nullptr) {
                    nuevaRaiz = antiguaRaiz->hijos[i];
                    antiguaRaiz->hijos[i] = nullptr;
                    break;
                }
            }

            delete[] antiguaRaiz->palabras;
            delete[] antiguaRaiz->hijos;
            delete antiguaRaiz;
            raiz = nuevaRaiz;
        }

        return eliminado;
    }

    long long calcularMemNodo() const {
        return sizeof(ArbolK) + memoriaNodo(raiz);
    }

    int obtCantidPalabras() const {
        return cantidadPalabras;
    }

    int mostrarK() const {
        return K;
    }

    bool estaVacio() const {
        return raiz == nullptr;
    }

    int altura() const {
        return alturaRecursiva(raiz);
    }

    void vaciarArbol() {
        liberarNodo(raiz);
        raiz = nullptr;
        cantidadPalabras = 0;
    }
};