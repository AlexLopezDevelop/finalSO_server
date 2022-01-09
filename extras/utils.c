//
// Created by Alex Lopez on 13/11/21.
//

#include "utils.h"
#include "funciones.h"
#include "menu.h"

#define printF(x) write(1, x, strlen(x))

int listenFD;

// se le pasa una trama y la devuelve la informacion en un struct
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
                return conexionData;
            }
        }
    }

    return conexionData;
}

// devuelve un struct con los datos de la trama de imagen
FotoData *utils_destruct_data_imagen(char *datos) {
    FotoData *fotoData = malloc(sizeof(FotoData));
    char delim[] = "*";
    char *ptr = strtok(datos, delim);

    while (ptr != NULL) {
        fotoData->nombre = strdup(ptr);
        ptr = strtok(NULL, delim);
        fotoData->size = atoi(ptr);
        ptr = strtok(NULL, delim);
        if(ptr != NULL) {
            fotoData->md5sum = strdup(ptr);
            ptr = strtok(NULL, delim);
        }
    }

    fotoData->sizeTrama = fotoData->size / TRAMA_DATA_SIZE;

    funciones_liberar_memoria(ptr);

    return fotoData;
}

// devuelve un struct con los datos de la trama de imagen
LoginData *utils_destruct_data(char *datos) {
    LoginData *loginData = malloc(sizeof(LoginData));

    char delim[] = "*";
    char *ptr = strtok(datos, delim);

    while (ptr != NULL) {
        loginData->nombre = strdup(ptr);
        ptr = strtok(NULL, delim);
        loginData->codigoPostal = strdup(ptr);
        ptr = strtok(NULL, delim);
    }

    return loginData;
}

// crea una trama para enviar al servidor con toda la informacion necesaria
char *crearTrama(char *origen, char tipo, char *data) {
    char *trama = NULL;
    trama = malloc(sizeof(char) * MAX_TRAMA_SIZE);

    int tipoSize = 1;
    int origenSize = strlen(origen);

    // origen
    for (int i = 0; i < TRAMA_ORIGEN_SIZE; i++) {
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
        int dataSize = strlen(data);

        for (int i = TRAMA_ORIGEN_SIZE + tipoSize; i < MAX_TRAMA_SIZE; i++) {
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

// devuelve el struct con la informacion base para el envio
char *utils_obtener_trama(char tipo, char *data) {
    return crearTrama("ATREIDES", tipo, data);
}

// devuelve la infromacion de un usuario en un string a un struct
LoginData *utils_destruct_data_search(char *tramaDatos) {
    char delim[] = "*";
    char *ptr = strtok(tramaDatos, delim);

    LoginData *loginData;
    loginData = malloc(sizeof(LoginData));

    while (ptr != NULL) {
        loginData->nombre = strdup(ptr);
        ptr = strtok(NULL, delim);
        loginData->id = atoi(ptr);
        ptr = strtok(NULL, delim);
        loginData->codigoPostal = strdup(ptr);
        ptr = strtok(NULL, delim);
    }

    return loginData;
}

// se encarga de construir una trama con la informacion base
void sendTrama(char * data, int clientFD) {
    char *tramaRespuesta = NULL;
    tramaRespuesta = utils_obtener_trama('L', data);
    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
    funciones_display("\nSend answer\n\n");
}

// envia por tramas el listado de usuarios
char *utils_crear_data_search(ListadoUsuarios *listadoUsuarios, int clientFD) {
    char *data = "";
    char *userData;
    char *dataAux;
    bool firstSend = true;

    // Usuarios
    for (int i = 0; i < listadoUsuarios->total; ++i) {
        asprintf(&userData, "%s*%d", listadoUsuarios->usuarios[i].nombre, listadoUsuarios->usuarios[i].id);
        asprintf(&dataAux, "%s%s", data, userData);
        if (strlen(dataAux) < TRAMA_DATA_SIZE) {
            if (i == listadoUsuarios->total - 1) {
                asprintf(&data, "%s*%s", data, userData);
                if (firstSend) {
                    asprintf(&dataAux, "%d%s", listadoUsuarios->total, data);
                    sendTrama(dataAux, clientFD);
                    firstSend = false;
                } else {
                    sendTrama(data, clientFD);
                }
            } else {
                if (!firstSend && strcmp(data, "") == 0) {
                    asprintf(&data, "%s%s", data, userData);
                } else {
                    asprintf(&data, "%s*%s", data, userData);
                }
            }
        } else {
            if (firstSend) {
                asprintf(&dataAux, "%d%s", listadoUsuarios->total, data);
                sendTrama(dataAux, clientFD);
                firstSend = false;
            } else {
                sendTrama(data, clientFD);
            }
            data = "";
            i--;
        }
    }


    return data;
}

// gestiona la salida de un cliente del servidor
void utils_salir() {
    close(listenFD);
    signal(SIGINT, SIG_DFL);
    raise(SIGINT);
}

// se encarga de crear sesiones por cada cliente
int utils_gestor_de_sockets(Configuracion config) {
    int clientFD;
    struct sockaddr_in servidor;

    signal(SIGINT, utils_salir);

    if ((listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_port = htons(config.puerto);
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

// Envia una trama al servidor en caso de que el md5sum del fichero descargado y el del servidor sean identicos
void utils_comparar_md5sum(int clientFD, char *trama, FotoData *fotoData, char *fileName) {
    char *md5File = funciones_generate_md5sum(fileName);
    if (strcmp(fotoData->md5sum, md5File) == 0) {
        trama = utils_obtener_trama('I', "IMAGE OK");
        write(clientFD, trama, MAX_TRAMA_SIZE);
    }
}