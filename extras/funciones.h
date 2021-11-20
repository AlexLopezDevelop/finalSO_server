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


#define FILE1 "Config.dat"

int errorArgumentos(int argc, char *argv[], int num_argumentos);

void display(char *string);

int checkEOF(int fd);

void liberarMemoria(void *ptr);

char *readLineFile(int fd, char hasta);

int errorAbrir(int fd, char *nombre_f);

void readInput(char **string);

char * readStringTo (char * string, char hasta);

char * concatStringsPorAsterico(char * string1, char * string2);

#endif //PRACTICAFINALSO_FUNCIONES_H
