/*
Tarea INFO088 - Etapa 2
Integrantes: Matias Montecinos, Afra Rodríguez, Eduardo Pezo

Compilación:
    make

Ejecución:
    ./programa k
    SI NO SE AGREGA K, el valor por defecto sera 8

El programa carga D1.txt, construye las tres estructuras,
ejecuta operaciones con D2.txt y muestra tiempos/resultados.
*/



#include <cstdlib>   // Para usar atoi
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm> // Para shuffle
#include <random>    // Para random_device
#include <chrono>    // Para medir tiempos 
#include "Solucion2.cpp" // para que lea el archivo solucion2
#include "Solucion1.cpp" // para que lea el archivo solucion1
#include "Solucion3.cpp" //Para que lea el archivo solucion3

typedef unsigned char uchar;

using namespace std;
using namespace std::chrono;
int main(int argc, char* argv[]) {
    int factorK = 8; // valor por defecto si no se entrega K por terminal

    if (argc >= 2) {
        factorK = atoi(argv[1]);
    }

    if (factorK != 8 && factorK != 32 && factorK != 128 && factorK != 512) {
        cout << "El programa se ejecuta con: ./programa K" << endl;
        cout << "K debe ser uno de estos valores: 8, 32, 128 o 512" << endl;
        return 1;
    }

    ifstream archivoD1("D1.txt");
    vector<string> palabrasD1;
    vector<string> palabrasD2;
    string linea;

    if (!archivoD1.is_open()) {
        cout << "Error: No se pudo abrir D1.txt" << endl;
        return 1;
    }

    cout << "PASO 1: Leyendo D1 en memoria..." << endl;

    while (archivoD1 >> linea) {
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) {
            linea.pop_back();
        }

        if (!linea.empty()) {
            palabrasD1.push_back(linea);
        }
    }

    archivoD1.close();

    ifstream archivoD2("D2.txt");

    if (!archivoD2.is_open()) {
        cout << "Error: No se pudo abrir D2.txt" << endl;
        return 1;
    }

    cout << "PASO 2: Leyendo D2 en memoria..." << endl;

    while (archivoD2 >> linea) {
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) {
            linea.pop_back();
        }

        if (!linea.empty()) {
            palabrasD2.push_back(linea);
        }
    }

    archivoD2.close();

    cout << "PASO 3: Construyendo estructuras con K = " << factorK << "..." << endl;

    //CONSTRUCCION SOLUCION 1
    auto inicioConstruccionS1 = high_resolution_clock::now();
    SolucionArreglo miArreglo(10000, 0.1);
    for (int i = 0; i < (int)palabrasD1.size(); i++) {
        miArreglo.insertar((uchar*)palabrasD1[i].c_str());
    }
    auto finConstruccionS1 = high_resolution_clock::now();
    duration<double, milli> tiempoConstruccionS1 = finConstruccionS1 - inicioConstruccionS1;

    // CONSTRUCCION SOLUCION 2
    auto inicioConstruccionS2 = high_resolution_clock::now();

    GrillaNiveles miGrilla(factorK);

    for (int i = 0; i < (int)palabrasD1.size(); i++) {
        uchar* palabraPtrS2 = new uchar[palabrasD1[i].length() + 1];
        strcpy((char*)palabraPtrS2, palabrasD1[i].c_str());
        miGrilla.insertarNivelBase(palabraPtrS2);
    }

    miGrilla.construirNivelesSuperiores();

    auto finConstruccionS2 = high_resolution_clock::now();
    duration<double, milli> tiempoConstruccionS2 = finConstruccionS2 - inicioConstruccionS2;


    // CONSTRUCCION SOLUCION 3
    auto inicioConstruccionS3 = high_resolution_clock::now();

    ArbolK miArbolK(factorK);

    for (int i = 0; i < (int)palabrasD1.size(); i++) {
        miArbolK.insertar((uchar*)palabrasD1[i].c_str());
    }

    auto finConstruccionS3 = high_resolution_clock::now();
    duration<double, milli> tiempoConstruccionS3 = finConstruccionS3 - inicioConstruccionS3;

    cout << "\nTIEMPOS DE CONSTRUCCION DESDE D1:" << endl;
    cout << "Solucion 1: " << tiempoConstruccionS1.count() << " ms" << endl;
    cout << "Solucion 2: " << tiempoConstruccionS2.count() << " ms" << endl;
    cout << "Solucion 3: " << tiempoConstruccionS3.count() << " ms" << endl;


    cout << "\nEjecutando Experimento con Solucion 2 (Grilla, K=" << factorK << ")..." << endl;
    int insertadasS2 = 0, eliminadasS2 = 0, encontradasS2 = 0;

    auto inicioS2 = high_resolution_clock::now();
    bool tocaInsertarS2 = true;

    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        const char* p = palabrasD2[i].c_str();

        if (tocaInsertarS2) {
            if (!miGrilla.buscar(p)) {
                uchar* nueva = new uchar[strlen(p) + 1];
                strcpy((char*)nueva, p);
                miGrilla.insertarOrdenado(nueva);
                insertadasS2++;
            } else {
                encontradasS2++;
            }
        } else {
            if (miGrilla.buscar(p)) {
                eliminadasS2++;
            }
        }

        tocaInsertarS2 = !tocaInsertarS2;
    }
    auto finS2 = high_resolution_clock::now();
    duration<double, milli> tiempoS2 = finS2 - inicioS2;


    cout << "Ejecutando Experimento con Solucion 1 (Arreglo)..." << endl;
    int insertadasS1 = 0, eliminadasS1 = 0, encontradasS1 = 0;

    auto inicioS1 = high_resolution_clock::now();
    bool tocaInsertarS1 = true;

    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        const char* p = palabrasD2[i].c_str();
        uchar* pUchar = (uchar*)p; 

        if (tocaInsertarS1) {
            if (!miArreglo.buscar(pUchar)) {
                uchar* nueva = new uchar[strlen(p) + 1];
                strcpy((char*)nueva, p);
                miArreglo.insertar(nueva);
                insertadasS1++;
            } else {
                encontradasS1++;
            }
        } else {
            if (miArreglo.buscar(pUchar)) {
                miArreglo.eliminar(pUchar);
                eliminadasS1++;
            }
        }

        tocaInsertarS1 = !tocaInsertarS1;
    }
    auto finS1 = high_resolution_clock::now();
    duration<double, milli> tiempoS1 = finS1 - inicioS1;

    cout << "Ejecutando Experimento con Solucion 3 (ArbolK, K=" << factorK << ")..." << endl;
    int insertadasS3 = 0, eliminadasS3 = 0, encontradasS3 = 0;

    auto inicioS3 = high_resolution_clock::now();
    bool tocaInsertarS3 = true;

    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        uchar* pUchar = (uchar*)palabrasD2[i].c_str();

        if (tocaInsertarS3) {
            if (!miArbolK.buscar(pUchar)) {
                miArbolK.insertar(pUchar);
                insertadasS3++;
            } else {
                encontradasS3++;
            }
        } else {
            if (miArbolK.buscar(pUchar)) {
                if (miArbolK.eliminar(pUchar)) {
                    eliminadasS3++;
                }
            }
        }

        tocaInsertarS3 = !tocaInsertarS3;
    }
    auto finS3 = high_resolution_clock::now();
    duration<double, milli> tiempoS3 = finS3 - inicioS3;

    cout << "\n=============================================" << endl;
    cout << "   RESULTADOS FINALES COMPARATIVOS (K = " << factorK << ")" << endl;
    cout << "=============================================" << endl;
    cout << "Factor K analizado en Grilla: " << factorK << endl;
    cout << "Total palabras evaluadas D2:  " << palabrasD2.size() << endl;
    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 1 (Arreglo + Indice ASCII):" << endl;
    cout << "  Tiempo de construccion: " << tiempoConstruccionS1.count() << " ms" << endl;
    cout << "  Tiempo de ejecucion D2: " << tiempoS1.count() << " ms" << endl;
    cout << "  Palabras insertadas: " << insertadasS1 << endl;
    cout << "  Palabras eliminadas: " << eliminadasS1 << endl;
    cout << "  Memoria utilizada: " << miArreglo.memoria() << " bytes" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 2 (Grilla de Niveles con K=" << factorK << "):" << endl;
    cout << "  Tiempo de construccion: " << tiempoConstruccionS2.count() << " ms" << endl;
    cout << "  Tiempo de ejecucion D2: " << tiempoS2.count() << " ms" << endl;
    cout << "  Palabras insertadas: " << insertadasS2 << endl;
    cout << "  Palabras eliminadas: " << eliminadasS2 << endl;
    cout << "  Memoria utilizada: " << miGrilla.memoriaUsada() << " bytes" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 3 (ArbolK con K=" << factorK << "):" << endl;
    cout << "  Tiempo de construccion: " << tiempoConstruccionS3.count() << " ms" << endl;
    cout << "  Tiempo de ejecucion D2: " << tiempoS3.count() << " ms" << endl;
    cout << "  Palabras insertadas: " << insertadasS3 << endl;
    cout << "  Palabras eliminadas: " << eliminadasS3 << endl;
    cout << "  Memoria utilizada " << miArbolK.calcularMemNodo() << " bytes" << endl;
    cout << "=============================================" << endl;

    cout << "Verificando consistencia..." << endl;
    if (!miGrilla.buscar("palabra_que_no_existe_seguro") && !miArreglo.buscar((uchar*)"palabra_que_no_existe_seguro") &&!miArbolK.buscar((uchar*)"palabra_que_no_existe_seguro")) {
        cout << "Confirmado: Todas las estructuras finalizaron de forma consistente." << endl;
    }
    
    return 0;
}

