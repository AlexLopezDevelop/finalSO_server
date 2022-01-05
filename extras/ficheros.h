//
// Created by Alex Lopez on 20/11/21.
//

#ifndef FINALSO_SERVER_FICHEROS_H
#define FINALSO_SERVER_FICHEROS_H

#include "funciones.h"
#include "../modelos/configuracion.h"
#include "../modelos/conexion.h"

#define PATH_USUARIOS_REGISTRADOS "usuariosRegistrados.txt"

int ficheros_leer(char *pathFile, Configuracion *configuracion);

int ficheros_leer_usuarios_registrados(Usuarios * usuarios);

int ficheros_guardar_usuarios_registrados(Usuarios * usuarios);

#endif //FINALSO_SERVER_FICHEROS_H
