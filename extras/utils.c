//
// Created by Alex Lopez on 13/11/21.
//

#include "../extras/funciones.h"
#include "../modelos/configuracion.h"
#include "../modelos/conexion.h"

#define MAX_TRAMA_SIZE 256
#define printF(x) write(1, x, strlen(x))

int listenFD;

ConexionData * guardarTrama(char * trama) {
    ConexionData * conexionData;
    conexionData = malloc(sizeof (ConexionData));

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

void *comprobarNombres(void *arg) {
    int clientFD = *(int *) arg;
    int salir=0;
    ConexionData * conexionData;
    conexionData = malloc(sizeof (ConexionData));

    while (salir == 0) {
        char trama[MAX_TRAMA_SIZE];
        read(clientFD, trama, MAX_TRAMA_SIZE);

        conexionData = guardarTrama(trama);

        switch (trama[15]) {
            case 'C':   //Login
                display("Received login ");
                display(conexionData->datos);
                display("\n");
                break;
            case 'S':   //search
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