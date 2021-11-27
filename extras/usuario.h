//
// Created by Alex Lopez on 26/11/21.
//

#ifndef FINALSO_SERVER_USUARIO_H
#define FINALSO_SERVER_USUARIO_H

#include "../modelos/conexion.h"

typedef struct {
    LoginData * registrados;
    int totalRegistrados;
    LoginData * conectados;
    int totalConectados;
} Usuarios;

typedef struct {
    LoginData * usuarios;
    int total;
} ListadoUsuarios;

Usuarios *obtenerUsuariosRegistrados();

void registrarUsuario(LoginData *loginData);

ListadoUsuarios *buscarUsuarios(LoginData *loginData);

int obtenerIdUsuario(LoginData * loginData);

int usuarioExiste(LoginData * loginData);

#endif //FINALSO_SERVER_USUARIO_H
