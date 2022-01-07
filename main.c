#include "modelos/configuracion.h"
#include "extras/funciones.h"
#include "extras/ficheros.h"
#include "extras/utils.h"

#define printF(x) write(1, x, strlen(x))

int main(int argc, char *argv[]) {

    Configuracion configAtreides;

    if(funciones_error_argumentos(argc, argv, 2)) {
        exit(1);
    }

    funciones_display("SERVIDOR ATREIDES\n");

    if (ficheros_leer(argv[1], &configAtreides)) {
        exit(1);
    }

    utils_gestor_de_sockets();

    return 0;
}