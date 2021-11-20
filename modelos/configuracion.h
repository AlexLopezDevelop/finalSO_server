//
// Created by Alex Lopez on 20/11/21.
//

#ifndef FINALSO_SERVER_CONFIGURACION_H
#define FINALSO_SERVER_CONFIGURACION_H

#define MAX_TRAMA_SIZE 256
#define TRAMA_ORIGEN_SIZE 15
#define TRAMA_DATA_SIZE 240

typedef struct {
    char ip[50];
    int puerto;
    char directorio[100];
} Configuracion;

#endif //FINALSO_SERVER_CONFIGURACION_H
