
#include "funciones_plataforma_dependiente.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void registrar_error(const char* mensaje, const char* nombre_archivo) {
    // Registra los errores en un archivo log.
    FILE* archivo;
    if (abrir_archivo(nombre_archivo, "a", &archivo) != 0) {
        perror("Error abriendo el archivo de errores");
        return;
    }


    time_t tiempo_actual;
    char* tiempo_actual_string;

    tiempo_actual = time(NULL);
    tiempo_actual_string = obtener_tiempo_string(&tiempo_actual);


    // Check if ctime returned NULL
    if (tiempo_actual_string == NULL) {
        fprintf(archivo, "[Fecha desconocida] Error: %s\n", mensaje);
    }
    else {
        fprintf(archivo, "[%s] Error: %s\n", tiempo_actual_string, mensaje);
    }

    fclose(archivo);
}

