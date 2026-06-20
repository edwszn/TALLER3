//Este archivo es unicamente para hacer las mediciones de los timpos de la solucion1.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm> // Para shuffle
#include <random>    // Para random_device
#include <chrono>    // Para medir tiempos 
#include "Solucion1.cpp" // 

using namespace std;
using namespace std::chrono;

int main() {
    //Inicialización
    SolucionArreglo miArreglo(10000, 0.1);

    // Carga inicial (D1.txt)
    ifstream archivoD1("D1.txt");
    string linea;
    if (!archivoD1.is_open()) {
        cout << "Error: No se pudo abrir D1.txt" << endl;
        return 1;
    }
    while (archivoD1 >> linea) {
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) linea.pop_back();
        uchar* p = new uchar[linea.length() + 1];
        strcpy((char*)p, linea.c_str());
        miArreglo.insertar(p);
    }
    archivoD1.close();

    //Carga D2 para experimento
    ifstream archivoD2("D2.txt");
    vector<string> palabrasD2;
    while (archivoD2 >> linea) {
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) linea.pop_back();
        if (!linea.empty()) palabrasD2.push_back(linea);
    }
    archivoD2.close();

    random_device rd;
    mt19937 g(rd());
    shuffle(palabrasD2.begin(), palabrasD2.end(), g);

    //INICIO DE MEDICIÓN (Solo Inserciones y Eliminaciones de D2)
    cout << "Ejecutando medicion de Solucion 1..." << endl;
    auto inicioS1 = high_resolution_clock::now();

    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        const char* p = palabrasD2[i].c_str();
        uchar* pUchar = (uchar*)p; 

        if (i < 5000) { // Inserciones
            if (!miArreglo.buscar(pUchar)) {
                uchar* nueva = new uchar[strlen(p) + 1];
                strcpy((char*)nueva, p);
                miArreglo.insertar(nueva);
            }
        } else { // Eliminaciones
            if (miArreglo.buscar(pUchar)) {
                miArreglo.eliminar(pUchar);
            }
        }
    }
    auto finS1 = high_resolution_clock::now();
    
    // 5. Cálculo y Resultado
    duration<double, milli> tiempoS1 = finS1 - inicioS1;
    cout << "---------------------------------------------" << endl;
    cout << "Tiempo de ejecucion S1 (insercion/eliminacion): " << tiempoS1.count() << " ms" << endl;
    cout << "---------------------------------------------" << endl;

    return 0;
}