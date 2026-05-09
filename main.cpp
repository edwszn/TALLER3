// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>

       // para probar si el main lee los diccionariosssssss

// #include <iostream>
// #include <fstream>
// #include <string>

// // Definición obligatoria según el enunciado
// typedef unsigned char uchar;

// int main() {
//     // 1. Intentar abrir el diccionario D1 (69.000 palabras aprox.)
//     std::ifstream archivoD1("D1.txt");
//     if (!archivoD1.is_open()) {
//         std::cerr << "Error: No se encontro D1.txt en la raiz del proyecto." << std::endl;
//         return 1;
//     }

//     std::string palabra;
//     int contadorD1 = 0;

//     std::cout << "--- Iniciando lectura de D1.txt ---" << std::endl;
//     while (archivoD1 >> palabra) {
//         contadorD1++;
//         // Imprimimos un progreso cada 20.000 palabras para no saturar la consola
//         if (contadorD1 % 20000 == 0) {
//             std::cout << "Leidas " << contadorD1 << " palabras..." << std::endl;
//         }
//     }
//     std::cout << "Exito: Se leyeron " << contadorD1 << " palabras de D1." << std::endl;
//     archivoD1.close();


//     // 2. Intentar abrir el diccionario D2 (10.000 palabras aprox.)
//     std::ifstream archivoD2("D2.txt");
//     if (!archivoD2.is_open()) {
//         std::cerr << "Error: No se encontro D2.txt." << std::endl;
//         return 1;
//     }

//     int contadorD2 = 0;
//     while (archivoD2 >> palabra) {
//         contadorD2++;
//     }
//     std::cout << "Exito: Se leyeron " << contadorD2 << " palabras de D2." << std::endl;
//     archivoD2.close();

//     std::cout << "\nPrueba finalizada. Ambos archivos son accesibles." << std::endl;

//     return 0;
// }


//aqui empieza lo de la solucion 2
#include "Solucion2.cpp" // para que el main reciba el archivo solucion2.cpp
#include <iostream>
#include <fstream>  
#include <string>   
#include <cstring>  


int main() {
    // 1. Crear la grilla con el factor k (probamos con 32)
    // El k es el que pide el profe (8, 32, 128, 512)
    GrillaNiveles miGrilla(32); 

    std::ifstream archivoD1("D1.txt");
    std::string linea;

    if (!archivoD1.is_open()) {
        std::cout << "No pude abrir D1.txt" << std::endl;
        return 1;
    }

    std::cout << "Cargando palabras en el nivel base..." << std::endl;
    while (archivoD1 >> linea) {
        // Convertimos string a uchar* para que la clase lo acepte
        uchar* palabraPtr = new uchar[linea.length() + 1];
        std::strcpy((char*)palabraPtr, linea.c_str());
        
        miGrilla.insertarNivelBase(palabraPtr);
    }
    archivoD1.close();

    // 2. ESTO ES LO MÁS IMPORTANTE!!!!!!!!!!!!!!!!
    // Crea los niveles superiores (los atajos)
    std::cout << "Construyendo la grilla de niveles..." << std::endl;
    miGrilla.construirNivelesSuperiores();

    // 3. Probar si funciona
    std::cout << "Buscando la palabra deseada:" << std::endl;  
    if (miGrilla.buscar("university")) {   // aqui va la palabra que queremos buscar
        std::cout << "Logrado! La grilla encontro la palabra." << std::endl;
    } else {
        std::cout << "No se encontro, pero la grilla funciona." << std::endl;
    }

    return 0;
}
//hasta aqui llega lo de la solucion 2
