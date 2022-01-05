//
// Created by Alex Lopez on 26/11/21.
//

#include "menu.h"
#include "utils.h"
#include "funciones.h"

void *utils_comprobar_nombres(void *arg) {
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
        conexionData = utils_guardar_trama(trama);
        switch (trama[15]) {
            case 'C':   //Login
                funciones_display("\n\nReceived login ");
                LoginData *loginData = utils_destruct_data(conexionData->datos);
                sprintf(print, "%s %s\n", loginData->nombre, loginData->codigoPostal);
                funciones_display(print);

                loginData->id = usuario_obtener_id(loginData);

                // revisar si no existe, no tiene id
                if (loginData->id == 0) {
                    usuario_registrar(loginData);
                }

                sprintf(idString, "%d", loginData->id);
                sprintf(print, "Assigned ID %s.\n", idString);
                funciones_display(print);
                tramaRespuesta = utils_obtener_trama('O', idString);
                write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                funciones_display("Send answer\n\n");
                break;
            case 'S':  //search
                if (!usuario_existe(loginData)) {
                    tramaRespuesta = utils_obtener_trama('K', "0");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    break;
                }
                funciones_display("Buscando usuarios\n");
                char *data = menu_opcion_buscar_usuario(conexionData);
                if (data[0] != '0') {
                    tramaRespuesta = utils_obtener_trama('L', data);
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    funciones_display("\nSend answer\n\n");
                } else {
                    tramaRespuesta = utils_obtener_trama('K', data);
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                }

                break;
            case 'F': //send
                funciones_display("Rebut send ");

                fotoData = utils_destruct_data_imagen(conexionData->datos);
                char *printf;
                asprintf(&printf, "%s de %s %d\n", fotoData->nombre, loginData->nombre, loginData->id);
                funciones_display(printf);

                fotoData->totalTramas = fotoData->size / TRAMA_DATA_SIZE;
                if (fotoData->size % TRAMA_DATA_SIZE != 0) {
                    fotoData->totalTramas++;
                }

                int fd;
                bool error = false;

                char *imageName;
                asprintf(&imageName, "%d.jpg", loginData->id);
                fd = open(imageName, O_WRONLY | O_CREAT | O_TRUNC, 00666);

                if (funciones_error_abrir(fd)) {
                    error = true;
                }
                //funciones_liberar_memoria(trama);
                break;
            case 'D': ;

                if (fotoData->size % TRAMA_DATA_SIZE != 0 && (fotoData->totalTramas-1) == i) {
                    write(fd, conexionData->datos, sizeof (char) * (fotoData->size % TRAMA_DATA_SIZE));
                    i=0;
                } else {
                    write(fd, conexionData->datos, sizeof (char) *TRAMA_DATA_SIZE);
                    i++;
                }

                if (i == fotoData->totalTramas-1 && !error) {
                    tramaRespuesta = utils_obtener_trama('I', "IMATGE OK");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    asprintf(&printf, "Guardada com %d.jpg\n\n", loginData->id);
                    funciones_display(printf);
                    close(fd);
                } else if (i == fotoData->totalTramas-1 && error) {
                    tramaRespuesta = utils_obtener_trama('R', "IMATGE KO");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    funciones_display("Error File not found\n");
                }




                break;
            case 'P': //photo
                funciones_display("recieved PHOTO download request\n");
                char sizeFileString[100];

                char *imagePath;
                asprintf(&imagePath, "%s.jpg", conexionData->datos);

                int sizeFile = funciones_get_file_size(imagePath);
                sprintf(sizeFileString, "%d", sizeFile);
                funciones_display(sizeFileString);

                char *md5File = funciones_generate_md5sum(imagePath);

                char *dataImage;
                asprintf(&dataImage, "%s*%s*%s", imagePath, sizeFileString, md5File);

                tramaRespuesta = utils_obtener_trama('P', dataImage);
                write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);

                funciones_send_image(clientFD, imagePath);

                break;
            case 'Q':   //logout
                if (!usuario_existe(loginData)) {
                    // TODO: enviar error al cliente y romper el fujo (return/break)
                }
                usuario_mensaje_desconectado(conexionData->datos);
                funciones_display("Cliente Desconectado!\n\n");
                close(clientFD);
                salir = 1;//Logout
                break;

        }

        funciones_liberar_memoria(trama);
    }

    pthread_cancel(pthread_self());
    pthread_detach(pthread_self());

    return NULL;
}

char *menu_opcion_buscar_usuario(ConexionData *conexionData) {
    LoginData *loginData = utils_destruct_data_search(conexionData->datos);
    ListadoUsuarios *listadoUsuarios = usuario_buscar(loginData);
    char print[200];
    char auxid[30];

    sprintf(print, "Received search %s de %s %d\nFeta la cerca\n", loginData->codigoPostal, loginData->nombre,
            loginData->id);
    funciones_display(print);

    if (listadoUsuarios->total == 0) {
        funciones_display("No hay ningun usuario con el codigo postal ");
        funciones_display(loginData->codigoPostal);
        funciones_display("\n");
    } else {
        sprintf(print, "Hi han %d persones humanes a %s\n", listadoUsuarios->total, loginData->codigoPostal);
        funciones_display(print);
        funciones_display("\n");

        for (int i = 0; i < listadoUsuarios->total; ++i) {
            sprintf(auxid, "%d", listadoUsuarios->usuarios[i].id);
            sprintf(print, "%s ", auxid);
            funciones_display(print);
            funciones_display(listadoUsuarios->usuarios[i].nombre);
            funciones_display("\n");
        }
    }

    return utils_crear_data_search(listadoUsuarios);
}