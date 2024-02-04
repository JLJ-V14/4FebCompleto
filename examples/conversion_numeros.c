#include "definiciones_globales.h"
#include <stdio.h>
#include <stdlib.h>
#pragma warning(push)
#pragma warning(disable: 6001)  // Disable warning C6001
// En este archivo se incluyen los subprogramas 
// relacionados con convertir caracteres a numeros.

int convertir_a_entero(const char* str, int* num) {
    // Este subprograma se utiliza para convertir un dato de tipo wchar
    // a una variable de tipo int.

    // Comprobación inicial.
    if (str == NULL) {
        return ERROR;
    }

    char* puntero_final;
    *num = strtol(str, &puntero_final, 10);

    // Puntero_Final se utiliza para comprobar que la conversion fue exitosa.
    if (*puntero_final != '\0') {
      return ERROR;
    }
    return EXITO;
}

int convertir_a_decimal(char* str, double* num) {
    // Este subprograma se utiliza para convertir un dato de tipo wchar
    // a una variable de tipo double.

    // Comprobación inicial.
    if (str == NULL) {
        return ERROR;
    }

    char* puntero_final;
    *num = strtod(str, &puntero_final);
 
    // Puntero_Final se utiliza para comprobar que la conversion fue exitosa.
    if (*puntero_final != '\0') {
      return ERROR;
    }
    return EXITO;
}
