#include "modelos/configuracion.h"
#include "extras/funciones.h"
#include "extras/ficheros.h"
#include "extras/utils.h"

#define printF(x) write(1, x, strlen(x))

int main(int argc, char *argv[]) {

    Configuracion configAtreides;

    errorArgumentos(argc, argv, 2);

    display("SERVIDOR ATREIDES\n");

    if (leerFichero(argv[1], &configAtreides)) {
        return 1;
    }

    gestorDeSockets();

    return 0;
}