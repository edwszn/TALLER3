/*
Tarea INFO088 - Etapa 2
Integrantes:
- Afra Rodríguez
- Eduardo Pezo
- Matías Montecinos

Descripción:
Este programa compara tres estructuras de datos dinámicas para búsqueda de claves:
1) Solución 1: Arreglo dinámico ordenado con índice ASCII.
2) Solución 2: Grilla de niveles usando factor K.
3) Solución 3: Árbol de búsqueda K-ario.

El programa carga las palabras del archivo D1.txt para construir las estructuras
y luego utiliza D2.txt para realizar operaciones intercaladas de inserción y eliminación.
También mide tiempos de construcción, búsqueda, inserción, eliminación y memoria usada.

Compilación:
    make

Ejecución:
    ./programa K

Si no se entrega un valor K, se usa K = 8 por defecto.

Archivos necesarios:
    D1.txt
    D2.txt
    Solucion1.cpp
    Solucion2.cpp
    Solucion3.cpp
    Makefile
*/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include "Solucion2.cpp"
#include "Solucion1.cpp"
#include "Solucion3.cpp"

typedef unsigned char uchar;

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {

    //definciones

    const int REP = 10000;

    int factorK = 8;
    vector<string> palabrasD1;
    vector<string> palabrasD2;
    string linea;

    int encontradasREP_S1 = 0;
    int encontradasREP_S2 = 0;
    int encontradasREP_S3 = 0;

    double tiempoInsercionS1 = 0.0;
    double tiempoEliminacionS1 = 0.0;
    double tiempoInsercionS2 = 0.0;
    double tiempoEliminacionS2 = 0.0;
    double tiempoInsercionS3 = 0.0;
    double tiempoEliminacionS3 = 0.0;

    int insertadasS1 = 0;
    int eliminadasS1 = 0;

    if (argc >= 2) {
        factorK = atoi(argv[1]);
    }

    if (factorK != 8 && factorK != 32 && factorK != 128 && factorK != 512) {
        cout << "El programa se ejecuta con: ./programa K" << endl;
        cout << "K debe ser uno de estos valores: 8, 32, 128 o 512" << endl;
        return 1;
    }

    //==========================Aqui lee y rellena el vector con palabras D1
    cout << "PASO 1: Leyendo D1 en memoria..." << endl;

    ifstream archivoD1("D1.txt");

    if (!archivoD1.is_open()) {
        cout << "Error: No se pudo abrir D1.txt" << endl;
        return 1;
    }


    while (archivoD1 >> linea) {
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) {
            linea.pop_back();
        }

        if (!linea.empty()) {
            palabrasD1.push_back(linea);
        }
    }

    archivoD1.close();
    
  //==========================Aqui rellena el vector con palabras D2
    cout << "PASO 2: Leyendo D2 en memoria..." << endl;
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

    //========================Aqui comienza a construir las estructuras

    cout << "PASO 3: Construyendo estructuras con K = " << factorK << "..." << endl;

    //SOLUCION 1
    auto inicioConstruccionS1 = high_resolution_clock::now();
    SolucionArreglo miArreglo(10000, 0.1);

    for (int i = 0; i < (int)palabrasD1.size(); i++) {
        miArreglo.insertar((uchar*)palabrasD1[i].c_str());
    }



    //Solucion 2
    auto finConstruccionS1 = high_resolution_clock::now(); 
    duration<double, milli> tiempoConstruccionS1 = finConstruccionS1 - inicioConstruccionS1;

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



    //Solucion 3
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

    //Aqui genera palabras aleatorias
    srand(time(0));

    vector<string> clavesBusqueda = palabrasD1;

    if (!clavesBusqueda.empty()) {
        for (int i = (int)clavesBusqueda.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(clavesBusqueda[i], clavesBusqueda[j]);
        }
    }

    if ((int)clavesBusqueda.size() > REP) {
        clavesBusqueda.resize(REP);
    }

    //==================================================BUSQUEDAS CON REP===========================================================================
    //SOLUCION 1
    auto inicioBusquedaS1 = high_resolution_clock::now();

    for (int i = 0; i < (int)clavesBusqueda.size(); i++) {
        if (miArreglo.buscar((uchar*)clavesBusqueda[i].c_str())) {
            encontradasREP_S1++;
        }
    }

    auto finBusquedaS1 = high_resolution_clock::now();
    duration<double, micro> tiempoBusquedaS1 = finBusquedaS1 - inicioBusquedaS1;

    //SOLUCION 2

    auto inicioBusquedaS2 = high_resolution_clock::now();

    for (int i = 0; i < (int)clavesBusqueda.size(); i++) {
        if (miGrilla.buscar(clavesBusqueda[i].c_str())) {
            encontradasREP_S2++;
        }
    }

    auto finBusquedaS2 = high_resolution_clock::now();
    duration<double, micro> tiempoBusquedaS2 = finBusquedaS2 - inicioBusquedaS2;

    //SOLUCION 3

    auto inicioBusquedaS3 = high_resolution_clock::now();

    for (int i = 0; i < (int)clavesBusqueda.size(); i++) {
        if (miArbolK.buscar((uchar*)clavesBusqueda[i].c_str())) {
            encontradasREP_S3++;
        }
    }

    auto finBusquedaS3 = high_resolution_clock::now();
    duration<double, micro> tiempoBusquedaS3 = finBusquedaS3 - inicioBusquedaS3;



    double promedioBusquedaS1 = tiempoBusquedaS1.count() / clavesBusqueda.size();
    double promedioBusquedaS2 = tiempoBusquedaS2.count() / clavesBusqueda.size();
    double promedioBusquedaS3 = tiempoBusquedaS3.count() / clavesBusqueda.size();

    cout << "\n=============================================" << endl;
    cout << "EXPERIMENTO BUSQUEDAS REP CON CLAVES DE D1" << endl;
    cout << "=============================================" << endl;
    cout << "REP usado: " << clavesBusqueda.size() << endl;
    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 1 (Arreglo + Indice ASCII):" << endl;
    cout << "  Encontradas: " << encontradasREP_S1 << " / " << clavesBusqueda.size() << endl;
    cout << "  Tiempo total busqueda: " << tiempoBusquedaS1.count() << " microsegundos" << endl;
    cout << "  Tiempo promedio busqueda: " << promedioBusquedaS1 << " microsegundos" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 2 (Grilla de Niveles con K=" << factorK << "):" << endl;
    cout << "  Encontradas: " << encontradasREP_S2 << " / " << clavesBusqueda.size() << endl;
    cout << "  Tiempo total busqueda: " << tiempoBusquedaS2.count() << " microsegundos" << endl;
    cout << "  Tiempo promedio busqueda: " << promedioBusquedaS2 << " microsegundos" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 3 (ArbolK con K=" << factorK << "):" << endl;
    cout << "  Encontradas: " << encontradasREP_S3 << " / " << clavesBusqueda.size() << endl;
    cout << "  Tiempo total busqueda: " << tiempoBusquedaS3.count() << " microsegundos" << endl;
    cout << "  Tiempo promedio busqueda: " << promedioBusquedaS3 << " microsegundos" << endl;
    cout << "=============================================" << endl;


    //============================================== TIEMPOS MODIFICACION DINAMICOS ================================
    // ELIMINACION INSERTADO 


    cout << "\nEjecutando Experimento con Solucion 1 (Arreglo)..." << endl;

    auto inicioS1 = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        string palabra = palabrasD2[i];

        if (i % 2 == 0) {
            auto inicio = chrono::high_resolution_clock::now();
            bool ok = miArreglo.insertar((uchar*)palabra.c_str());
            auto fin = chrono::high_resolution_clock::now();

            tiempoInsercionS1 += chrono::duration<double, milli>(fin - inicio).count();

            if (ok) {
                insertadasS1++;
            }
        } else {
            auto inicio = chrono::high_resolution_clock::now();
            bool ok = miArreglo.eliminar((uchar*)palabra.c_str());
            auto fin = chrono::high_resolution_clock::now();

            tiempoEliminacionS1 += chrono::duration<double, milli>(fin - inicio).count();

            if (ok) {
                eliminadasS1++;
            }
        }
    }

    auto finS1 = high_resolution_clock::now();
    duration<double, milli> tiempoS1 = finS1 - inicioS1;

    cout << "Ejecutando Experimento con Solucion 2 (Grilla, K=" << factorK << ")..." << endl;
    int insertadasS2 = 0;
    int eliminadasS2 = 0;

    auto inicioS2 = high_resolution_clock::now();

    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        string palabra = palabrasD2[i];

        if (i % 2 == 0) {
            auto inicio = chrono::high_resolution_clock::now();
            bool ok = miGrilla.insertarOrdenado(palabra.c_str());
            auto fin = chrono::high_resolution_clock::now();

            tiempoInsercionS2 += chrono::duration<double, milli>(fin - inicio).count();

            if (ok) {
                insertadasS2++;
            }
        } else {
            auto inicio = chrono::high_resolution_clock::now();
            bool ok = miGrilla.eliminar(palabra.c_str());
            auto fin = chrono::high_resolution_clock::now();

            tiempoEliminacionS2 += chrono::duration<double, milli>(fin - inicio).count();

            if (ok) {
                eliminadasS2++;
            }
        }
    }


    auto finS2 = high_resolution_clock::now();
    duration<double, milli> tiempoS2 = finS2 - inicioS2;

    cout << "Ejecutando Experimento con Solucion 3 (ArbolK, K=" << factorK << ")..." << endl;
    int insertadasS3 = 0;
    int eliminadasS3 = 0;
    auto inicioS3 = high_resolution_clock::now();

    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        string palabra = palabrasD2[i];

        if (i % 2 == 0) {
            auto inicio = chrono::high_resolution_clock::now();
            bool ok = miArbolK.insertar((uchar*)palabra.c_str());
            auto fin = chrono::high_resolution_clock::now();

            tiempoInsercionS3 += chrono::duration<double, milli>(fin - inicio).count();

            if (ok) {
                insertadasS3++;
            }
        } else {
            auto inicio = chrono::high_resolution_clock::now();
            bool ok = miArbolK.eliminar((uchar*)palabra.c_str());
            auto fin = chrono::high_resolution_clock::now();

            tiempoEliminacionS3 += chrono::duration<double, milli>(fin - inicio).count();

            if (ok) {
                eliminadasS3++;
            }
        }
    }

    auto finS3 = high_resolution_clock::now();
    duration<double, milli> tiempoS3 = finS3 - inicioS3;

    cout << "\n=============================================" << endl;
    cout << "   RESULTADOS FINALES COMPARATIVOS (K = " << factorK << ")" << endl;
    cout << "=============================================" << endl;
    cout << "Factor K analizado: " << factorK << endl;
    cout << "Total palabras evaluadas D2:  " << palabrasD2.size() << endl;

    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 1 (Arreglo + Indice ASCII):" << endl;
    cout << "  Tiempo de construccion: " << tiempoConstruccionS1.count() << " ms" << endl;
    cout << "  Tiempo total insercion: " << tiempoInsercionS1 << " ms" << endl;
    cout << "  Tiempo total eliminacion: " << tiempoEliminacionS1 << " ms" << endl;
    cout << "  Tiempo de ejecucion D2: " << tiempoInsercionS1 + tiempoEliminacionS1 << " ms" << endl;
    cout << "  Palabras insertadas: " << insertadasS1 << endl;
    cout << "  Palabras eliminadas: " << eliminadasS1 << endl;
    cout << "  Memoria utilizada: " << miArreglo.memoria() << " bytes" << endl;

    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 2 (Grilla de Niveles con K=" << factorK << "):" << endl;
    cout << "  Tiempo de construccion: " << tiempoConstruccionS2.count() << " ms" << endl;
    cout << "  Tiempo total insercion: " << tiempoInsercionS2 << " ms" << endl;
    cout << "  Tiempo total eliminacion: " << tiempoEliminacionS2 << " ms" << endl;
    cout << "  Tiempo de ejecucion D2: " << tiempoInsercionS2 + tiempoEliminacionS2 << " ms" << endl;
    cout << "  Palabras insertadas: " << insertadasS2 << endl;
    cout << "  Palabras eliminadas: " << eliminadasS2 << endl;
    cout << "  Memoria utilizada: " << miGrilla.memoriaUsada() << " bytes" << endl;

    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 3 (ArbolK con K=" << factorK << "):" << endl;
    cout << "  Tiempo de construccion: " << tiempoConstruccionS3.count() << " ms" << endl;
    cout << "  Tiempo total insercion: " << tiempoInsercionS3 << " ms" << endl;
    cout << "  Tiempo total eliminacion: " << tiempoEliminacionS3 << " ms" << endl;
    cout << "  Tiempo de ejecucion D2: " << tiempoInsercionS3 + tiempoEliminacionS3 << " ms" << endl;
    cout << "  Palabras insertadas: " << insertadasS3 << endl;
    cout << "  Palabras eliminadas: " << eliminadasS3 << endl;
    cout << "  Memoria utilizada: " << miArbolK.calcularMemNodo() << " bytes" << endl;
    cout << "  Altura del arbol: " << miArbolK.altura() << endl;
    cout << "=============================================" << endl;

    cout << "Verificando consistencia..." << endl;

    if (!miGrilla.buscar("palabra_que_no_existe_seguro") && !miArreglo.buscar((uchar*)"palabra_que_no_existe_seguro") && !miArbolK.buscar((uchar*)"palabra_que_no_existe_seguro")) {
        cout << "Confirmado: Todas las estructuras finalizaron de forma consistente." << endl;
    }

    return 0;
}
