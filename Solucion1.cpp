#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

typedef unsigned char uchar;

using namespace std;

struct SolucionArreglo {
    uchar** v; // vector que contiene las palabras
    int capacidad; // tamaño total
    int cantidad; // cuántas palabras hay guardadas
    int ascii[256]; // codigo ascii, para cada letra
    float overhead = 0.1; // 10% de espacio extra

    // Constructor inicial
    SolucionArreglo(int cap_inicial, float FOverhead) {
        cantidad = 0;
        overhead = FOverhead;
        capacidad = cap_inicial;
        v = new uchar*[capacidad];

        for(int i= 0; i< capacidad; i++) {
            v[i] = nullptr;
        }
        for(int i=0; i<256; i++) {
            ascii[i]=-1;
        }
    }
    int comparar(uchar* p1, uchar* p2) {
        int i = 0;
        while (p1[i] != '\0' && p2[i] != '\0') { //aquí se comparan
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
            nuevo_v[i] = v[i];
        }
        for(int i = cantidad; i < actualizacion; i++) {
            nuevo_v[i] = nullptr;
        }
        delete[] v; // elimina arreglo
        v = nuevo_v;
        capacidad = actualizacion;
    }
    void insertar(uchar* palabra) {
        if (palabra == nullptr || palabra[0] == '\0') return;
        if (cantidad >= capacidad) Actualizar();

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
        // memmove es una función estándar en C++ para mover bloques de memoria de forma masiva y extremadamente eficiente
        if (cantidad > posicionicion) {
         memmove(&v[posicionicion + 1], &v[posicionicion], (cantidad - posicionicion) * sizeof(uchar*));
        
        }

        v[posicionicion] = copia;
        cantidad++;

        actualizarascii();
    }

    void eliminar(uchar* palabra) {
        // localiza la posicion de la palabra
        if (palabra == nullptr || palabra[0] == '\0') return;

        int posicion = -1;
        for(int j = 0; j < cantidad; j++) {
            if(comparar(v[j], palabra) == 0) {
                posicion = j;
                break;
            }
        }

        // si la palabra existe, se borra
        if(posicion != -1) {
            // liberación de memoria
            delete[] v[posicion]; 

            // dasplazamiento con memmove
            if (posicion < cantidad - 1) {
             memmove(&v[posicion], &v[posicion + 1], (cantidad - 1 - posicion) * sizeof(uchar*));
            }
            v[cantidad - 1] = nullptr;

            cantidad--;
            actualizarascii(); 
        }
    }
    
    void actualizarascii() {
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
        if (palabra == nullptr || palabra[0] == '\0') return false;
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
        if (v != nullptr){
            for(int i = 0; i < cantidad; i++) {
                if (v[i] != nullptr){
                    delete[] v[i];
                }
            }
            delete[] v;
        }
    }
};
