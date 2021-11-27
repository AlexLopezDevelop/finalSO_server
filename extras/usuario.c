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

int obtenerIdUsuario(LoginData * loginData) {
    Usuarios * usuarios = obtenerUsuariosRegistrados();
    int mismoNombre, mismoCodigoPostal;

    for (int i = 0; i < usuarios->totalRegistrados; ++i) {
        mismoNombre = strcmp(usuarios->registrados[i].nombre, loginData->nombre) == 0;
        mismoCodigoPostal = strcmp(usuarios->registrados[i].codigoPostal, loginData->codigoPostal) == 0;

        if (mismoNombre && mismoCodigoPostal) {
            return usuarios->registrados[i].id;
        }
    }

    return 0;
}

int usuarioExiste(LoginData * loginData) {
    Usuarios * usuarios = obtenerUsuariosRegistrados();

    for (int i = 0; i < usuarios->totalRegistrados; ++i) {
        if (usuarios->registrados[i].id == loginData->id) {
            return true;
        }
    }
    return false;
}

void mensajeDesconectadoUsuario(char *datos){
    char *nombre;
    char lineaFile[50], print[100];
    char * id;
    int indexId=0;
    strcpy(lineaFile,readStringTo(datos,'*'));
    nombre = malloc(strlen(lineaFile));
    id = malloc(sizeof (char));
    strcpy(nombre,lineaFile);
    for (int i = strlen(lineaFile) + 1; i < strlen(datos) ; ++i) {
        id[indexId] = datos[i];
        indexId++;
        id = realloc(id,sizeof (char)*indexId);
    }
    sprintf(print,"Rebut logout de %s %s\n",nombre,id);
    display(print);
}