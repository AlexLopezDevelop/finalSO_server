//
// Created by Alex Lopez on 20/11/21.
//

#include "ficheros.h"
#include "../extras/funciones.h"


#define MAX_LINEA_FILE 200

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

int leerFicheroUsuariosRegistrados(Usuarios * usuarios) {

    int fd;

    fd = open(PATH_USUARIOS_REGISTRADOS, O_RDONLY);

    if (errorAbrir(fd, PATH_USUARIOS_REGISTRADOS)) {
        return 1;
    }

    int totalUsuarios = atoi(readLineFile(fd, '\n'));

    if (totalUsuarios == 0) { // no hay usuarios registrados
        return 0;
    }

    usuarios->totalRegistrados = totalUsuarios;
    usuarios->registrados = realloc(usuarios->registrados, sizeof(LoginData) * totalUsuarios);

    int i = 0;
    char lineaFile[MAX_LINEA_FILE];

    while (!checkEOF(fd)) {
        usuarios->registrados[i].id = atoi(readLineFile(fd, '\n')); // id

        // nombre
        strcpy(lineaFile, readLineFile(fd, '\n'));
        usuarios->registrados[i].nombre = malloc(strlen(lineaFile));
        strcpy(usuarios->registrados[i].nombre, lineaFile);

        // codigo postal
        strcpy(lineaFile, readLineFile(fd, '\n'));
        usuarios->registrados[i].codigoPostal = malloc(strlen(lineaFile));
        strcpy(usuarios->registrados[i].codigoPostal, lineaFile);
        i++;
    }

    return 0;
}

int guardarUsuariosRegistrados(Usuarios * usuarios) {
    int fd;
    
    fd = open(PATH_USUARIOS_REGISTRADOS, O_WRONLY|O_CREAT|O_TRUNC, 00666);

    if (errorAbrir(fd, PATH_USUARIOS_REGISTRADOS)) {
        return 1;
    }

    int totalUsuarios = usuarios->totalRegistrados;
    char aux[100];

    // numero total de registrados
    sprintf(aux, "%d", totalUsuarios);
    write(fd, aux, strlen(aux));
    write(fd,"\n",1);

    for (int i = 0; i < totalUsuarios; ++i) {
        // id
        sprintf(aux, "%d", usuarios->registrados[i].id);
        write(fd, aux, strlen(aux));
        write(fd,"\n",1);

        // nombre
        write(fd, usuarios->registrados[i].nombre, strlen(usuarios->registrados[i].nombre));
        write(fd,"\n",1);

        // codigo postal
        write(fd, usuarios->registrados[i].codigoPostal, strlen(usuarios->registrados[i].codigoPostal));
        write(fd,"\n",1);
    }

    return 0;
}
