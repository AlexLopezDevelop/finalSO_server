//
// Created by Alex Lopez on 26/11/21.
//

#include "menu.h"
#include "utils.h"
#include "funciones.h"

void *comprobarNombres(void *arg) {
    int clientFD = *(int *) arg;
    int salir = 0;
    char idString[30];
    char print[100];
    ConexionData *conexionData;

    while (salir == 0) {
        char trama[MAX_TRAMA_SIZE];
        read(clientFD, trama, MAX_TRAMA_SIZE);

        conexionData = guardarTrama(trama);

        switch (trama[15]) {
            case 'C':   //Login
                display("\n\nReceived login ");
                LoginData *loginData = destructData(conexionData->datos);
                sprintf(print, "%s %s\n", loginData->nombre, loginData->codigoPostal);
                display(print);
                // TODO: revisar si existe
                // buscarUsuarios()
                registrarUsuario(loginData);
                sprintf(idString, "%d", loginData->id);
                sprintf(print, "Assigned ID %s.\n", idString);
                display(print);
                char *tramaRespuesta = obtenerTrama('O', idString);
                write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                display("Send answer\n\n");
                break;
            case 'S':   //search
                display("Buscando usuarios\n");
                char *data = opcionBuscarUsuario(conexionData);
                // TODO: devolver data al usuario
                break;
            case 'Q':   //logout
                display("\nCliente Desconectado!\n\n");
                close(clientFD);
                salir = 1;//Logout
                break;

        }
    }

    pthread_cancel(pthread_self());
    pthread_detach(pthread_self());

    return NULL;
}

char * opcionBuscarUsuario(ConexionData *conexionData) {
    LoginData *loginData = destructDataSearch(conexionData->datos);
    ListadoUsuarios *listadoUsuarios = buscarUsuarios(loginData);
    char print[200];

    sprintf(print, "Rebut search %s de %s %d\nFeta la cerca\n", loginData->codigoPostal, loginData->nombre,
            loginData->id);
    display(print);

    if (listadoUsuarios->total == 0) {
        display("No hay ningun usuario con el codigo postal ");
        display(loginData->codigoPostal);
        display("\n");
    } else {
        sprintf(print, "Hi han %d persones humanes a %s\n\n", listadoUsuarios->total, loginData->codigoPostal);
        display(print);
        display("\n");

        for (int i = 0; i < listadoUsuarios->total; ++i) {
            sprintf(print, "%s ", listadoUsuarios->usuarios[i].codigoPostal);
            display(print);
            display(listadoUsuarios->usuarios[i].nombre);
            display("\n");
        }
    }

    return crearDataSearch(listadoUsuarios);
}