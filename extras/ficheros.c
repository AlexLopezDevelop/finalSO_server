//
// Created by Alex Lopez on 20/11/21.
//

#include "ficheros.h"
#include "../extras/funciones.h"

int leerFichero(char *pathFile, Configuracion *configuracion) {

    int fd;

    fd = open(pathFile, O_RDONLY);

    if (errorAbrir(fd, pathFile)) {
        return 1;
    }

    while (!checkEOF(fd)) {
        strcpy(configuracion->ip, readLineFile(fd, '\n'));
        configuracion->puerto = atoi(readLineFile(fd, '\n'));
        strcpy(configuracion->directorio, readLineFile(fd, '\n'));

    }

    return 0;
}
