//
// Created by Alex Lopez on 26/11/21.
//

#include "usuario.h"
#include "funciones.h"
#include "ficheros.h"

Usuarios *obtenerUsuariosRegistrados() {
    Usuarios *usuarios = malloc(sizeof(Usuarios));
    usuarios->conectados = malloc(sizeof(LoginData));
    usuarios->totalConectados = 0;
    usuarios->registrados = malloc(sizeof(LoginData));
    usuarios->totalRegistrados = 0;

    leerFicheroUsuariosRegistrados(usuarios);

    return usuarios;
}

void registrarUsuario(LoginData *loginData) {
    Usuarios *usuarios = obtenerUsuariosRegistrados();

    // obtenemos el id para el nuevo usuario
    int userIndex = usuarios->totalRegistrados - 1;
    loginData->id = usuarios->registrados[userIndex].id + 1;

    // Añadimos usuario
    int totalUsuarios = usuarios->totalRegistrados + 1;
    usuarios->totalRegistrados = totalUsuarios;
    usuarios->registrados = realloc(usuarios->registrados, sizeof(LoginData) * totalUsuarios);
    usuarios->registrados[totalUsuarios - 1] = *loginData;

    // Guardamos en fichero
    guardarUsuariosRegistrados(usuarios);
}

ListadoUsuarios *buscarUsuarios(LoginData *loginData) {
    Usuarios *usuarios = obtenerUsuariosRegistrados();
    ListadoUsuarios *listadoUsuarios = malloc(sizeof(ListadoUsuarios));
    listadoUsuarios->usuarios = malloc(sizeof(Usuarios));
    listadoUsuarios->total = 0;

    for (int i = 0; i < usuarios->totalRegistrados; ++i) {
        if (strcmp(usuarios->registrados[i].codigoPostal, loginData->codigoPostal) == 0) {

            listadoUsuarios->total++;
            listadoUsuarios->usuarios = realloc(listadoUsuarios->usuarios, sizeof(Usuarios) * listadoUsuarios->total);
            listadoUsuarios->usuarios[listadoUsuarios->total - 1] = usuarios->registrados[i];
        }
    }

    return listadoUsuarios;
}