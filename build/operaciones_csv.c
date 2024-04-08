/*
En este archivo se incluyen las funciones
auxiliares relacionadas con el manejo de
los CSVs.
*/

#include "definiciones_globales.h"
#include "operaciones_string.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "locale.h"
#include <stdio.h>
#include <wchar.h>
/*Defino la fila donde se encuentran los ecabezados*/
#define FILA_ENCABEZADOS 0

/*Este subprograma se utiliza para cargar las dimensiones
/* de un csv en una variable.
*/

int  comprobar_dimensiones_csv_fijo(datos_csv_t* datos_csv, const int cantidad_filas_fijas,
	const int  cantidad_columnas_fijas, const char* categoria_csv) {

	//Este subprograma se utiliza para
	//comprobar si las dimensiones de
	//un CSV son correctas, se utiliza
	//para aquellos CSVs en los que las 
	//dimensiones son fijas, y no dependen
	//del numero de vehiculos o baterias o 
	//horas.

	int numero_filas_csv = datos_csv->filas;
	int columnas_filas_csv = datos_csv->columnas;

	if ((datos_csv->filas != cantidad_filas_fijas) ||
		(datos_csv->columnas != cantidad_columnas_fijas)) {

		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error el csv de %s debe tener %d filas y %d columnas y tiene %d filas y %d columnas\n",
			categoria_csv, cantidad_filas_fijas, cantidad_columnas_fijas, numero_filas_csv, columnas_filas_csv);
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);

		return ERROR;
	}

	return EXITO;

}

//Este subprograma se utiliza para
	//comprobar si las dimensiones de 
	//los CSVs de los precios
	// que se leen son los 
	//correctos, 
	//el numero de columnas es fijo
	//y el numero de filas es variables
	//pero debe haber más de dos filas
	//(una fila para la información) y
	//otra fila para los datos.


int  comprobar_dimensiones_csv_variable(datos_csv_t* datos_csv, const int minimo_filas,
	const int cantidad_columnas_fijas, const char* categoria_csv) {


	int numero_filas = datos_csv->filas;
	int numero_columnas = datos_csv->columnas;

	//El minimo de filas incluye la fila con los
	//encabezados.

	if (numero_filas < minimo_filas) {

		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error: Debe haber al menos %d filas con informacion en el csv de %s Error:Actualmente tiene %d filas\n",
			minimo_filas, categoria_csv, numero_filas);
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	if (numero_columnas != cantidad_columnas_fijas) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error: Debe haber %d columnas en el csv de %s Error: Actualmente tiene %d columnas\n",
			cantidad_columnas_fijas, categoria_csv, numero_columnas);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	return EXITO;
}

int verificar_encabezado_csv(datos_csv_t* datos_csv, const char * valores_encabezado[],
	const int numero_encabezados, const char* categoria_csv) {
	// Este subprograma se utiliza para comprobar
	// que los encabezados de un archivo CSV sean
	// correctos.

	if (datos_csv == NULL || valores_encabezado == NULL || numero_encabezados <= 0 ||
		categoria_csv == NULL) {
		printf("Error: Parámetros de entrada no válidos en la comprobacion de los encabezados.\n");
		registrar_error("Error: Parámetros de entrada no válidos en la comprobacion de los encabezados.\n", REGISTRO_ERRORES);
		return ERROR;
	}


	for (int numero_columna = 0; numero_columna < numero_encabezados; numero_columna++) {
		if (!strings_iguales(datos_csv->datos[FILA_ENCABEZADOS][numero_columna],
			valores_encabezado[numero_columna])) {

			char mensaje_error[512];

      
      

			snprintf(mensaje_error, sizeof(mensaje_error)/sizeof(mensaje_error[0]), "Error en la columna %d del CSV %s el encabezado deberia ser %s y es %s",
				numero_columna, categoria_csv, valores_encabezado[numero_columna], datos_csv->datos[FILA_ENCABEZADOS][numero_columna]);
			printf("%s\n", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}
	}
	return EXITO;
}
