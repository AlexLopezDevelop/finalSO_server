//
// Created by Alex Lopez on 26/11/21.
//

#ifndef FINALSO_SERVER_USUARIO_H
#define FINALSO_SERVER_USUARIO_H

#include "ficheros.h"
#include "funciones.h"
#include "../modelos/conexion.h"

Usuarios *usuario_obtener_registrados();

void usuario_registrar(LoginData *loginData);

ListadoUsuarios *usuario_buscar_registrados(LoginData *loginData);

int usuario_obtener_id(LoginData * loginData);

int usuario_existe(LoginData * loginData);

void usuario_mensaje_desconectado(char * datos);

#endif //FINALSO_SERVER_USUARIO_H
