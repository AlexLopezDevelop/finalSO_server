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

int errorAbrir(int fd) {
    char aux[200];

    if (fd < 0) {
        sprintf(aux, "\nERROR al abrir el fichero\n");
        display(aux);
        return 1;
    } else {
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
    int i = 0, size;
    char c = '\0';
    char *string = (char *) malloc(sizeof(char));

    while (1) {
        size = read(fd, &c, sizeof(char));

        if (c != hasta && size > 0) {
            string = (char *) realloc(string, sizeof(char) * (i + 2));
            string[i++] = c;
        } else {
            break;
        }

    }

    string[i] = '\0';

    return string;
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

int getFileSize(char *fileName) {
    struct stat sb;

    if (stat(fileName, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    return sb.st_size;
}

char *generateMd5sum(char *string) {
    char *args[] = {"md5sum", string, 0};
    int fd = open(MD5FILE, O_CREAT | O_WRONLY, S_IRWXU);
    pid_t pid = fork();

    if (!pid) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execvp(args[0], args);
    }

    fd = open(MD5FILE, O_RDONLY);

    char *md5String = malloc(sizeof(char) * 33);

    if (errorAbrir(fd)) {
        return md5String;
    }

    strcpy(md5String, readLineFile(fd, ' '));

    close(fd);

    return md5String;
}

int sendImage(int socket, char *fileName) {
    int picture;

    picture = open(fileName, O_RDONLY);

    if (errorAbrir(picture)) {
        display("Error Opening Image File");
        return 1;
    }

    char c[TRAMA_DATA_SIZE];

    while (!checkEOF(picture)) {
        memset(c, 0, TRAMA_DATA_SIZE);
        read(picture, &c, sizeof(char) * TRAMA_DATA_SIZE);

        char *trama = obtenerTrama('F', c);
        write(socket, trama, MAX_TRAMA_SIZE);
        usleep(200);
    }

    close(picture);

    display("Foto Enviada");
    return 0;
}

