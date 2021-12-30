//
// Created by Alex Lopez on 12/30/2021.
//

#ifndef FINALSO_SERVER_FOTO_H
#define FINALSO_SERVER_FOTO_H

typedef struct {
    char * nombre;
    int size;
    int totalTramas;
    char * md5sum;
    char **tramas;
} FotoData;

#endif //FINALSO_SERVER_FOTO_H
