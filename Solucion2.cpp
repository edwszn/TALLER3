#ifndef SOLUCION2_CPP
#define SOLUCION2_CPP

#include <cstddef>

typedef unsigned char uchar;

struct Nodo {
    uchar* clave;
    Nodo* sig;
    Nodo* ant;
    Nodo* abajo;
    Nodo* arriba;
    bool duenoClave;

    Nodo(uchar* c, Nodo* abajoPtr = nullptr, bool dueno = false) {
        clave = c;
        sig = nullptr;
        ant = nullptr;
        abajo = abajoPtr;
        arriba = nullptr;
        duenoClave = dueno;
    }
};

class GrillaNiveles {
private:
    Nodo** cabezas;
    Nodo* ultimoBase;
    int numNiveles;
    int k;
    int capacidadCabezas;
    int cantidadBase;
    int cambiosPendientes;
    bool baseOrdenada;

    int largoCadena(const char* palabra) const {
        if (palabra == nullptr) return 0;
        int largo = 0;
        while (palabra[largo] != '\0') largo++;
        return largo;
    }

    int comparar(const uchar* a, const char* b) const {
        if (a == nullptr && b == nullptr) return 0;
        if (a == nullptr) return -1;
        if (b == nullptr) return 1;

        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            uchar cb = (uchar)b[i];
            if (a[i] < cb) return -1;
            if (a[i] > cb) return 1;
            i++;
        }

        if (a[i] == '\0' && b[i] == '\0') return 0;
        return (a[i] == '\0') ? -1 : 1;
    }

    int compararUchar(const uchar* a, const uchar* b) const {
        if (a == nullptr && b == nullptr) return 0;
        if (a == nullptr) return -1;
        if (b == nullptr) return 1;

        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (a[i] < b[i]) return -1;
            if (a[i] > b[i]) return 1;
            i++;
        }

        if (a[i] == '\0' && b[i] == '\0') return 0;
        return (a[i] == '\0') ? -1 : 1;
    }

    uchar* copiarPalabra(const char* palabra) const {
        if (palabra == nullptr) return nullptr;

        int largo = largoCadena(palabra);
        uchar* copia = new uchar[largo + 1];

        for (int i = 0; i < largo; i++) {
            copia[i] = (uchar)palabra[i];
        }

        copia[largo] = '\0';
        return copia;
    }

    void asegurarCapacidad(int requerida) {
        if (requerida <= capacidadCabezas) return;

        int nuevaCapacidad = capacidadCabezas;
        while (nuevaCapacidad < requerida) nuevaCapacidad *= 2;

        Nodo** nuevo = new Nodo*[nuevaCapacidad];
        for (int i = 0; i < nuevaCapacidad; i++) nuevo[i] = nullptr;
        for (int i = 0; i < capacidadCabezas; i++) nuevo[i] = cabezas[i];

        delete[] cabezas;
        cabezas = nuevo;
        capacidadCabezas = nuevaCapacidad;
    }

    void liberarNivel(Nodo* cabeza, bool liberarClaves) {
        Nodo* actual = cabeza;

        while (actual != nullptr) {
            Nodo* siguiente = actual->sig;

            if (liberarClaves && actual->duenoClave && actual->clave != nullptr) {
                delete[] actual->clave;
            }

            delete actual;
            actual = siguiente;
        }
    }

    void limpiarNivelesSuperiores() {
        for (int i = 1; i < numNiveles; i++) {
            liberarNivel(cabezas[i], false);
            cabezas[i] = nullptr;
        }

        Nodo* actual = cabezas[0];
        while (actual != nullptr) {
            actual->arriba = nullptr;
            actual = actual->sig;
        }

        numNiveles = 1;
    }

    Nodo* mezclarListas(Nodo* a, Nodo* b) {
        if (a == nullptr) return b;
        if (b == nullptr) return a;

        Nodo* cabeza = nullptr;
        Nodo* cola = nullptr;

        while (a != nullptr && b != nullptr) {
            Nodo* elegido = nullptr;

            if (compararUchar(a->clave, b->clave) <= 0) {
                elegido = a;
                a = a->sig;
            } else {
                elegido = b;
                b = b->sig;
            }

            elegido->ant = cola;
            elegido->sig = nullptr;

            if (cola == nullptr) {
                cabeza = elegido;
            } else {
                cola->sig = elegido;
            }

            cola = elegido;
        }

        Nodo* resto = (a != nullptr) ? a : b;

        while (resto != nullptr) {
            Nodo* siguiente = resto->sig;
            resto->ant = cola;
            resto->sig = nullptr;

            if (cola == nullptr) {
                cabeza = resto;
            } else {
                cola->sig = resto;
            }

            cola = resto;
            resto = siguiente;
        }

        return cabeza;
    }

    Nodo* ordenarLista(Nodo* cabeza) {
        if (cabeza == nullptr || cabeza->sig == nullptr) return cabeza;

        Nodo* lento = cabeza;
        Nodo* rapido = cabeza->sig;

        while (rapido != nullptr && rapido->sig != nullptr) {
            lento = lento->sig;
            rapido = rapido->sig->sig;
        }

        Nodo* mitad = lento->sig;
        lento->sig = nullptr;
        if (mitad != nullptr) mitad->ant = nullptr;

        Nodo* izquierda = ordenarLista(cabeza);
        Nodo* derecha = ordenarLista(mitad);

        return mezclarListas(izquierda, derecha);
    }

    void repararBase() {
        Nodo* actual = cabezas[0];
        Nodo* anterior = nullptr;
        cantidadBase = 0;
        ultimoBase = nullptr;

        while (actual != nullptr) {
            actual->ant = anterior;
            actual->abajo = nullptr;
            actual->arriba = nullptr;
            actual->duenoClave = true;

            anterior = actual;
            ultimoBase = actual;
            cantidadBase++;
            actual = actual->sig;
        }
    }

    void eliminarDuplicadosOrdenados() {
        Nodo* actual = cabezas[0];

        while (actual != nullptr && actual->sig != nullptr) {
            if (compararUchar(actual->clave, actual->sig->clave) == 0) {
                Nodo* repetido = actual->sig;
                actual->sig = repetido->sig;

                if (repetido->sig != nullptr) {
                    repetido->sig->ant = actual;
                } else {
                    ultimoBase = actual;
                }

                if (repetido->duenoClave && repetido->clave != nullptr) {
                    delete[] repetido->clave;
                }

                delete repetido;
                cantidadBase--;
            } else {
                actual = actual->sig;
            }
        }
    }

    int nivelSuperiorDisponible() const {
        int nivel = numNiveles - 1;

        while (nivel > 0 && cabezas[nivel] == nullptr) {
            nivel--;
        }

        return nivel;
    }

    Nodo* buscarNodoBase(const char* palabraBuscada) const {
        if (palabraBuscada == nullptr || cabezas[0] == nullptr) return nullptr;

        int nivel = nivelSuperiorDisponible();
        Nodo* actual = nullptr;

        if (nivel > 0 && cabezas[nivel] != nullptr && comparar(cabezas[nivel]->clave, palabraBuscada) <= 0) {
            actual = cabezas[nivel];

            while (nivel > 0) {
                while (actual->sig != nullptr && comparar(actual->sig->clave, palabraBuscada) <= 0) {
                    actual = actual->sig;
                }

                actual = actual->abajo;
                nivel--;
            }
        } else {
            actual = cabezas[0];
        }

        while (actual != nullptr && comparar(actual->clave, palabraBuscada) < 0) {
            actual = actual->sig;
        }

        if (actual != nullptr && comparar(actual->clave, palabraBuscada) == 0) return actual;
        return nullptr;
    }

    Nodo* buscarPredecesorBase(const char* palabra) const {
        if (palabra == nullptr || cabezas[0] == nullptr) return nullptr;

        int nivel = nivelSuperiorDisponible();
        Nodo* actual = nullptr;
        Nodo* predecesor = nullptr;

        if (nivel > 0 && cabezas[nivel] != nullptr && comparar(cabezas[nivel]->clave, palabra) < 0) {
            actual = cabezas[nivel];

            while (nivel > 0) {
                while (actual->sig != nullptr && comparar(actual->sig->clave, palabra) < 0) {
                    actual = actual->sig;
                }

                actual = actual->abajo;
                nivel--;
            }

            if (actual != nullptr && comparar(actual->clave, palabra) < 0) {
                predecesor = actual;
                actual = actual->sig;
            }
        } else {
            actual = cabezas[0];
        }

        while (actual != nullptr && comparar(actual->clave, palabra) < 0) {
            predecesor = actual;
            actual = actual->sig;
        }

        return predecesor;
    }

    bool insertarBaseOrdenada(uchar* palabra) {
        if (palabra == nullptr || palabra[0] == '\0') {
            if (palabra != nullptr) delete[] palabra;
            return false;
        }

        if (cabezas[0] == nullptr) {
            Nodo* nuevo = new Nodo(palabra, nullptr, true);
            cabezas[0] = nuevo;
            ultimoBase = nuevo;
            cantidadBase = 1;
            baseOrdenada = true;
            cambiosPendientes++;
            return true;
        }

        if (buscarNodoBase((const char*)palabra) != nullptr) {
            delete[] palabra;
            return false;
        }

        Nodo* anterior = buscarPredecesorBase((const char*)palabra);
        Nodo* siguiente = (anterior == nullptr) ? cabezas[0] : anterior->sig;
        Nodo* nuevo = new Nodo(palabra, nullptr, true);

        nuevo->ant = anterior;
        nuevo->sig = siguiente;

        if (anterior == nullptr) {
            cabezas[0] = nuevo;
        } else {
            anterior->sig = nuevo;
        }

        if (siguiente != nullptr) {
            siguiente->ant = nuevo;
        } else {
            ultimoBase = nuevo;
        }

        cantidadBase++;
        baseOrdenada = true;
        cambiosPendientes++;
        return true;
    }

    bool debeReconstruir() const {
        if (cantidadBase <= k) return false;
        if (numNiveles <= 1) return true;

        int limite = k;
        if (limite < 256) limite = 256;
        if (limite > 4096) limite = 4096;

        return cambiosPendientes >= limite;
    }

    void desconectarBase(Nodo* nodo) {
        if (nodo == nullptr) return;

        if (nodo->ant != nullptr) {
            nodo->ant->sig = nodo->sig;
        } else {
            cabezas[0] = nodo->sig;
        }

        if (nodo->sig != nullptr) {
            nodo->sig->ant = nodo->ant;
        } else {
            ultimoBase = nodo->ant;
        }
    }

    void desconectarSuperior(Nodo* nodo) {
        if (nodo == nullptr) return;

        if (nodo->ant != nullptr) nodo->ant->sig = nodo->sig;
        if (nodo->sig != nullptr) nodo->sig->ant = nodo->ant;

        for (int i = 1; i < numNiveles; i++) {
            if (cabezas[i] == nodo) {
                cabezas[i] = nodo->sig;
                break;
            }
        }
    }

    void reducirNivelesVacios() {
        while (numNiveles > 1 && cabezas[numNiveles - 1] == nullptr) {
            numNiveles--;
        }
    }

    void reemplazarTorre(Nodo* baseEliminada, Nodo* reemplazo) {
        if (baseEliminada == nullptr || reemplazo == nullptr) return;
        if (baseEliminada->arriba == nullptr) return;
        if (reemplazo->arriba != nullptr) return;

        Nodo* superior = baseEliminada->arriba;
        Nodo* inferior = reemplazo;

        while (superior != nullptr) {
            Nodo* siguienteSuperior = superior->arriba;

            superior->clave = inferior->clave;
            superior->abajo = inferior;
            inferior->arriba = superior;

            inferior = superior;
            superior = siguienteSuperior;
        }

        baseEliminada->arriba = nullptr;
    }

    void eliminarTorre(Nodo* superior) {
        while (superior != nullptr) {
            Nodo* siguienteSuperior = superior->arriba;

            if (superior->abajo != nullptr && superior->abajo->arriba == superior) {
                superior->abajo->arriba = nullptr;
            }

            desconectarSuperior(superior);
            delete superior;
            superior = siguienteSuperior;
        }

        reducirNivelesVacios();
    }

public:
    GrillaNiveles(int factorK) {
        k = (factorK < 2) ? 2 : factorK;
        numNiveles = 1;
        capacidadCabezas = 20;
        cantidadBase = 0;
        cambiosPendientes = 0;
        baseOrdenada = true;
        ultimoBase = nullptr;

        cabezas = new Nodo*[capacidadCabezas];
        for (int i = 0; i < capacidadCabezas; i++) {
            cabezas[i] = nullptr;
        }
    }

    ~GrillaNiveles() {
        limpiarNivelesSuperiores();
        liberarNivel(cabezas[0], true);
        cabezas[0] = nullptr;
        delete[] cabezas;
        cabezas = nullptr;
        ultimoBase = nullptr;
        cantidadBase = 0;
        numNiveles = 0;
    }

    void insertarNivelBase(uchar* palabra) {
        if (palabra == nullptr || palabra[0] == '\0') {
            if (palabra != nullptr) delete[] palabra;
            return;
        }

        Nodo* nuevo = new Nodo(palabra, nullptr, true);

        if (cabezas[0] == nullptr) {
            cabezas[0] = nuevo;
            ultimoBase = nuevo;
            cantidadBase = 1;
            baseOrdenada = true;
            return;
        }

        if (compararUchar(ultimoBase->clave, palabra) > 0) {
            baseOrdenada = false;
        }

        nuevo->ant = ultimoBase;
        ultimoBase->sig = nuevo;
        ultimoBase = nuevo;
        cantidadBase++;
    }

    void insertarNivelBase(const char* palabra) {
        insertarNivelBase(copiarPalabra(palabra));
    }

    void construirNivelesSuperiores() {
        limpiarNivelesSuperiores();

        if (cabezas[0] == nullptr) {
            cambiosPendientes = 0;
            return;
        }

        if (!baseOrdenada) {
            cabezas[0] = ordenarLista(cabezas[0]);
        }

        repararBase();
        eliminarDuplicadosOrdenados();
        baseOrdenada = true;

        int nivelActual = 0;
        int cantidadNivelActual = cantidadBase;

        while (cantidadNivelActual > k) {
            asegurarCapacidad(nivelActual + 2);
            cabezas[nivelActual + 1] = nullptr;

            Nodo* viajero = cabezas[nivelActual];
            Nodo* anteriorArriba = nullptr;
            int contador = 0;
            int nodosCreados = 0;

            while (viajero != nullptr) {
                if (contador % k == 0) {
                    Nodo* nuevoArriba = new Nodo(viajero->clave, viajero, false);
                    viajero->arriba = nuevoArriba;

                    if (anteriorArriba == nullptr) {
                        cabezas[nivelActual + 1] = nuevoArriba;
                    } else {
                        anteriorArriba->sig = nuevoArriba;
                        nuevoArriba->ant = anteriorArriba;
                    }

                    anteriorArriba = nuevoArriba;
                    nodosCreados++;
                }

                viajero = viajero->sig;
                contador++;
            }

            if (nodosCreados == 0) break;

            numNiveles = nivelActual + 2;
            cantidadNivelActual = nodosCreados;
            nivelActual++;
        }

        cambiosPendientes = 0;
    }

    bool insertarOrdenado(uchar* palabra) {
        bool insertada = insertarBaseOrdenada(palabra);

        if (insertada && debeReconstruir()) {
            construirNivelesSuperiores();
        }

        return insertada;
    }

    bool insertarOrdenado(const char* palabra) {
        return insertarOrdenado(copiarPalabra(palabra));
    }

    bool buscar(const char* palabraBuscada) const {
        return buscarNodoBase(palabraBuscada) != nullptr;
    }

    bool buscar(uchar* palabraBuscada) const {
        return buscarNodoBase((const char*)palabraBuscada) != nullptr;
    }

    bool eliminar(const char* palabraAEliminar) {
        Nodo* nodo = buscarNodoBase(palabraAEliminar);
        if (nodo == nullptr) return false;

        Nodo* superior = nodo->arriba;
        Nodo* reemplazo = nullptr;

        if (nodo->sig != nullptr && nodo->sig->arriba == nullptr) {
            reemplazo = nodo->sig;
        } else if (nodo->ant != nullptr && nodo->ant->arriba == nullptr) {
            reemplazo = nodo->ant;
        }

        if (superior != nullptr && reemplazo != nullptr) {
            reemplazarTorre(nodo, reemplazo);
        } else if (superior != nullptr) {
            eliminarTorre(superior);
        }

        desconectarBase(nodo);

        if (nodo->duenoClave && nodo->clave != nullptr) {
            delete[] nodo->clave;
        }

        delete nodo;
        cantidadBase--;
        cambiosPendientes++;

        if (cantidadBase == 0) {
            limpiarNivelesSuperiores();
            ultimoBase = nullptr;
            cambiosPendientes = 0;
            return true;
        }

        if (debeReconstruir()) {
            construirNivelesSuperiores();
        }

        return true;
    }

    bool eliminar(uchar* palabraAEliminar) {
        return eliminar((const char*)palabraAEliminar);
    }

    long memoriaUsada() const {
        long total = 0;
        total += (long)sizeof(GrillaNiveles);
        total += (long)capacidadCabezas * (long)sizeof(Nodo*);

        for (int i = 0; i < numNiveles; i++) {
            Nodo* actual = cabezas[i];

            while (actual != nullptr) {
                total += (long)sizeof(Nodo);

                if (i == 0 && actual->clave != nullptr) {
                    total += (long)(largoCadena((const char*)actual->clave) + 1) * (long)sizeof(uchar);
                }

                actual = actual->sig;
            }
        }

        return total;
    }

    int cantidadPalabras() const {
        return cantidadBase;
    }

    int cantidadNiveles() const {
        return numNiveles;
    }
};

#endif