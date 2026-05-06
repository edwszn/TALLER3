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
