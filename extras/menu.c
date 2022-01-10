//
// Created by Alex Lopez on 26/11/21.
//

#include "menu.h"
#include "utils.h"
#include "funciones.h"
#include <stdbool.h>

// se le pasa una trama y la devuelve la informacion en un struct
ConexionData *ficheros_guardar_trama(const char *trama) {
    ConexionData *conexionData;
    conexionData = malloc(sizeof(ConexionData));

    // obtener origen
    for (int i = 0; i < TRAMA_ORIGEN_SIZE; ++i) {
        conexionData->origen[i] = trama[i];
        if (trama[i] == '\0') {
            break;
        }
    }

    // obtener tipo
    conexionData->tipo = trama[TRAMA_ORIGEN_SIZE];

    // obtener data
    int dataIndex = 0;

    for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; ++i) {
        conexionData->datos[dataIndex] = trama[i];
        dataIndex++;
    }

    return conexionData;
}

// se encarga de recibir las peticiones del cliente y ejecutar dicha funcionalidad en caso de que sea correcta
void *menu_comprobar_nombres(void *arg) {
    int clientFD = *(int *) arg;
    int salir = 0;
    char idString[30];
    char print[100];
    ConexionData *conexionData;
    char *tramaRespuesta = NULL;
    char *trama = NULL;
    int i = 0;

    FotoData *fotoData = malloc(sizeof(FotoData));
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

                //funciones_liberar_memoria(loginData->nombre);
                //funciones_liberar_memoria(loginData->codigoPostal);
                //funciones_liberar_memoria(loginData);
                break;
            case 'S':  //search
                if (!usuario_existe(loginData)) {
                    tramaRespuesta = utils_obtener_trama('K', "0");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    break;
                }

                funciones_display("Buscando usuarios\n");
                ListadoUsuarios *listadoUsuarios = menu_opcion_buscar_usuario(conexionData);
                if (listadoUsuarios->total != 0) {
                    utils_crear_data_search(listadoUsuarios, clientFD);
                } else {
                    tramaRespuesta = utils_obtener_trama('K', "0");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                }

                break;
            case 'F': //send
                funciones_display("Recieved send ");

                fotoData = utils_destruct_data_imagen(conexionData->datos);
                char *printf;
                asprintf(&printf,"%s", fotoData->nombre);
                funciones_display(printf);
                asprintf(&printf," de %s ", loginData->nombre);
                funciones_display(printf);
                asprintf(&printf,"%d\n", loginData->id);
                funciones_display(printf);

                bool descargandoImagen = true;
                char tramaImagen[MAX_TRAMA_SIZE];

                fotoData->totalTramas = fotoData->size / TRAMA_DATA_SIZE;

                // revisamos si tiene una trama de menos de TRAMA_DATA_SIZE le sumamos una trama mas a recibir
                if (fotoData->size % TRAMA_DATA_SIZE != 0) {
                    fotoData->totalTramas++;
                }

                char *imageName;
                asprintf(&imageName, "%d.jpg", loginData->id);

                // en caso de existir ya la imagen borra la antigua
                remove(imageName);

                // mientras no se reciban todas las tramas
                while (descargandoImagen) {
                    memset(tramaImagen, 0, TRAMA_DATA_SIZE);
                    memset(conexionData, 0, sizeof(ConexionData));

                    read(clientFD, tramaImagen, MAX_TRAMA_SIZE);
                    conexionData = ficheros_guardar_trama(tramaImagen);

                    int fd;

                    fd = open(imageName, O_WRONLY | O_CREAT | O_APPEND, 00666);

                    if (funciones_error_abrir(fd)) {
                        funciones_display("Error al guardar la imagen\n");
                    }

                    if (fotoData->size % TRAMA_DATA_SIZE != 0 && (fotoData->totalTramas-1) == i) {
                        write(fd, conexionData->datos, sizeof(char) * (fotoData->size % TRAMA_DATA_SIZE));
                        i = 0;
                        descargandoImagen = false;
                        utils_comparar_md5sum(clientFD, trama, fotoData, imageName);
                    } else {
                        if ((fotoData->totalTramas-1) == i) {
                            descargandoImagen = false;
                            utils_comparar_md5sum(clientFD, trama, fotoData, imageName);
                        }
                        write(fd, conexionData->datos, sizeof(char) * TRAMA_DATA_SIZE);
                        i++;
                    }

                    close(fd);
                }

                funciones_display("Saved as ");
                funciones_display(imageName);
                funciones_display("\n\n");
                break;
            case 'P': //photo
                funciones_display("Recieved photo ");
                char *printphoto;
                asprintf(&printphoto, "%s de %s %d\n", conexionData->datos, loginData->nombre, loginData->id);
                funciones_display(printphoto);

                char sizeFileString[100];

                char *imagePath;
                asprintf(&imagePath, "%s.jpg", conexionData->datos);

                int picture;

                picture = open(imagePath, O_RDONLY);

                //check if file exist
                if (funciones_error_abrir(picture)) {
                    funciones_display("Image not found\n\n");

                    tramaRespuesta = utils_obtener_trama('F', "FILE NOT FOUND");
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);

                } else {
                    int sizeFile = funciones_get_file_size(imagePath);
                    sprintf(sizeFileString, "%d", sizeFile);

                    char *md5File = funciones_generate_md5sum(imagePath);

                    char *dataImage;
                    asprintf(&dataImage, "%s*%s*%s", imagePath, sizeFileString, md5File);

                    tramaRespuesta = utils_obtener_trama('F', dataImage);
                    write(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);

                    funciones_send_image(clientFD, imagePath);

                    read(clientFD, tramaRespuesta, MAX_TRAMA_SIZE);
                    if (tramaRespuesta[15] == 'I') {
                        funciones_display("Foto enviada amb èxit a Fremen.\n\n");
                    } else if (tramaRespuesta[15] == 'R') {
                        funciones_display("Error foto no enviada a Fremen.\n\n");
                    }

                    //funciones_liberar_memoria(imagePath);
                    //funciones_liberar_memoria(md5File);
                    //funciones_liberar_memoria(dataImage);
                }

                break;
            case 'Q':   //logout
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


    funciones_liberar_memoria(conexionData);
    funciones_liberar_memoria(tramaRespuesta);
    funciones_liberar_memoria(trama);
    funciones_liberar_memoria(fotoData->nombre);
    funciones_liberar_memoria(fotoData->md5sum);
    return NULL;
}

// muestra los usuarios con el mismo codigo postal
ListadoUsuarios *menu_opcion_buscar_usuario(ConexionData *conexionData) {
    LoginData *loginData = utils_destruct_data_search(conexionData->datos);
    ListadoUsuarios *listadoUsuarios = usuario_buscar_registrados(loginData);

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

    return listadoUsuarios;
}