//
// Created by Alex Lopez on 20/11/21.
//

#ifndef FINALSO_CONEXION_H
#define FINALSO_CONEXION_H

#include "configuracion.h"

typedef struct {
    char origen[TRAMA_ORIGEN_SIZE];
    char tipo;
    char datos[TRAMA_DATA_SIZE];
} ConexionData;

#endif //FINALSO_CONEXION_H
