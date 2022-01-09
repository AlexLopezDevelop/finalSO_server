//
// Created by Alex Lopez on 20/11/21.
//

#ifndef FINALSO_SERVER_UTILS_H
#define FINALSO_SERVER_UTILS_H

#include "usuario.h"
#include "../modelos/conexion.h"
#include "../modelos/foto.h"

void utils_salir();

int utils_gestor_de_sockets(Configuracion config);

char * utils_concatenar_strings(char **instruccion);

char * utils_crear_data_search(ListadoUsuarios * listadoUsuarios, int clientFD);

FotoData *utils_destruct_data_imagen(char *datos);

LoginData *utils_destruct_data_search(char *tramaDatos);

char *utils_obtener_trama(char tipo, char *data);

LoginData *utils_destruct_data(char *datos);

ConexionData *utils_guardar_trama(const char *trama);

void utils_comparar_md5sum(int clientFD, char *trama, FotoData *fotoData, char * imageName);

#endif //FINALSO_SERVER_UTILS_H
