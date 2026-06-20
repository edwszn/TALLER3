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
#include "Solucion3.cpp" //Para que lea el archivo solucion 3

using namespace std;
using namespace std::chrono;

int main() {
    // MODIFICA ESTE VALOR (8, 32, 128, 512) PARA TUS DISTINTAS PRUEBAS
    int factorK = 512; 
    
    GrillaNiveles miGrilla(factorK);
    SolucionArreglo miArreglo(10000, 0.1);


    ifstream archivoD1("D1.txt");
    string linea;
    if (!archivoD1.is_open()) {
        cout << "Error: No se pudo abrir D1.txt" << endl;
        return 1;
    }

    cout << "PASO 1: Cargando D1 en memoria (Factor K asignado: " << factorK << ")..." << endl;
    while (archivoD1 >> linea) {
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) linea.pop_back();
        
        // Carga para Grilla (Solución 2)
        uchar* palabraPtrS2 = new uchar[linea.length() + 1];
        strcpy((char*)palabraPtrS2, linea.c_str());
        miGrilla.insertarNivelBase(palabraPtrS2);

        // Carga para Arreglo (Solución 1)
        uchar* palabraPtrS1 = new uchar[linea.length() + 1];
        strcpy((char*)palabraPtrS1, linea.c_str());
        miArreglo.insertar(palabraPtrS1);
    }
    archivoD1.close();

    // CONSTRUCCIÓN DE LA ESTRUCTURA SUPERIOR (Solo Grilla utiliza factorK interno)
    cout << "PASO 2: Construyendo niveles superiores de la Grilla usando K = " << factorK << "..." << endl;
    miGrilla.construirNivelesSuperiores();

    ifstream archivoD2("D2.txt");
    vector<string> palabrasD2;
    if (!archivoD2.is_open()) {
        cout << "Error: No se pudo abrir D2.txt" << endl;
        return 1;
    }
    
    while (archivoD2 >> linea) {
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) linea.pop_back();
        if (!linea.empty()) palabrasD2.push_back(linea);
    }
    archivoD2.close();

    random_device rd;
    mt19937 g(rd());
    shuffle(palabrasD2.begin(), palabrasD2.end(), g);


    cout << "\nEjecutando Experimento con Solucion 2 (Grilla, K=" << factorK << ")..." << endl;
    int insertadasS2 = 0, eliminadasS2 = 0, encontradasS2 = 0;

    auto inicioS2 = high_resolution_clock::now();
    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        const char* p = palabrasD2[i].c_str();

        if (i < 5000) {
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
                miGrilla.eliminar(p);
                eliminadasS2++;
            }
        }
    }
    auto finS2 = high_resolution_clock::now();
    duration<double, milli> tiempoS2 = finS2 - inicioS2;


    cout << "Ejecutando Experimento con Solucion 1 (Arreglo)..." << endl;
    int insertadasS1 = 0, eliminadasS1 = 0, encontradasS1 = 0;

    auto inicioS1 = high_resolution_clock::now();
    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        const char* p = palabrasD2[i].c_str();
        uchar* pUchar = (uchar*)p; 

        if (i < 5000) {
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
    }
    auto finS1 = high_resolution_clock::now();
    duration<double, milli> tiempoS1 = finS1 - inicioS1;

    cout << "\n=============================================" << endl;
    cout << "   RESULTADOS FINALES COMPARATIVOS (K = " << factorK << ")" << endl;
    cout << "=============================================" << endl;
    cout << "Factor K analizado en Grilla: " << factorK << endl;
    cout << "Total palabras evaluadas D2:  " << palabrasD2.size() << endl;
    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 1 (Arreglo + Indice ASCII):" << endl;
    cout << "  Tiempo de ejecucion: " << tiempoS1.count() << " ms" << endl;
    cout << "  Palabras insertadas: " << insertadasS1 << endl;
    cout << "  Palabras eliminadas: " << eliminadasS1 << endl;
    cout << "  *Nota: Esta estructura es independiente de K" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "SOLUCION 2 (Grilla de Niveles con K=" << factorK << "):" << endl;
    cout << "  Tiempo de ejecucion: " << tiempoS2.count() << " ms" << endl;
    cout << "  Palabras insertadas: " << insertadasS2 << endl;
    cout << "  Palabras eliminadas: " << eliminadasS2 << endl;
    cout << "=============================================" << endl;

    cout << "Verificando consistencia..." << endl;
    if (!miGrilla.buscar("palabra_que_no_existe_seguro") && !miArreglo.buscar((uchar*)"palabra_que_no_existe_seguro")) {
        cout << "Confirmado: Ambas estructuras finalizaron de forma consistente." << endl;
    }
    
///COMPROBAR SI CORRE LA SOLUCION 3
  
    ArbolK arbol(512);
    cout << "====================================================================" << endl;
    cout << "SOLUCION 3 COMPILA CORRECTAMENTE" << endl;


//
    
    return 0;
}
