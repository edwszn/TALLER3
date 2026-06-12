Tarea INFO088 - Etapa 1

Matias Montecinos, Afra Rodríguez, Eduardo Pezo

Este programa implementa un arreglo dinamico ordenado a traves de lo siguiente:


1. 	Primero se abre el archivo D1.txt, donde se encuentran las palabras que se van a cargar inicialmente en la estructura.

2. 	Luego, cada palabra de D1 se inserta en el arreglo dinamico ordenado. El orden se mantiene dentro de la funcion insertar(), ya que antes de agregar una palabra se busca la posicion donde corresponde ubicarla. Despues, si es necesario, se desplazan las palabras hacia la derecha para hacer espacio.

3. 	Mientras se cargan las palabras de D1, el programa mide el tiempo de construccion de la estructura. Tambien muestra una estimacion de la memoria utilizada, considerando los espacios reservados por el arreglo y el arreglo auxiliar de indices.

4.	 Si al ejecutar el programa se entrega una palabra por terminal (por ejemplo ./programa abandon) la palabra se busca en estructura. Esta busqueda se realiza en la funcion buscar(), usando busqueda binaria.

5.	 Despues se realizan 10000 busquedas con palabras existentes de D1. Estas busquedas sirven para medir un tiempo promedio, es importante destacar que se ordeno primero lexicograficamente, para luego buscarlo con busqueda binaria, de esta manera es muchisimo mas eficiente.

7. 	Luego se abre el archivo D2.txt, que se usa para probar operaciones dinamicas sobre la estructura.

8. 	El programa recorre las palabras de D2 intercalando operaciones: una palabra se intenta insertar, la siguiente se intenta eliminar, y asi sucesivamente.Si la palabra ya está repetida al insertar, la estructura no cambia. Si se intenta eliminar una palabra que no existe, tampoco se modifica.
9.	 Finalmente, se muestran los resultados obtenidos: cantidad de operaciones realizadas, inserciones exitosas, eliminaciones exitosas, tiempos totales, tiempos promedio y la cantidad final de palabras guardadas en el arreglo.




-La carpeta codigo_fuente incluye:
	main.cpp; donde se ejecuta el programa completo, se hace las pruebas y se entrega resultados correspondiente.
	Para esto se hace un llamado a los dos siguientes documentos que tambien se encuentran en la carpeta.
	dynamic_array.h, Donde se define la clase "ArregloDinamico", sus variables principales y las funciones que se pueden usar (buscar, insertar, eliminar, obtener tamaño)
	dynamic_array.cpp, contiene el codigo completo de las funciones definidas en dynamic_array.h.

-La carpeta Data, incluye los diccionarios entregados por le profesor.
- El Makefile permite compilar el codigo de forma rapida usando el comando make en la Terminal, el cual genera el ejecutable "programa", que es el archivo que se utiliza para ejecutar la tarea.




Para ejecutar el programa, se ejecuta : make . 
A continuacion se ingresa la siguiente linea de codigo: ./programa palabra_buscar_en_d1 (Importante aclarar que esto ultimo se hace solamente para experimentar tiempos de busqueda de esta palabra, y si existe o no)
