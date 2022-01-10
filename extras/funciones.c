//
// Created by joanb on 14/10/2021.
//

#include "funciones.h"

// se le pasa un puntero y libera la memoria
void funciones_liberar_memoria(void *ptr) {
    // Libermaos Memoria
    free(ptr);
    // Dejamos apuntando a NULL
    ptr = NULL;
}

// se le pasa un string y se muestra por pantalla
void funciones_display(char *string) {
    write(1, string, sizeof(char) * strlen(string));
}

// devuelve un string hasta un caracter del parametro string
char * funciones_read_string_to (char * string, char hasta) {
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

// comprueba que los argumentos sean correctos
int funciones_error_argumentos(int argc, char *argv[], int num_argumentos) {

    if (argc != num_argumentos) {
        funciones_display("\nERROR en el numero de ficheros\n");
        return 1;
    } else {
        if (strcmp(argv[1], FILE1) != 0) {
            funciones_display("\nERROR, el fichero no es correcto\n");
            return 1;
        } else {
            funciones_display("\nFicheros recibidos correctamente\n");
            return 0;
        }
    }
}

// Verifica si se ha abierto correctamente el fichero
int funciones_error_abrir(int fd) {
    char aux[200];

    if (fd < 0) {
        sprintf(aux, "\nERROR al abrir el fichero\n");
        funciones_display(aux);
        return 1;
    } else {
        return 0;
    }
}

// devuelve un string hasta el caracter de la linea de un fichero
char *funciones_read_line_file(int fd, char hasta) {
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

// Verifica si es el final de un fichero
int funciones_check_eof(int fd) {
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

// devuelve el size de un fichero
int funciones_get_file_size(char *fileName) {
    struct stat sb;

    if (stat(fileName, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    return sb.st_size;
}

// obtiene el md5sum del fichero pasado por argumento
char *funciones_generate_md5sum(char *string) {
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

    if (funciones_error_abrir(fd)) {
        return md5String;
    }

    strcpy(md5String, funciones_read_line_file(fd, ' '));

    close(fd);

    return md5String;
}

// envia una foto por tramas al cliente
int funciones_send_image(int socket, char *fileName) {
    int picture;

    picture = open(fileName, O_RDONLY);

    if (funciones_error_abrir(picture)) {
        funciones_display("Error Opening Image File");
        return 1;
    }

    char c[TRAMA_DATA_SIZE];

    while (!funciones_check_eof(picture)) {
        memset(c, 0, TRAMA_DATA_SIZE);
        read(picture, &c, sizeof(char) * TRAMA_DATA_SIZE);

        char *trama = utils_obtener_trama('F', c);
        write(socket, trama, MAX_TRAMA_SIZE);
        usleep(200);
    }

    close(picture);

    return 0;
}

