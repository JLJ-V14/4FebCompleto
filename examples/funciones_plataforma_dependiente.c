#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * En este archivo se definen las funciones que cambian en función de si el sistema en el que
 * están siendo ejecutado el código es tipo Windows o Linux.
 */

 /*
  * Función que sirve para implementar la función wcsdup
  * y que funcione en Windows o Linux. Esta función sirve
  * para duplicar un string.
  */


  // Adjusted typedef for clarity and uniform behavior across platforms

char* strok_seguro(char* str, const char* delimitador, char** token) {

#ifdef _WIN32
  strtok_s(str, delimitador, token);
  if (*token != NULL) {
   // printf("Token: %s\n", *token); // Ensure this is safe to print
  }
  else {
    //printf("H\n"); // Indicate no more tokens
    return NULL;
  }
#else
  char* result = strtok_r(str, delimitador, token);
  if (result != NULL) {
    printf("Token: %s\n", result);
}
  else {
    printf("H\n"); // POSIX path no more tokens
    return NULL;
  }
#endif
  return *token;
}

/*
  * Función que sirve para implementar la función wcstok
  * y que funcione en Windows o Linux. Esta función sirve
  * para duplicar un string.
  */

char* strdup_plataforma(const char* src) {
#ifdef _WIN32
    return _strdup(src);
#else
    return strdup(src);
#endif
}

/*
  * Función que sirve para implementar la función wcstok
  * y que funcione en Windows o Linux. Esta función sirve
  * para abrir un archivo.
  */

int abrir_archivo(const char* nombre_archivo, const char* modo, FILE** archivo) {
#ifdef _WIN32
    return fopen_s(archivo, nombre_archivo, modo);
#else
    * archivo = fopen(nombre_archivo, modo);
    return (*archivo) ? 0 : errno;  // Retorna 0 en caso de  exito, codigo de error con facraso
#endif
}

/*
  * Función que sirve para implementar la función wcstok
  * y que funcione en Windows o Linux. Esta función sirve
  * para imprimir un codigo de error por pantalla
  */

void imprimir_error(int codigo_error) {
#ifdef _WIN32
    char buffer[256];
    strerror_s(buffer, sizeof(buffer), codigo_error);
    fprintf(stderr, "Error: %s\n", buffer);
#else
    fprintf(stderr, "Error: %s\n", strerror(codigo_error));
#endif
}

/*
  * Función que sirve para obtener el dato de tiempo
  * en formato string.
  */


const char* obtener_tiempo_string(time_t* tiempo) {
    static char buffer[26]; // ctime_s recommends a buffer of at least 26 bytes.

#ifdef _WIN32
    errno_t err = ctime_s(buffer, sizeof(buffer), tiempo);
    if (err) {
        return NULL;
    }
    return buffer;
#else
    return ctime(tiempo);  // ctime returns a static buffer on non-Windows platforms.
#endif
}