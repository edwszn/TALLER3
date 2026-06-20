#include <iostream>
#include <cstring>

typedef unsigned char uchar;

using namespace std;

//Definir la Struct del arbol

struct NodoK {
    uchar** palabras;   // arreglo de palabras guardadas en este nodo
    NodoK** hijos;      // arreglo de punteros a los hijos
    int cantidad;       // cantidad de palabras usadas en este nodo
};

//------------------------------clase principal-------------------------------------
class ArbolK
{
private:
      //ATRIBUTOS
      NodoK* raiz;            //Guarda la raiz del arbol
      int K;                  //Guarda la cantidad de claves
      int cantidadPalabras;  //Un contador de palabras que hay en el arbol
      
      //********Funciones internas Basicas*********
      
      int compararPalabras(uchar* palabra1, uchar* palabra2);   
      uchar* guardarPalabra(uchar* palabra); //Esto podria ser para que guarde una copia en el arbol
      void liberarEspacio(uchar* palabra); //Esto es solo para que no se ocupe memoria de más
      int buscarPosNodo(NodoK* nodo, uchar* palabra); //Esto es para buscar en que posicion del nodo esta la palabra que busco
      int hijoDondeBajar(NodoK* nodo, uchar* palabra); // Esto es para que despues de pillar el nodo con BuscarPosNodo, saber por cual bajar, ya que compara entre palabras, por lo cual tambien debe utilizar comparar palabra
      void liberarNodo(NodoK* nodo); //Para eliminar una rama completa del arbol (Yo creo que lo haremos de manera recursiva
      
      

      
public: 
      //constructor y destructor
      ArbolK(int valorK);
      ~ArbolK();
      
      //funciones principales
      bool insertar (uchar* palabra); // para agregar directamente una palabra en el arbol,  true si se agrego, false si no (puede ser x que ya existia)
      bool buscar(uchar* palabra); //SI esta una palabra o no
      bool eliminar(uchar* palabra); //DEbe eliminar una palabra buscar
      void cargarArchivo(const char* nombreArchivocargar); //PAra abrir d1;
      void experimenD2 (const char* nombreArchivocargar); //Para ejecutar D2, INSERTANDO Y ELIMINANDO
      int obtCantidPalabras();
      int mostrarK();
      bool estaVacio(); //si no hay palabras
      void vaciarArbol(); //aqui usamos la funcion liberar nodo, para eliminar todo el arbol y luego raiz == nullptr;
      
      // ---> void imprimirArbol(); podriamos hacer este para poder mostrar cada nivel del arbol
      
};

ArbolK::ArbolK(int valorK) {
    raiz = nullptr;
    K = valorK;
    cantidadPalabras = 0;
};

ArbolK::~ArbolK() {
    // Por ahora vacío
};
