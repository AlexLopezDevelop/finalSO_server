//
// Created by Alex Lopez on 20/11/21.
//

#include "ficheros.h"


#define MAX_LINEA_FILE 200

// Obtiene la informacion de configuracion incial a partir de un fichero
int ficheros_leer(char *pathFile, Configuracion *configuracion) {

    int fd;

    fd = open(pathFile, O_RDONLY);

    if (funciones_error_abrir(fd)) {
        return 1;
    }

    while (!funciones_check_eof(fd)) {
        configuracion->ip = funciones_read_line_file(fd, '\n');
        configuracion->puerto = atoi(funciones_read_line_file(fd, '\n'));
        configuracion->directorio = funciones_read_line_file(fd, '\n');
    }

    return 0;
}

// obtiene los usuarios registrados del txt en el que se guardan
int ficheros_leer_usuarios_registrados(Usuarios * usuarios) {

    int fd;

    fd = open(PATH_USUARIOS_REGISTRADOS, O_RDONLY);

    if (funciones_error_abrir(fd)) {
        return 1;
    }

    int totalUsuarios = atoi(funciones_read_line_file(fd, '\n'));

    if (totalUsuarios == 0) { // no hay usuarios registrados
        return 0;
    }

    usuarios->totalRegistrados = totalUsuarios;
    usuarios->registrados = realloc(usuarios->registrados, sizeof(LoginData) * totalUsuarios);

    int i = 0;
    char lineaFile[MAX_LINEA_FILE];

    while (!funciones_check_eof(fd)) {
        usuarios->registrados[i].id = atoi(funciones_read_line_file(fd, '\n')); // id

        // nombre
        strcpy(lineaFile, funciones_read_line_file(fd, '\n'));
        usuarios->registrados[i].nombre = malloc(strlen(lineaFile)+1);
        strcpy(usuarios->registrados[i].nombre, lineaFile);

        // codigo postal
        strcpy(lineaFile, funciones_read_line_file(fd, '\n'));
        usuarios->registrados[i].codigoPostal = malloc(strlen(lineaFile)+1);
        strcpy(usuarios->registrados[i].codigoPostal, lineaFile);
        i++;
    }

    return 0;
}

// Guarda los usuarios registrados que hay en el struct en el fichero txt
int ficheros_guardar_usuarios_registrados(Usuarios * usuarios) {
    int fd;
    
    fd = open(PATH_USUARIOS_REGISTRADOS, O_WRONLY|O_CREAT|O_TRUNC, 00666);

    if (funciones_error_abrir(fd)) {
        return 1;
    }

    int totalUsuarios = usuarios->totalRegistrados;

    // TODO: quitar estatico
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
