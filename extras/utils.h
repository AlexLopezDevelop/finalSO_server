//
// Created by Alex Lopez on 20/11/21.
//

#ifndef FINALSO_SERVER_UTILS_H
#define FINALSO_SERVER_UTILS_H

#include "usuario.h"
#include "../modelos/conexion.h"

void * comprobarNombres(void * arg);

void salir();

void gestorDeSockets();

char * concatenarStrings(char **instruccion);

char * crearDataSearch(ListadoUsuarios * listadoUsuarios);

LoginData *destructDataSearch(char *tramaDatos);

char *obtenerTrama(char tipo, char *data);

LoginData *destructData(char *datos);

ConexionData *guardarTrama(const char *trama);

#endif //FINALSO_SERVER_UTILS_H
