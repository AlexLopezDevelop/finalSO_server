//
// Created by joanb on 14/10/2021.
//

#ifndef PRACTICAFINALSO_FUNCIONES_H
#define PRACTICAFINALSO_FUNCIONES_H

// Librerias GENERALES
#define _GNU_SOURCE

#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/wait.h>   // wait
#include <netdb.h>
#include <stdio.h>      // sprintf
#include <unistd.h>     // hauria de ser red write open y close
#include <sys/types.h>  //
#include <fcntl.h>      // Modos de opertura : open(), O_RDONLY
#include <string.h>     // strlen, strcmp
#include <strings.h>
#include <stdlib.h>     // malloc, realloc
#include <time.h>       // fecha del sistema
#include "dirent.h"
#include "errno.h"
#include "stdbool.h"
#include <sys/stat.h>

#include "../modelos/configuracion.h"
#include "../extras//utils.h"


#define FILE1 "Config.dat"
#define MD5FILE "md5sum.txt"

int funciones_error_argumentos(int argc, char *argv[], int num_argumentos);

void funciones_display(char *string);

int funciones_check_eof(int fd);

void funciones_liberar_memoria(void *ptr);

char *funciones_read_line_file(int fd, char hasta);

int funciones_error_abrir(int fd);

void funciones_read_input(char **string);

char *funciones_read_string_to(char *string, char hasta);

char *funciones_concat_strings_por_asterico(char *string1, char *string2);

int funciones_get_file_size(char *fileName);

char *funciones_generate_md5sum(char *string);

int funciones_send_image(int socket, char *fileName);

#endif //PRACTICAFINALSO_FUNCIONES_H
