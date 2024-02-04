
/*
En este archivo se encuentran las definiciones de
los subprogramas relacionados con verificar las
dimensiones y encabezados de los csvs de Entrada.
*/

#ifndef OPERACIONES_CSV_H
#define OPERACIONES_CSV_H
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*Funcion que verifica que el numero de filas y columnas es correcto para CSVs cuya dimension
	  no es siempre la misma.*/
	int comprobar_dimensiones_csv_variable(datos_csv_t* datos_csv, const int minimo_filas,
		const int cantidad_columnas_fijas, const char* categoria_csv);

	/*Funcion que verifica que el numero de filas y columnas es correcto para CSVs cuya dimension
	  es siempre la misma.*/
	int comprobar_dimensiones_csv_fijo(datos_csv_t* datos_csv, const int cantidad_fija_filas,
		const int cantidad_columnas_fijas, const char* categoria_csv);

	/*Funcion que verifican que los encabezados de las columnas del csv son correctos*/
	int verificar_encabezado_csv(datos_csv_t* datos_csv, const char* valores_encabezado[],
		const int numero_encabezados, const char* categoria_csv);



#ifdef __cplusplus
}
#endif

#endif
