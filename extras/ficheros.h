//
// Created by Alex Lopez on 20/11/21.
//

#ifndef FINALSO_SERVER_FICHEROS_H
#define FINALSO_SERVER_FICHEROS_H

#include "funciones.h"
#include "../modelos/configuracion.h"
#include "../modelos/conexion.h"

#define PATH_USUARIOS_REGISTRADOS "usuariosRegistrados.txt"

int leerFichero(char *pathFile, Configuracion *configuracion);

int leerFicheroUsuariosRegistrados(Usuarios * usuarios);

int guardarUsuariosRegistrados(Usuarios * usuarios);

#endif //FINALSO_SERVER_FICHEROS_H
