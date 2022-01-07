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

    funciones_liberar_memoria(ptr);

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

char *utils_obtener_trama(char tipo, char *data) {
    return crearTrama("ATREIDES", tipo, data);
}

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

char *utils_crear_data_search(ListadoUsuarios *listadoUsuarios) {
    char *data;
    char *newdata;

    asprintf(&data, "%d*", listadoUsuarios->total);

    // Usuarios
    if (listadoUsuarios->total != 0) {
        for (int i = 0; i < listadoUsuarios->total; ++i) {
            if (i == listadoUsuarios->total-1){
                asprintf(&newdata, "%s*%d", listadoUsuarios->usuarios[i].nombre,listadoUsuarios->usuarios[i].id);
                asprintf(&data,"%s%s",data,newdata);
            } else {
                asprintf(&newdata, "%s*%d*", listadoUsuarios->usuarios[i].nombre,listadoUsuarios->usuarios[i].id);
                asprintf(&data,"%s%s",data,newdata);
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


void utils_comparar_md5sum(int clientFD, char *trama, FotoData *fotoData, char * fileName) {
    char *md5File = funciones_generate_md5sum(fileName);
    if (strcmp(fotoData->md5sum, md5File) == 0) {
        trama = utils_obtener_trama('I', "IMAGE OK");
        write(clientFD, trama, MAX_TRAMA_SIZE);
    }
}