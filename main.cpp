
// aqui comienza la solución 2
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm> // Para shuffle
#include <random>    // Para random_device
#include <chrono>    // Para medir tiempos 
#include "Solucion2.cpp" // para que lea el archivo solucion2

using namespace std;
using namespace std::chrono;

int main() {
    int factorK = 512; // Este es el "salto", lo podemos cambiar a 8, 32, 128, 512 para comparar las velocidades
    GrillaNiveles miGrilla(factorK);

    //PASO 1: CARGA DE D1
    ifstream archivoD1("D1.txt");
    string linea;
    if (!archivoD1.is_open()) {
        cout << "Error: No se pudo abrir D1.txt" << endl;
        return 1;
    }

    cout << "PASO 1: Cargando D1 en memoria" << endl;
    while (archivoD1 >> linea) {
        // Limpieza básica por si el archivo viene de Windows/Linux mezclado
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) linea.pop_back();
        
        uchar* palabraPtr = new uchar[linea.length() + 1];
        strcpy((char*)palabraPtr, linea.c_str());
        miGrilla.insertarNivelBase(palabraPtr);
    }
    archivoD1.close();

    // CONSTRUCCIÓN DE LA ESTRUCTURA
    cout << "PASO 2: Construyendo niveles superiores..." << endl;
    miGrilla.construirNivelesSuperiores();

    //PREPARACIÓN DE D2 (para que sean aleatorios) 
    ifstream archivoD2("D2.txt");
    vector<string> palabrasD2;
    if (!archivoD2.is_open()) {
        cout << "Error: No se pudo abrir D2.txt" << endl;
        return 1;
    }
    
    while (archivoD2 >> linea) {
        // Limpieza para los caracteres invisibles
        if (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')) linea.pop_back();
        if (!linea.empty()) palabrasD2.push_back(linea);
    }
    archivoD2.close();

    random_device rd;
    mt19937 g(rd());
    shuffle(palabrasD2.begin(), palabrasD2.end(), g);

    // EXPERIMENTOS 
    cout << "Iniciando experimentos aleatorios con " << palabrasD2.size() << " palabras..." << endl;

    int encontradas = 0;
    int insertadas = 0;
    int eliminadas = 0;

    // Inicia el cronometro para tomar los tiempos
    auto inicioTotal = high_resolution_clock::now();

    for (int i = 0; i < (int)palabrasD2.size(); i++) {
        const char* p = palabrasD2[i].c_str();

        if (i < 5000) {
            // EXPERIMENTO A: Buscar e Insertar si no existe
            if (!miGrilla.buscar(p)) {
                uchar* nueva = new uchar[strlen(p) + 1];
                strcpy((char*)nueva, p);
                miGrilla.insertarOrdenado(nueva);
                insertadas++;
            } else {
                encontradas++;
            }
        } else {
            // EXPERIMENTO B: Buscar y Eliminar si existe
            if (miGrilla.buscar(p)) {
                miGrilla.eliminar(p);
                eliminadas++;
            }
        }

        // Pequeño indicador de progreso, para que no se vea la terminal tan pegada
        if (i % 2500 == 0 && i > 0) cout << "...procesadas " << i << " palabras" << endl;
    }

    auto finTotal = high_resolution_clock::now();
    duration<double, milli> tiempo = finTotal - inicioTotal;

    // RESULTADOS
    cout << "RESULTADOS ETAPA 1 - SOLUCION 2" << endl;
    cout << "============================================" << endl;
    cout << "Factor k:             " << factorK << endl;
    cout << "Tiempo de ejecucion:  " << tiempo.count() << " ms" << endl;
    cout << "Palabras insertadas:  " << insertadas << endl;
    cout << "Palabras eliminadas:  " << eliminadas << endl;
    cout << "Total procesadas D2:  " << palabrasD2.size() << endl;
    cout << "============================================" << endl;

    cout << "Verificando consistencia..." << endl;
    if (!miGrilla.buscar("palabra_que_no_existe_seguro")) {
    cout << "Confirmado: La busqueda arroja negativo para claves inexistentes." << endl;
}
    return 0;
}
