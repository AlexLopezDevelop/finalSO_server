//
// Created by Alex Lopez on 26/11/21.
//

#ifndef FINALSO_SERVER_USUARIO_H
#define FINALSO_SERVER_USUARIO_H

#include "ficheros.h"
#include "funciones.h"
#include "../modelos/conexion.h"

Usuarios *obtenerUsuariosRegistrados();

void registrarUsuario(LoginData *loginData);

ListadoUsuarios *buscarUsuarios(LoginData *loginData);

int obtenerIdUsuario(LoginData * loginData);

int usuarioExiste(LoginData * loginData);

void mensajeDesconectadoUsuario(char * datos);

#endif //FINALSO_SERVER_USUARIO_H
