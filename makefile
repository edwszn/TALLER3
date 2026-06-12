all:
	g++ codigo_fuente/main.cpp codigo_fuente/dynamic_array.cpp -o programa

run:
	./programa

clean:
	rm -f programa
