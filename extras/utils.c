//
// Created by Alex Lopez on 13/11/21.
//

#include "../extras/funciones.h"
#include "../modelos/configuracion.h"
#include "../modelos/conexion.h"
#include "ficheros.h"

#define printF(x) write(1, x, strlen(x))

int listenFD;

ConexionData *guardarTrama(const char *trama) {
    ConexionData *conexionData;
    conexionData = malloc(sizeof(ConexionData));

    // obtener origen
    for (int i = 0; i < TRAMA_ORIGEN_SIZE; ++i) {
        conexionData->origen[i] = trama[i];
        if (trama[i] == '\0') {
            break;
        }
    }

    // obtener tipo
    conexionData->tipo = trama[TRAMA_ORIGEN_SIZE];

    // obtener data
    int dataIndex = 0;

    for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; ++i) {
        conexionData->datos[dataIndex] = trama[i];
        dataIndex++;
        if (trama[i] == '\0') {
            break;
        }
    }

    return conexionData;
}

LoginData *destructData(char *datos) {
    LoginData *loginData = malloc(sizeof(LoginData));
    loginData->nombre = malloc(sizeof(char));
    loginData->codigoPostal = malloc(sizeof(char));

    int isCodigoPostal = false;
    int cpIndex = 0;
    int sizeDatos = strlen(datos);

    for (int i = 0; i < sizeDatos; ++i) {
        if (datos[i] == '*') {
            loginData->nombre[i] = '\0';
            isCodigoPostal = true;
            i++;
        }

        if (!isCodigoPostal) {
            loginData->nombre[i] = datos[i];
            loginData->nombre = realloc(loginData->nombre, sizeof(char) * (i + 2));
        } else {
            loginData->codigoPostal[cpIndex] = datos[i];
            loginData->codigoPostal = realloc(loginData->codigoPostal, sizeof(char) * (cpIndex + 2));
            cpIndex++;
            if (i + 1 == sizeDatos) { // final del string
                loginData->codigoPostal[cpIndex] = '\0';
            }
        }

    }

    return loginData;
}

// TODO: add to users model
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

char *crearTrama(char *origen, char tipo, char *data) {
    char *trama = NULL;
    trama = malloc(sizeof(char) * MAX_TRAMA_SIZE);

    int origenSize = strlen(origen);
    int tipoSize = 1;
    int dataSize = strlen(data);

    // origen
    for (int i = 0; i < TRAMA_ORIGEN_SIZE; ++i) {
        if (i < origenSize) {
            trama[i] = origen[i];
        } else {
            trama[i] = '\0';
        }
    }

    // tipo
    trama[TRAMA_ORIGEN_SIZE] = tipo;


    // data
    int dataIndex = 0;

    for (int i = TRAMA_ORIGEN_SIZE + tipoSize; i < TRAMA_DATA_SIZE; ++i) {
        if (dataIndex < dataSize) {
            trama[i] = data[dataIndex];
            dataIndex++;
        } else {
            trama[i] = '\0';
        }
    }
    return trama;
}

char *obtenerTrama(char tipo, char *data) {
    return crearTrama("ATREIDES", tipo, data);
}

LoginData *destructDataSearch(char *tramaDatos) {
    bool isCodigoPostal = false, isId = false;
    int idIndex = 0, cpIndex = 0;
    int sizeDatos = strlen(tramaDatos);
    char auxId[100];

    LoginData *loginData;
    loginData = malloc(sizeof(LoginData));
    loginData->nombre = malloc(sizeof(char));
    loginData->codigoPostal = malloc(sizeof(char));

    for (int i = 0; i < sizeDatos; ++i) {
        if (tramaDatos[i] == '*' && (isCodigoPostal == false && isId == false)) {
            loginData->nombre[i] = '\0';
            isId = true;
        } else if (tramaDatos[i] == '*' && isId == true) {
            isCodigoPostal = true;
        } else if (isCodigoPostal == false && isId == false) {
            loginData->nombre[i] = tramaDatos[i];
            loginData->nombre = realloc(loginData->nombre, sizeof(char) * (i + 2));
        } else if (isCodigoPostal == false && isId == true) {
            auxId[idIndex] = tramaDatos[i];
            idIndex++;
        } else {
            loginData->codigoPostal[cpIndex] = tramaDatos[i];
            loginData->codigoPostal = realloc(loginData->codigoPostal, sizeof(char) * (cpIndex + 2));
            cpIndex++;
            if (i + 1 == sizeDatos) { // final del string
                loginData->codigoPostal[cpIndex] = '\0';
            }
        }
        loginData->id = atoi(auxId);
    }

    return loginData;
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

char * crearDataSearch(ListadoUsuarios * listadoUsuarios) {
    char * data = malloc(sizeof (char));
    char stringAux [100];
    int dataSize;

    // total usuarios
    sprintf(stringAux, "%d", listadoUsuarios->total);
    dataSize = strlen(stringAux);
    data = realloc(data, sizeof (char) * dataSize);
    strcpy(data, stringAux);

    // *
    dataSize++;
    data = realloc(data, sizeof (char) * dataSize);
    strcat(data, "*");

    // Usuarios
    for (int i = 0; i < listadoUsuarios->total; ++i) {

        // Nombre
        dataSize = dataSize + strlen(listadoUsuarios->usuarios[i].nombre);
        data = realloc(data, sizeof (char) * dataSize);
        strcat(data, listadoUsuarios->usuarios[i].nombre);

        // *
        dataSize++;
        data = realloc(data, sizeof (char) * dataSize);
        strcat(data, "*");

        // id
        sprintf(stringAux, "%d", listadoUsuarios->usuarios[i].id);
        dataSize = dataSize + strlen(stringAux);
        data = realloc(data, sizeof (char) * dataSize);
        strcat(data, stringAux);

        // controlar asterisco final
        if (listadoUsuarios->total - 1 != i) {
            // *
            dataSize++;
            data = realloc(data, sizeof (char) * dataSize);
            strcat(data, "*");
        }
    }

    return data;
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

void salir() {
    close(listenFD);
    signal(SIGINT, SIG_DFL);
    raise(SIGINT);
}

void gestorDeSockets() {
    int clientFD;
    struct sockaddr_in servidor;

    signal(SIGINT, salir);

    if ((listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_port = htons(8710);
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenFD, (struct sockaddr *) &servidor, sizeof(servidor)) < 0) {
        printF("Error fent el bind\n");
    }

    if (listen(listenFD, 10) < 0) {
        printF("Error fent el listen\n");
    }

    while (1) {
        printF("Esperant connexions...\n");

        clientFD = accept(listenFD, (struct sockaddr *) NULL, NULL);

        printF("\nNou client connectat!\n");

        pthread_t thrd;
        printf("clientFD: %d\n", clientFD);
        pthread_create(&thrd, NULL, comprobarNombres, (void *) &clientFD);
    }
}