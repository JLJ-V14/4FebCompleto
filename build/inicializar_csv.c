/*En este archivo se encuentran las funciones relacionadas con
  la inicializacion de las variables que almacenan los datos leidos
  sin procesar de los csvs de entrada */

#include "definiciones_globales.h"
#include "inicializar_posiciones.h"
#include "tipos_optimizacion.h"
#include "registrar_errores.h"
#include <stdio.h>


  /*Se inicializa el puntero a los datos a NULL*/
static void inicializar_datos_null(datos_csv_t* datos_csv) {
    datos_csv->datos = NULL;
}

/*Se inicializa el numero de filas y columnas de la
    variable que almacena el csv a 0 */
static void inicializar_numero_filas_columnas(datos_csv_t* datos_csv) {

    datos_csv->filas = 0;
    datos_csv->columnas = 0;
}


/*Se reserva espacio en memoria para las variables que
 almacenan los datos de los CSVs */
int inicializar_csvs(datos_csv_t* datos_csv[], int num_csvs) {

    

    for (int i = 0; i < num_csvs; i++) {

        

        if ((datos_csv[i]) == NULL) {
            printf("Problema en el csv numero%d\n", i);
            printf("El puntero es null\n");
            printf("Error reservando memoria\n");
            char mensaje_error[512];
            snprintf(mensaje_error, sizeof(mensaje_error), "Problema en el csv numero %d error reservando la memoria",
                i);
            registrar_error(mensaje_error, REGISTRO_ERRORES);
            return ERROR;
        }

        inicializar_numero_filas_columnas(datos_csv[i]);
        inicializar_datos_null(datos_csv[i]);

    }


    return EXITO;
}

/*Se inicializa las variables que contienen la informacion
de entrada del algoritmo. Tambien se reserva espacio en
memoria para las mismas*/

int inicializar_informacion_entrada(informacion_entrada_t* informacion_sistema) {

    //Se llama al subprograma que se encargan de almacenar las columnas y filas
    //en las cuales se encuentra la informacion
    indicar_posiciones_informacion(informacion_sistema);

    datos_csv_t* datos_algoritmo = &(informacion_sistema->datos_algoritmo.informacion_algoritmo);
    datos_csv_t* datos_vehiculos = &(informacion_sistema->datos_vehiculos.informacion_vehiculos);
    datos_csv_t* datos_baterias = &(informacion_sistema->datos_baterias.informacion_baterias);
    datos_csv_t* datos_restricciones = &(informacion_sistema->datos_restricciones.informacion_restricciones);
    datos_csv_t* datos_precio_compra = &(informacion_sistema->datos_precio_compra.informacion_precio);
    datos_csv_t* datos_precio_venta = &(informacion_sistema->datos_precio_venta.informacion_precio);
    datos_csv_t* datos_terminales = &(informacion_sistema->datos_terminales.informacion_terminales);



    datos_csv_t* datos_csvs[] = { datos_algoritmo, datos_vehiculos, datos_baterias,
                                  datos_restricciones, datos_precio_compra,
                                  datos_precio_venta, datos_terminales };


    /*Se reserva memoria para las variables que almacenan la informacion sin procesar de
      los csvs.*/
    int numero_csvs = sizeof(datos_csvs) / sizeof(datos_csvs[0]);





    if (inicializar_csvs(datos_csvs, numero_csvs) == ERROR) {
        printf("No se ha podido inicializar  los Datos de los Csv \n");
        registrar_error("No se ha podido reservar memoria para los Datos de los Csv", REGISTRO_ERRORES);
        return ERROR;
    }
    return EXITO;
}
