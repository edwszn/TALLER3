#include <iostream>
#include <cstring>

typedef unsigned char uchar;

using namespace std;

//Definir la Struct del arbol

struct NodoK {
    uchar** palabras; // arreglo de palabras guardadas en este nodo
    NodoK** hijos; // arreglo de punteros a los hijos
    int cantidad; // cantidad de palabras usadas en este nodo
};

//------------------------------clase principal-------------------------------------
class ArbolK
{
private:
    //ATRIBUTOS
    NodoK* raiz;
    int K; //Guarda la cantidad de claves
    int cantidadPalabras;  //Un contador de palabras que hay en el arbol
      
    //********Funciones internas Basicas*********
      
    NodoK* crearNodo(){ //Funcion para crear un nuevo nodo
        NodoK* nuevo =new NodoK;
        nuevo->cantidad = 0;
        nuevo->palabras = new uchar*[K];
        for (int i =0; i <K; i++){
            nuevo->palabras[i] = nullptr;
        }
        nuevo->hijos = new NodoK*[K +1];
        for (int i =0; i <K +1; i++){
            nuevo->hijos[i] = nullptr;
        }
        return nuevo;
    }


    int obtenerLargo(uchar* palabra) {
        if (palabra == nullptr) return 0;
        int largo = 0;
        while (palabra[largo] != '\0') {
            largo++;
        }
        return largo;
      }

    void liberarNodo(NodoK* nodo){ //Para eliminar una rama completa del arbol de manera recursiva
        if (nodo == nullptr){
                return;
            }
        for (int i = 0; i <= K; i++) {
            if (nodo->hijos[i] != nullptr) {
                liberarNodo(nodo->hijos[i]);
            }
        }
        
        for (int i = 0; i < nodo->cantidad; i++) { // Corregido: antes tenías 'pal'
            liberarEspacio(nodo->palabras[i]);
       }
        delete[] nodo->palabras;
        delete[] nodo->hijos;
        delete nodo;
    }

    int calcularMemNodo(NodoK* nodo){ //Funcion para calcular la memoria ocupada por un nodo
        if (nodo == nullptr) return 0;

        int memoria = sizeof(NodoK);
        memoria += sizeof(uchar*) * K;
        memoria += sizeof(NodoK*) * (K + 1);

        for (int i = 0; i < nodo->cantidad; i++) {
            int largo = 0;
            while (nodo->palabras[i][largo] != '\0') {
                largo++;
        }
            memoria += (largo + 1) * sizeof(uchar);
        }

        for (int i = 0; i <= K; i++) {
            memoria += calcularMemNodo(nodo->hijos[i]);
        }

    return memoria;
    }

    int compararPalabras(uchar* palabra1, uchar* palabra2){ //Compara palabra1 con palabra2 y devuelve negativo si palabra1 < palabra2, 0 si son iguales, y positivo si palabra1 > palabra2
        int i =0;
        while (palabra1[i] != '\0' && palabra2[i] != '\0' && palabra1[i] == palabra2[i]){
            i++;
        }
        return palabra1[i] - palabra2[i];
    }


    uchar* guardarPalabra(uchar* palabra){
        int largo = 0;
        while (palabra[largo] != '\0') {
            largo++;
        }
        uchar* copia = new uchar[largo + 1];
    
        // Copiamos los caracteres de la palabra original a la copia
        for (int i = 0; i < largo; i++) {
            copia[i] = palabra[i];
        }
        copia[largo] = '\0'; // Agregamos el carácter nulo al final de la copia
    
        return copia;
    }
    void liberarEspacio(uchar* palabra){ //Esto es solo para que no se ocupe memoria de más
        if (palabra!= nullptr) {
            delete[] palabra;
        }
    }
    int buscarPosNodo(NodoK* nodo, uchar* palabra){ //Esto es para buscar en que posicion del nodo esta la palabra que busco
        if (nodo == nullptr) return -1;
        for (int i = 0; i < nodo->cantidad; i++) {
            if (compararPalabras(nodo->palabras[i], palabra) == 0) {
                return i;
            }
        }
        return -1;
    }

    int hijoDondeBajar(NodoK* nodo, uchar* palabra){ // Esto es para que despues de encontrar el nodo con BuscarPosNodo, saber por cual bajar
        int i = 0;
        while (i < nodo->cantidad && compararPalabras(nodo->palabras[i], palabra) < 0) {
            i++;
        }
        return i;
    }


    bool esHoja(NodoK* nodo){ // Revisa si el nodo no tiene hijos
        if (nodo == nullptr) return true;
        for (int i = 0; i <= K; i++) {
            if (nodo->hijos[i] != nullptr) {
                return false;
            }
        }
        return true;
    }

    uchar* obtenerMinimo(NodoK* nodo){ // Busca la palabra mas pequena dentro de un subarbol
        if (nodo == nullptr) return nullptr;

        NodoK* actual = nodo;
        while (actual->hijos[0] != nullptr) {
            actual = actual->hijos[0];
        }
        return actual->palabras[0];
    }

    uchar* obtenerMaximo(NodoK* nodo){ 
        if (nodo == nullptr) return nullptr;

        NodoK* actual = nodo;

        while (actual->hijos[actual->cantidad] != nullptr) {
            actual = actual->hijos[actual->cantidad];
        }

        return actual->palabras[actual->cantidad - 1];
    }

    void borrarPalabraDeNodo(NodoK* nodo, int pos){ // Borra una palabra de un nodo y corre las demas a la izquierda
        if (nodo == nullptr || pos < 0 || pos >= nodo->cantidad) return;

        liberarEspacio(nodo->palabras[pos]);

        for (int i = pos; i < nodo->cantidad - 1; i++) {
            nodo->palabras[i] = nodo->palabras[i + 1];
        }

        nodo->palabras[nodo->cantidad - 1] = nullptr;
        nodo->cantidad--;
    }


    void borrarPalabraYReacomodarHijos(NodoK* nodo, int pos){
        // Se usa cuando borramos una clave de un nodo interno.
        // Ademas de correr palabras, corre los hijos de la derecha para no perder subarboles.
        if (nodo == nullptr || pos < 0 || pos >= nodo->cantidad) return;

        liberarEspacio(nodo->palabras[pos]);

        for (int i = pos; i < nodo->cantidad - 1; i++) {
            nodo->palabras[i] = nodo->palabras[i + 1];
        }
        nodo->palabras[nodo->cantidad - 1] = nullptr;

        for (int i = pos + 1; i < K; i++) {
            nodo->hijos[i] = nodo->hijos[i + 1];
        }
        nodo->hijos[K] = nullptr;

        nodo->cantidad--;
    }

    bool eliminarRecursivo(NodoK*& nodo, uchar* palabra){
        if (nodo == nullptr) {
            return false;
        }

        int pos = buscarPosNodo(nodo, palabra);

        // Caso 1: la palabra esta en este nodo
        if (pos != -1) {

            // Caso 1A: si es hoja, se borra directamente
            if (esHoja(nodo)) {
                borrarPalabraDeNodo(nodo, pos);
                cantidadPalabras--;

                // Si el nodo quedo vacio, se elimina el nodo completo
                if (nodo->cantidad == 0) {
                    delete[] nodo->palabras;
                    delete[] nodo->hijos;
                    delete nodo;
                    nodo = nullptr;
                }
                return true;
            }

            // Caso 1B: si tiene hijo derecho, reemplazamos por el sucesor
            if (nodo->hijos[pos + 1] != nullptr) {
                uchar* sucesor = obtenerMinimo(nodo->hijos[pos + 1]);

                liberarEspacio(nodo->palabras[pos]);
                nodo->palabras[pos] = guardarPalabra(sucesor);

                return eliminarRecursivo(nodo->hijos[pos + 1], sucesor);
            }

            // Caso 1C: si no tiene hijo derecho, usamos el predecesor del hijo izquierdo
            if (nodo->hijos[pos] != nullptr) {
                uchar* predecesor = obtenerMaximo(nodo->hijos[pos]);

                liberarEspacio(nodo->palabras[pos]);
                nodo->palabras[pos] = guardarPalabra(predecesor);

                return eliminarRecursivo(nodo->hijos[pos], predecesor);
            }

            // Caso raro: no es hoja, pero no tiene hijos alrededor de esa clave.
            // Hay que mover los hijos tambien para no dejar un subarbol inaccesible.
            borrarPalabraYReacomodarHijos(nodo, pos);
            cantidadPalabras--;
            return true;
        }

        // Caso 2: la palabra no esta en este nodo, bajamos por el hijo correcto
        int hijo = hijoDondeBajar(nodo, palabra);
        return eliminarRecursivo(nodo->hijos[hijo], palabra);
    }
      
public: 
    // constructor y destructor
    ArbolK(int valorK);
    ~ArbolK();
      
    // funciones principales
    
    int calcularMemNodo() {
        return calcularMemNodo(raiz); 
    }

    bool insertar(uchar* palabra){

        if (raiz == nullptr) {
            raiz = crearNodo();
            raiz->palabras[0] = guardarPalabra(palabra);
            raiz->cantidad = 1;
            cantidadPalabras++;
            return true;
        }

        NodoK* actual = raiz;
        while (true) {
        // Si la palabra ya existe en este nodo, no se permiten duplicados
            if (buscarPosNodo(actual, palabra) != -1) {
                return false;
            }

            int i = hijoDondeBajar(actual, palabra);

            // Si el hijo por el cual debemos bajar existe, descendemos
            if (actual->hijos[i] != nullptr) {
                actual = actual->hijos[i];
            } else {
                // Si no tiene hijo y el nodo actual TIENE espacio libre (< K)
                if (actual->cantidad < K) {
                // Desplazar elementos a la derecha para mantener el orden
                    for (int j = actual->cantidad; j > i; j--) {
                        actual->palabras[j] = actual->palabras[j - 1];
                        actual->hijos[j + 1] = actual->hijos[j];
                    }
                    actual->hijos[i + 1] = actual->hijos[i];
                
                    actual->palabras[i] = guardarPalabra(palabra);
                    actual->cantidad++;
                    cantidadPalabras++;
                    return true;
                } else {
                // Si el nodo está LLENO y el hijo es nulo, creamos un nuevo nodo hijo
                    actual->hijos[i] = crearNodo();
                    actual->hijos[i]->palabras[0] = guardarPalabra(palabra);
                    actual->hijos[i]->cantidad = 1;
                    cantidadPalabras++;
                    return true;
                }
            }
        }
    }

    bool buscar(uchar* palabra){
        NodoK* actual = raiz;
        while (actual != nullptr) {
        // Buscamos si está en este nodo
            int pos = buscarPosNodo(actual, palabra);
            if (pos != -1) {
                return true; // Encontrada
            }
        // Si no está, vemos por cuál hijo descender
            int siguienteHijo = hijoDondeBajar(actual, palabra);
            actual = actual->hijos[siguienteHijo];
        }
        return false; //no existe
    }


    bool eliminar(uchar* palabra){
        if (raiz == nullptr){
            return false;
        }
        return eliminarRecursivo(raiz, palabra);
    }
    
    int obtCantidPalabras() { return cantidadPalabras; }
    int mostrarK() { return K; }
    bool estaVacio() { return raiz == nullptr; }
    
    void vaciarArbol() {
        liberarNodo(raiz);
        raiz = nullptr;
        cantidadPalabras = 0;
    }
};

// Definición externa del constructor y destructor
ArbolK::ArbolK(int valorK) {
    raiz = nullptr;
    K = valorK;
    cantidadPalabras = 0;
}

ArbolK::~ArbolK() {
    vaciarArbol();
}
