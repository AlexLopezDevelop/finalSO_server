//
// Created by joanb on 14/10/2021.
//

#include "funciones.h"

void liberarMemoria(void *ptr) {
    // Libermaos Memoria
    free(ptr);
    // Dejamos apuntando a NULL
    ptr = NULL;
}
void display(char *string) {
    write(1, string, sizeof(char) * strlen(string));
}

char * readStringTo (char * string, char hasta) {
    int i = 0;
    char *aux = NULL;
    char caracter = '\0';

    int lenString = strlen(string);
    for (int j = 0; j < lenString || caracter != hasta; ++j) {
        caracter = string[j];

        if (caracter != hasta) {
            aux = (char *) realloc(aux, i + 1);
            aux[i] = caracter;
            i++;
        } else {
            aux[j] = '\0';
            return aux;
        }

    }

    return aux;
}

char * concatStringsPorAsterico(char * string1, char * string2) {
    char * concatString = NULL;
    int stringSize = strlen(string1) + strlen(string2) + 1;
    concatString = malloc(stringSize * sizeof (char));

    strcpy(concatString, string1);
    strcat(concatString, "*");
    strcat(concatString, string2);

    return concatString;
}

int errorArgumentos(int argc, char *argv[], int num_argumentos) {

    if (argc != num_argumentos) {
        display("\nERROR en el numero de ficheros\n");
        return 1;
    } else {
        if (strcmp(argv[1], FILE1) != 0) {
            display("\nERROR, el fichero no es correcto\n");
            return 1;
        } else {
            display("\nFicheros recibidos correctamente\n");
            return 0;
        }
    }
}

int errorAbrir(int fd, char *nombre_f) {
    char aux[200];

    if (fd < 0) {
        sprintf(aux, "\nERROR al abrir el fichero\n");
        display(aux);
        return 1;
    } else {
        sprintf(aux, "\nEl fichero: %s se ha abierto correctamente\n", nombre_f);
        display(aux);
        return 0;
    }
}

void readInput(char **string) {
    int i = 0;
    char caracter = ' ';

    // Bucle para leer caracteres entrados por teclado hasta Enter
    while (caracter != '\n') {
        // Lectura de caracter por caracter
        read(0, &caracter, sizeof(char));

        // TODO: Revisar esta ðŸ’©
        // Redimensiona la cadena por cada caracter del usuario
        *string = (char *) realloc(*string, (sizeof(char) * (i + 1)));

        // Asigna el caracter a la cadena
        (*string)[i] = caracter;

        // Cuando el caracter sea un salto de linea (enter) marcamos el fianl de la cadena
        if (caracter == '\n') {
            (*string)[i] = '\0';
        }
        i++;
    }
}

char *readLineFile(int fd, char hasta) {
    int i = 0;
    char *aux = NULL;
    char caracter = '\0';

    // Bucle para almacenar la cadena hasta el caracter delimitador o cuando sea EOF
    while (caracter != hasta && !checkEOF(fd)) {

        // Leemos un solo caracter (1byte)
        read(fd, &caracter, sizeof(char));

        // Si el caracter es valido reasignamos espacio en la memoria y lo guardamos en la cadena
        if (caracter != hasta) {
            aux = (char *) realloc(aux, i + 1);
            aux[i] = caracter;
            i++;
        }

    }
    // Marcamos el final de la cadena y lo copiamos
    aux[i] = '\0';

    return aux;
}

int checkEOF(int fd) {
    int num_bytes;
    char car;

    // Controlamos cuantos bytes retorna la funcion read()
    num_bytes = read(fd, &car, sizeof(char));

    // Si retorna 0bytes porque no hay mas caracteres es el End Of File
    if (num_bytes == 0) {
        return 1;
    }

    // Reposicionamos el puntero a la posicion que acaba de leer
    lseek(fd, -1, SEEK_CUR);

    return 0;
}


