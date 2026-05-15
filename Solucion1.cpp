 //Aqui va el areglo dinamico

#include <iostream>
#include <vector>
#include <cstdlib>

typedef unsigned char uchar;

using namespace std;

struct SolucionArreglo {
    uchar** v; // vector que contiene las palabras
    int capacidad; // Tamaño total
    int cantidad; // Cuántas palabras hay guardadas
    int ascii[256]; // codigo ascii, para cada letra
    float overhead = 0.1; // 10% de espacio extra

    void eliminar(uchar* palabra) {
        // Localizar la posicion de la palabra
        int posicion = -1;
        for(int j = 0; j < cantidad; j++) {
            if(comparar(v[j], palabra) == 0) {
                posicion = j;
                break;
            }
        }

        // Si la palabra existe, procedemo a borrar
        if(posicion != -1) {
            // LIBERACIÓN DE MEMORIA: Muy importante para la nota
            delete[] v[posicion]; 

            // DESPLAZAMIENTO (Cerrar el hueco)
            // Movemos todos los punteros desde la derecha hacia la izquierda
            for(int j = posicion; j < cantidad - 1; j++) {
                v[j] = v[j + 1];
            }

            // Actualizar contadores y el mapa de búsqueda
            cantidad--;
            actualizaracsii(); 
        }
    }

    // Constructor inicial
    SolucionArreglo(int cap_inicial, float FOverhead) {
        cantidad = 0;
        overhead = FOverhead;
        capacidad = cap_inicial;
        v = new uchar*[capacidad];

        for(int i=0; i<256; i++) {
            ascii[i]=-1;
        }
    }
    int comparar(uchar* p1, uchar* p2) {
        int i = 0;
        while (p1[i] != '\0' && p2[i] != '\0') { //aquí se comparan lexicograficamnete
            if (p1[i] < p2[i]) return -1;
            if (p1[i] > p2[i]) return 1;
            i++;
        }
        if (p1[i] == '\0' && p2[i] == '\0') return 0;
        return (p1[i] == '\0') ? -1 : 1;
    }
    // Redimensiona
    void Actualizar() {
        int actualizacion = capacidad + (capacidad * overhead) +1;
        uchar** nuevo_v = new uchar*[actualizacion];

        for(int i = 0; i < cantidad; i++) {
            nuevo_v[i] = v[i]; // copiar punteros
        }
        delete[] v; // elimina arreglo
        v = nuevo_v;
        capacidad = actualizacion;
    }
    void insertar(uchar* palabra) {
        if (cantidad == capacidad) Actualizar();

        // copia heap
        int largo = 0;
        while(palabra[largo] != '\0') {
            largo++;
        }
        uchar* copia = new uchar[largo + 1];
        for( int i = 0; i <= largo; i++) {
            copia[i] = palabra[i];
        }

        int posicionicion = 0;
        while (posicionicion < cantidad && comparar(v[posicionicion], copia) < 0) {
            posicionicion++;
        }
        for ( int i = cantidad; i > posicionicion; i--) {
            v[i] = v[i-1];
        }

        v[posicionicion] = copia;
        cantidad++;

        actualizaracsii();
    }
    void actualizaracsii() {
        for(int i = 0; i < 256; i++) {
            ascii[i] = -1;
        }
        for(int i = 0; i < cantidad; i++) {
            uchar letraInicial = v[i][0];
            if (ascii[letraInicial] == -1) {
                ascii[letraInicial] = i; // Guarda donde empiezan las letras
            }
        }
    }
    bool buscar(uchar* palabra) {
        uchar inicial = palabra[0];
        int inicio = ascii[inicial];

        if (inicio == -1) return false; // No hay palabras con esa letra

        // Determinar el fin del rango
        int fin = cantidad- 1;
        for (int i = inicial + 1; i < 256; i++) {
            if (ascii[i] != -1) {
                fin = ascii[i] - 1;
                break;
            }
        }
        // Búsqueda binaria tradicional en el rango
        while (inicio <= fin) {
            int medio = inicio + (fin - inicio) / 2;
            int comp = comparar(v[medio], palabra);
            if (comp == 0) return true;
            if (comp < 0) inicio = medio + 1;
            else fin = medio - 1;
        }
        return false;
    }

    // Destructor

    ~SolucionArreglo() {
        for(int i = 0; i < cantidad; i++) {
            delete[] v[i];
        }
        delete[] v;
    }
};
