cmake_minimum_required(VERSION 3.20)
project(finalSO_server C)

set(CMAKE_C_STANDARD 99)

set(Headers extras/ficheros.h extras/funciones.h extras/utils.h modelos/configuracion.h modelos/conexion.h extras/menu.h extras/usuario.h)

add_executable(finalSO_server main.c extras/ficheros.c extras/funciones.c extras/utils.c extras/menu.c extras/usuario.c)
