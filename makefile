all:Atreides

main.o: main.c extras/funciones.h extras/ficheros.h modelos/configuracion.h extras/utils.h
		gcc -c main.c -Wall -Wextra -lpthread

funciones.o: extras/funciones.c extras/funciones.h
		gcc -c ../extras/funciones.c -Wall -Wextra -lpthread

ficheros.o: extras/ficheros.c extras/ficheros.h extras/funciones.h
		gcc -c ../extras/ficheros.c -Wall -Wextra -lpthread

utils.o: extras/utils.c extras/funciones.h extras/ficheros.h modelos/configuracion.h
		gcc -c extras/utils.c -Wall -Wextra -lpthread

Atreides: main.o funciones.o ficheros.o utils.o
		gcc main.o funciones.o ficheros.o utils.o -o Atreides.exe -Wall -Wextra -lpthread

clean:
		rm -rf *.o *.exe

do:
		./Atreides.exe Config.dat

val:
		valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./Atreides.exe Config.dat