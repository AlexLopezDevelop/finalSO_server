//
// Created by Alex Lopez on 26/11/21.
//

#include "usuario.h"

void freeUsuarios(Usuarios *usuarios);

Usuarios *usuario_obtener_registrados() {
    Usuarios *usuarios = malloc(sizeof(Usuarios));
    usuarios->conectados = malloc(sizeof(LoginData));
    usuarios->totalConectados = 0;
    usuarios->registrados = malloc(sizeof(LoginData));
    usuarios->totalRegistrados = 0;

    ficheros_leer_usuarios_registrados(usuarios);

    return usuarios;
}

void freeUsuarios(Usuarios *usuarios) {
    for (int i = 0; i < usuarios->totalRegistrados; i++) {
        funciones_liberar_memoria(usuarios->registrados[i].nombre);
        funciones_liberar_memoria(usuarios->registrados[i].codigoPostal);
    }

    funciones_liberar_memoria(usuarios->registrados);

    for (int i = 0; i < usuarios->totalConectados; i++) {
        funciones_liberar_memoria(usuarios->conectados[i].nombre);
        funciones_liberar_memoria(usuarios->conectados[i].codigoPostal);
    }

    funciones_liberar_memoria(usuarios->conectados);

    funciones_liberar_memoria(usuarios);
}

void usuario_registrar(LoginData *loginData) {
    Usuarios *usuarios = usuario_obtener_registrados();

    // obtenemos el id para el nuevo usuario
    int userIndex = usuarios->totalRegistrados - 1;
    loginData->id = usuarios->registrados[userIndex].id + 1;

    // Añadimos usuario
    int totalUsuarios = usuarios->totalRegistrados + 1;
    usuarios->totalRegistrados = totalUsuarios;
    usuarios->registrados = realloc(usuarios->registrados, sizeof(LoginData) * totalUsuarios);
    usuarios->registrados[totalUsuarios - 1] = *loginData;

    // Guardamos en fichero
    ficheros_guardar_usuarios_registrados(usuarios);

    freeUsuarios(usuarios);
}

ListadoUsuarios *usuario_buscar_registrados(LoginData *loginData) {
    Usuarios *usuarios = usuario_obtener_registrados();
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

    //freeUsuarios(usuarios);

    return listadoUsuarios;
}

int usuario_obtener_id(LoginData *loginData) {
    Usuarios *usuarios = usuario_obtener_registrados();
    int mismoNombre, mismoCodigoPostal;

    for (int i = 0; i < usuarios->totalRegistrados; ++i) {
        mismoNombre = strcmp(usuarios->registrados[i].nombre, loginData->nombre) == 0;
        mismoCodigoPostal = strcmp(usuarios->registrados[i].codigoPostal, loginData->codigoPostal) == 0;

        if (mismoNombre && mismoCodigoPostal) {
            return usuarios->registrados[i].id;
        }
    }

    freeUsuarios(usuarios);

    return 0;
}

int usuario_existe(LoginData *loginData) {
    Usuarios *usuarios = usuario_obtener_registrados();

    for (int i = 0; i < usuarios->totalRegistrados; ++i) {
        if (usuarios->registrados[i].id == loginData->id) {
            freeUsuarios(usuarios);
            return true;
        }
    }
    freeUsuarios(usuarios);
    return false;
}

void usuario_mensaje_desconectado(char *datos) {
    char *nombre;
    char lineaFile[50], print[100];
    char *id;
    int indexId = 0;
    int datosSize;
    int lineaFileSize;

    strcpy(lineaFile, funciones_read_string_to(datos, '*'));
    nombre = malloc(strlen(lineaFile));
    id = malloc(sizeof(char));
    strcpy(nombre, lineaFile);
    datosSize = (int) strlen(datos);
    lineaFileSize = (int) strlen(lineaFile);
    for (int i = lineaFileSize + 1; i < datosSize; ++i) {
        id[indexId] = datos[i];
        indexId++;
        id = realloc(id, sizeof(char) * indexId);
    }
    sprintf(print, "Rebut logout de %s %s\n", nombre, id);
    funciones_display(print);

    funciones_liberar_memoria(nombre);
    funciones_liberar_memoria(id);
}