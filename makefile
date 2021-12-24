all:Atreides

main.o: main.c modelos/configuracion.h extras/funciones.h extras/ficheros.h extras/utils.h
		gcc -c -g main.c -Wall -Wextra -lpthread

usuario.o: extras/usuario.c extras/usuario.h extras/funciones.h extras/ficheros.h modelos/conexion.h
		gcc -c -g extras/usuario.c -Wall -Wextra -lpthread

funciones.o: extras/funciones.c extras/funciones.h modelos/configuracion.h extras/usuario.h
		gcc -c -g extras/funciones.c -Wall -Wextra -lpthread

ficheros.o: extras/ficheros.c extras/ficheros.h extras/funciones.h modelos/conexion.h
		gcc -c -g extras/ficheros.c -Wall -Wextra -lpthread

utils.o: extras/utils.c extras/utils.h extras/funciones.h modelos/conexion.h extras/usuario.h extras/menu.h
		gcc -c -g extras/utils.c -Wall -Wextra -lpthread

menu.o: extras/menu.c extras/menu.h extras/utils.h extras/funciones.h modelos/conexion.h
		gcc -c -g extras/menu.c -Wall -Wextra -lpthread

Atreides: main.o funciones.o ficheros.o utils.o usuario.o menu.o
		gcc main.o funciones.o ficheros.o utils.o usuario.o menu.o -o Atreides.exe -Wall -Wextra -lpthread

clean:
		rm -rf *.o *.exe

do:
		./Atreides.exe Config.dat

val:
		valgrind --dsymutil=yes --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./Atreides.exe Config.dat