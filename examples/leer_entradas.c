
#include <errno.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones_globales.h"
#include "funciones_plataforma_dependiente.h"
#include "leer_entradas.h"
#include "liberar_memoria.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"



#define  MAX_LONGITUD_LINEA 1024
#define  MAX_NUM_FILAS_INICIAL 10
#define  DELIMITADOR ","
#define  MAX_COLUMNS 27

//Se define el numero de csvs que se tiene.
#define NUM_CSVS (sizeof(csvs) / sizeof(csvs[0]))


static int leer_csv(const char* nombre_archivo, char*delimitador, datos_csv_t* datos_csv) {

    char delimitador_str[] = ",";
    FILE* archivo;
    int resultado = abrir_archivo(nombre_archivo, "r", &archivo);
    if (!archivo) {
        char mensaje_error[256];
        snprintf(mensaje_error, sizeof(mensaje_error), "No se ha podido abrir el archivo %s", nombre_archivo);
        printf("No se ha podido abrir el archivo %s\n", nombre_archivo);
        registrar_error(mensaje_error, REGISTRO_ERRORES);
        return ERROR;
    }

    char bom[4] = { 0 };
    if (fgets(bom, sizeof(bom), archivo)) {
      if (strncmp(bom, "\xEF\xBB\xBF", 3) != 0) {
        // No BOM found, rewind to start
        fseek(archivo, 0, SEEK_SET);
      }
    }


    int numero_lineas = 0;
    int numero_columnas = 0;
    datos_csv->filas = 0;
    datos_csv->columnas = 0;

    char linea[MAX_LONGITUD_LINEA];
    char* next_token = NULL; // Declaration added here
    int i = 0;

    int capacidad = MAX_NUM_FILAS_INICIAL;
    datos_csv->datos = malloc(capacidad * sizeof(wchar_t**));
    if (datos_csv->datos == NULL) {
        perror("Error reservando memoria");
        fclose(archivo);
        return ERROR;
    }

    char* tokens[MAX_COLUMNS]; // Se define un array para almacenar la diferente informacion contenida en una línea

    while (fgets(linea, MAX_LONGITUD_LINEA, archivo)) {
        if (linea[0] == '\n') continue;

        if (numero_lineas == capacidad) {
            capacidad *= 2;
            char*** temp = realloc(datos_csv->datos, capacidad * sizeof(char**));
            if (temp == NULL) {
                perror("Error reservando memoria\n");
                liberar_memoria_csv_individual(datos_csv);
                fclose(archivo);
                return ERROR;
            }
            datos_csv->datos = temp;
        }


        int current_columns = 1;

       // printf("%s\n", linea);
        //Se guarda la primera columna
        tokens[0] = linea;
        char* token = strok_seguro(linea, delimitador_str, &next_token);
        //char* token = linea;
       // printf("%s\n", token);
        
        while (token != NULL && token[0] != '\0') {
            
            tokens[current_columns] = token; // Store in temporary array
            //printf("Almacenado en array\n");
            current_columns++;
            token = strok_seguro(NULL, delimitador_str, &next_token);
            //printf("%s\n", token);
        }

        //Se elimina el caracter de nueva linea.
        if (current_columns > 0) {
            char* last_token = tokens[current_columns - 1];
            size_t len = strlen(last_token);
            if (len > 0 && last_token[len - 1] == '\n') {
                last_token[len - 1] = '\0';
            }
        }



        if (current_columns > numero_columnas) {
            numero_columnas = current_columns;
        }

        datos_csv->datos[i] = malloc(numero_columnas * sizeof(wchar_t*));
        if (datos_csv->datos[i] == NULL) {
            perror("Error reservando memoria");
            liberar_memoria_csv_individual(datos_csv);
            fclose(archivo);
            return ERROR;
        }

        for (int j = 0; j < current_columns; j++) {


            datos_csv->datos[i][j] = strdup_plataforma(tokens[j]);

            if (datos_csv->datos[i][j] == NULL) {
                perror("Error reservando memoria\n");
                liberar_memoria_csv_individual(datos_csv);
                fclose(archivo);
                return ERROR;
            }
        }

        // If the current line has fewer columns, set the remaining pointers to NULL
        for (int j = current_columns; j < numero_columnas; j++) {
 
            datos_csv->datos[i][j] = NULL;
         
        }

        numero_lineas++;
        i++;
    }

    datos_csv->filas = numero_lineas;
    datos_csv->columnas = numero_columnas;
    fclose(archivo);
    return EXITO;
}



int leer_entradas(informacion_entrada_t* informacion_sistema) {

    //Este subprograma se utiliza para leer las diferentes
    //datos de entrada contenidos en los archivos csvs.

    entrada_csv csvs[] = {
    {INFORMACION_TERMINALES, &(informacion_sistema->datos_terminales.informacion_terminales)},
    {INFORMACION_VEHICULOS, &(informacion_sistema->datos_vehiculos.informacion_vehiculos)},
    {INFORMACION_BATERIAS, &(informacion_sistema->datos_baterias.informacion_baterias)},
    {INFORMACION_ALGORITMO, &(informacion_sistema->datos_algoritmo.informacion_algoritmo)},
    {INFORMACION_RESTRICCIONES_SISTEMA, &(informacion_sistema->datos_restricciones.informacion_restricciones)},
    {INFORMACION_PRECIO_COMPRA, &(informacion_sistema->datos_precio_compra.informacion_precio)},
    {INFORMACION_PRECIO_VENTA, &(informacion_sistema->datos_precio_venta.informacion_precio)}

    };

    for (int i = 0; i < NUM_CSVS; i++) {

        if (leer_csv(csvs[i].nombre_archivo, DELIMITADOR, csvs[i].datos) == ERROR) {

            return ERROR;
        }
    }


    return EXITO;



}