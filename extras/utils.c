//
// Created by Alex Lopez on 13/11/21.
//

#include "utils.h"
#include "funciones.h"
#include "menu.h"

#define printF(x) write(1, x, strlen(x))

int listenFD;

ConexionData *utils_guardar_trama(const char *trama) {
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

    if (conexionData->tipo == 'D') {
        for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; ++i) {
            conexionData->datos[dataIndex] = trama[i];
            dataIndex++;
        }
    } else {
        for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; ++i) {
            conexionData->datos[dataIndex] = trama[i];
            dataIndex++;
            if (trama[i] == '\0') {
                break;
            }
        }
    }

    return conexionData;
}

FotoData *utils_destruct_data_imagen(char *datos) {
    FotoData *fotoData = malloc(sizeof(FotoData));
    char delim[] = "*";
    char *ptr = strtok(datos, delim);

    while (ptr != NULL) {
        fotoData->nombre = strdup(ptr);
        ptr = strtok(NULL, delim);
        fotoData->size = atoi(ptr);
        ptr = strtok(NULL, delim);
        fotoData->md5sum = strdup(ptr);
        ptr = strtok(NULL, delim);
    }

    fotoData->sizeTrama = fotoData->size / TRAMA_DATA_SIZE;

    return fotoData;
}

LoginData *utils_destruct_data(char *datos) {
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

    if (tipo == 'F') {
        for (int i = TRAMA_ORIGEN_SIZE + tipoSize; i < MAX_TRAMA_SIZE; i++) {
            trama[i] = data[dataIndex];
            dataIndex++;
        }
    } else {
        for (int i = TRAMA_ORIGEN_SIZE + tipoSize; i < TRAMA_DATA_SIZE; ++i) {
            if (dataIndex < dataSize) {
                trama[i] = data[dataIndex];
                dataIndex++;
            } else {
                trama[i] = '\0';
            }
        }
    }

    return trama;
}

char *utils_obtener_trama(char tipo, char *data) {
    return crearTrama("ATREIDES", tipo, data);
}

LoginData *utils_destruct_data_search(char *tramaDatos) {
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

char *utils_crear_data_search(ListadoUsuarios *listadoUsuarios) {
    char *data = malloc(sizeof(char));
    char stringAux[100];
    int dataSize;
    int nombreSize;
    int stringAuxSize;

    // total usuarios
    sprintf(stringAux, "%d", listadoUsuarios->total);
    dataSize = strlen(stringAux);
    data = realloc(data, sizeof(char) * dataSize);
    strcpy(data, stringAux);

    // *
    dataSize++;
    data = realloc(data, sizeof(char) * dataSize);
    data[dataSize - 1] = '*';

    // Usuarios
    if (listadoUsuarios->total != 0) {
        for (int i = 0; i < listadoUsuarios->total; ++i) {

            // Nombre
            nombreSize = (int) strlen(listadoUsuarios->usuarios[i].nombre);
            for (int j = 0; j < nombreSize; ++j) {
                dataSize++;
                data = realloc(data, sizeof(char) * dataSize);
                data[dataSize - 1] = listadoUsuarios->usuarios[i].nombre[j];
            }

            // *
            dataSize++;
            data = realloc(data, sizeof(char) * dataSize);
            data[dataSize - 1] = '*';

            // id
            sprintf(stringAux, "%d", listadoUsuarios->usuarios[i].id);
            stringAuxSize = (int) strlen(stringAux);
            for (int j = 0; j < stringAuxSize; ++j) {
                dataSize++;
                data = realloc(data, sizeof(char) * dataSize);
                data[dataSize - 1] = stringAux[j];
            }

            // controlar asterisco final
            if (listadoUsuarios->total != i) {
                // *
                dataSize++;
                data = realloc(data, sizeof(char) * dataSize);
                data[dataSize - 1] = '*';
            }
        }
    } else {
        data = "0";
    }

    return data;
}

void utils_salir() {
    close(listenFD);
    signal(SIGINT, SIG_DFL);
    raise(SIGINT);
}

int utils_gestor_de_sockets() {
    int clientFD;
    struct sockaddr_in servidor;

    signal(SIGINT, utils_salir);

    if ((listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_port = htons(8755);
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
        pthread_create(&thrd, NULL, menu_comprobar_nombres, (void *) &clientFD);
    }
}