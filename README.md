Tarea INFO088 - Etapa 2

Integrantes:
-Matías Montecinos
-Afra Rodríguez
-Eduardo Pezo

====================================DESCRIPCION GENERAL============================================================

El fin de este programa es comparar 3 estructuras de datos dinamicas, permitiendo almacenar claves de tipo uchar*
haciendo operaciones básicas como buscar, insertar y eliminar.

Las tres soluciones implementadas son:

1. Solución 1: Arreglo dinámico ordenado con índice auxiliar ASCII
2. Solución 2: Grilla de niveles usando un factor "k"
3. Solución 3: Árbol de búsqueda k-ario, donde cada nodo puede almacenar hasta "K" claves y hasta "K + 1" hijos.

como funciona el main:

1.lee el archivo "D1.txt", guardando sus palabras en memoria para construir las tres estructuras.
	Durante este proceso se mide el tiempo de construcción de cada solución y se calcula una estimación de la memoria utilizada.

2. Luego se buscan 10000 claves aleatorias existentes en D1. Permitiendo medir tiempos promedios de busqueda.

3. luego se abre el archivo "D2.txt" y recorre sus palabras intercalando operaciones: una palabra se intenta insertar, la siguiente se intenta eliminar, y así sucesivamente. Si una palabra ya existe al momento de insertar, la estructura no se modifica. Si se intenta eliminar una palabra que no existe, la estructura tampoco cambia.

Finalmente, el programa muestra los resultados comparativos de las tres soluciones.

Datos que entrega:
	Tiempo de construcción desde `D1.txt`.
	Tiempo total y promedio de búsqueda.
	Cantidad de palabras encontradas.
	Cantidad de inserciones exitosas.
	Cantidad de eliminaciones exitosas.
	Tiempo de ejecución de las operaciones con `D2.txt`.
	Memoria utilizada por cada estructura.
	Altura del árbol en la Solución 3.

=======================================================Ejecución: ===================================================

Compilación:
Para que compile el main.cpp y por rebote las 3 solución se ejecuta make, el cual se encarga de crear un nuevo archivo llamado programa.

Para Ejecutar:
El programa debe abrirse como "./programa" (en el bash), y de esta manera inicia el programa con un k = 8, en caso contario queriendo hacer las pruebas con distintos k, se debe ejecutar el comando "./programa k".

//k es el valor que utiliza la solución 2 y 3, para crear las grillas con factor k, y para crear el arbol con k, y k + 1 correspondientemente.

====================================================Archivos que incluye==============================================
Archivos principales

La carpeta del código fuente incluye los siguientes archivos:

main.cpp: contiene el programa principal, lee los archivos `D1.txt` y `D2.txt`, construye las tres estructuras, ejecuta los experimentos y muestra los resultados.
-Solucion1.cpp: contiene la implementación del arreglo dinámico ordenado con índice ASCII
-Solucion2.cpp: contiene la implementación de la grilla de niveles con factor "K"
-Solucion3.cpp: contiene la implementación del árbol k ario
-Makefile: encargado de compilar el programa usando el comando bash: make

Archivos de datos

D1.txt: diccionario usado para construir inicialmente las estructuras.
D2.txt: diccionario usado para intercalar inserciones y eliminaciones dinámicas.


==================================================Resultados esperados===========================================================

Al ejecutar el programa, se muestran por pantalla los resultados de construcción, búsqueda, inserción, eliminación, memoria utilizada y consistencia final de las tres estructuras. Estos datos nos permiten realizar la comparación entre los 3 datos.
