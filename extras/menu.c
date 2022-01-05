//
// Created by Alex Lopez on 26/11/21.
//

#include "menu.h"
#include "utils.h"
#include "funciones.h"

void *comprobarNombres(void *arg) {
    int clientFD = *(int *) arg;
    int salir = 0;
    char idString[30];
    char print[100];
    ConexionData *conexionData;
    char *tramaRespuesta = NULL;
    char *trama = NULL;
    int i = 0;

    FotoData *fotoData = malloc(sizeof(FotoData));
    fotoData->tramas = malloc(sizeof(char *));
    fotoData->sizeTrama = 0;
    fotoData->totalTramas = 0;

    while (salir != 1) {

        trama = malloc(sizeof(char) * MAX_TRAMA_SIZE);

        read(clientFD, trama, MAX_TRAMA_SIZE);
        conexionData = guardarTrama(trama);
        switch (trama[15]) {
            case 'C':   //Login
                display("\n\nReceived login ");
                LoginData *loginData = destructData(conexionData->datos);
                sprintf(print, "%s %s\n", loginData->nombre, loginData->codigoPostal);
                display(print);

                loginData->id = obtenerIdUsuario(loginData);

                // revisar si no existe, no tiene id
                if (loginData->id == 0) {
                    registrarUsuario(loginData);
                }

                sprintf(idString, "%d", loginData->id);
                sprintf(print, "Assigned ID %s.\n", idString);
                display(print);
                tramaRespuesta = obtenerTrama('O', idString);
                write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                display("Send answer\n\n");
                break;
            case 'S':  //search
                if (!usuarioExiste(loginData)) {
                    tramaRespuesta = obtenerTrama('K', "0");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    break;
                }
                display("Buscando usuarios\n");
                char *data = opcionBuscarUsuario(conexionData);
                if (data[0] != '0') {
                    tramaRespuesta = obtenerTrama('L', data);
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    display("\nSend answer\n\n");
                } else {
                    tramaRespuesta = obtenerTrama('K', data);
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                }

                break;
            case 'F': //send
                display("Rebut send ");

                fotoData = destructDataImagen(conexionData->datos);
                char *printf;
                asprintf(&printf, "%s de %s %d\n", fotoData->nombre, loginData->nombre, loginData->id);
                display(printf);

                fotoData->totalTramas = fotoData->size / TRAMA_DATA_SIZE;
                if (fotoData->size % TRAMA_DATA_SIZE != 0) {
                    fotoData->totalTramas++;
                }

                int fd;
                bool error = false;

                char *imageName;
                asprintf(&imageName, "%d.jpg", loginData->id);
                fd = open(imageName, O_WRONLY | O_CREAT | O_TRUNC, 00666);

                if (errorAbrir(fd)) {
                    error = true;
                }
                //liberarMemoria(trama);
                break;
            case 'D': ;

                if (fotoData->size % TRAMA_DATA_SIZE != 0 && fotoData->totalTramas == i) {
                    write(fd, conexionData->datos, sizeof (char) * (fotoData->size % TRAMA_DATA_SIZE));
                    i=0;
                } else {
                    write(fd, conexionData->datos, sizeof (char) *TRAMA_DATA_SIZE);
                    i++;
                }

                if (i == fotoData->totalTramas && !error) {
                    tramaRespuesta = obtenerTrama('I', "IMATGE OK");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    display("IMAGE OK\n");
                    asprintf(&printf, "Guardada com %d.jpg\n\n", loginData->id);
                    display(printf);
                    close(fd);
                } else if (i == fotoData->totalTramas && error) {
                    tramaRespuesta = obtenerTrama('R', "IMATGE KO");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    display("Error File not found\n");
                }




                break;
            case 'P': //photo
                display("recieved PHOTO download request\n");
                char sizeFileString[100];

                char *imagePath;
                asprintf(&imagePath, "%s.jpg", conexionData->datos);

                int sizeFile = getFileSize(imagePath);
                sprintf(sizeFileString, "%d", sizeFile);
                display(sizeFileString);

                char *md5File = generateMd5sum(imagePath);

                char *dataImage;
                asprintf(&dataImage, "%s*%s*%s", imagePath, sizeFileString, md5File);

                tramaRespuesta = obtenerTrama('P', dataImage);
                write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);

                sendImage(clientFD, imagePath);

                break;
            case 'Q':   //logout
                if (!usuarioExiste(loginData)) {
                    // TODO: enviar error al cliente y romper el fujo (return/break)
                }
                mensajeDesconectadoUsuario(conexionData->datos);
                display("Cliente Desconectado!\n\n");
                close(clientFD);
                salir = 1;//Logout
                break;

        }

        liberarMemoria(trama);
    }

    pthread_cancel(pthread_self());
    pthread_detach(pthread_self());

    return NULL;
}

char *opcionBuscarUsuario(ConexionData *conexionData) {
    LoginData *loginData = destructDataSearch(conexionData->datos);
    ListadoUsuarios *listadoUsuarios = buscarUsuarios(loginData);
    char print[200];
    char auxid[30];

    sprintf(print, "Rebut search %s de %s %d\nFeta la cerca\n", loginData->codigoPostal, loginData->nombre,
            loginData->id);
    display(print);

    if (listadoUsuarios->total == 0) {
        display("No hay ningun usuario con el codigo postal ");
        display(loginData->codigoPostal);
        display("\n");
    } else {
        sprintf(print, "Hi han %d persones humanes a %s\n", listadoUsuarios->total, loginData->codigoPostal);
        display(print);
        display("\n");

        for (int i = 0; i < listadoUsuarios->total; ++i) {
            sprintf(auxid, "%d", listadoUsuarios->usuarios[i].id);
            sprintf(print, "%s ", auxid);
            display(print);
            display(listadoUsuarios->usuarios[i].nombre);
            display("\n");
        }
    }

    return crearDataSearch(listadoUsuarios);
}