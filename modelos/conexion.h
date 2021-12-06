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

typedef struct {
    char * nombre;
    char * codigoPostal;
    int id;
} LoginData;

typedef struct {
    LoginData * registrados;
    int totalRegistrados;
    LoginData * conectados;
    int totalConectados;
} Usuarios;

typedef struct {
    LoginData * usuarios;
    int total;
} ListadoUsuarios;

#endif //FINALSO_CONEXION_H