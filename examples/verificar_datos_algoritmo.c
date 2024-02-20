#include "definiciones_globales.h"
#include "operaciones_fecha.h"
#include "operaciones_csv.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "validaciones.h"
#include "wchar.h"
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


//Este subprograma se utiliza con el proposito de
//comprobar que la diferencia temporal entre los
//puntos de la simulacion es de 1 a 60 minutos.

static int comprobar_resolucion_tiempo_simulacion(datos_csv_algoritmo_t* datos_algoritmo) {

	//Cargo el puntero que apunta a la posicion de memoria
	//donde se encuentra la informaciond de el algoritmo
	datos_csv_t* informacion_algoritmo = &(datos_algoritmo->informacion_algoritmo);

	//Cargo la fila y columna donde se encuentra la informacion del algoritmo
	int fila_datos_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.fila_informacion;
	int columna_resolucion_minutos = datos_algoritmo->posiciones_informacion_algoritmo.resolucion_minutos;


	char* resolucion_minutos = informacion_algoritmo->datos[fila_datos_algoritmo][columna_resolucion_minutos];
	int resolucion_minutos_int = strtol(resolucion_minutos, NULL, 10); //Se convierte a tipo int
	if ((resolucion_minutos_int > 60) || (resolucion_minutos_int < 1)) {


		char mensaje_error[512];

		snprintf(mensaje_error, sizeof(mensaje_error), "Error la resolucion temporal del algoritmo ha de estar entra 1 y 60 y está en %d\n",
			resolucion_minutos_int);
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	else {
		return EXITO;
	}
}

//Este subprograma se utiliza
//para verificar que los datos
//del Array son correctos. Se 
//Define los valores que han de 
//tener los encabezados y se llama
//al subprograma que se encarga de
//comprobarlo.



static int verificar_encabezados_algoritmo(datos_csv_t* datos_algoritmo) {

 
  
	const char * valores_aceptables[] = {"Año Inicial","Mes Inicial","Dia Inicial",
											"Hora Inicial","Minuto Inicial","Año Final","Mes Final",
											"Dia Final","Hora Final","Minuto Final", "Resolucion Minutos" };

	const int Numero_Encabezados = sizeof(valores_aceptables) / sizeof(valores_aceptables[0]);

  //printf("%s", valores_aceptables[0]);
  //printf("%s", datos_algoritmo->datos[0][0]);

	if (verificar_encabezado_csv(datos_algoritmo, valores_aceptables,
		Numero_Encabezados, "Datos del Algoritmo") == ERROR) {
		printf("Los encabezados del CSV de los datos del algoritmo son incorrectos\n");
		registrar_error("Fallo en la lectura de los datos de entrada", REGISTRO_ERRORES);
		return ERROR;
	}


	return EXITO;

}
//Este subprograma se utiliza para verificar
//que  los datos de un CSV son del tipo 
//correcto (numerico o string)

static int verificar_tipo_datos_csv_algoritmo(datos_csv_algoritmo_t* datos_algoritmo) {

	//Cargo el puntero que apunta a la posiciones de memoria donde se encuentra la informacion del algoritmo
	datos_csv_t* informacion_algoritmo = &(datos_algoritmo->informacion_algoritmo);
	int fila_datos_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.fila_informacion;

	for (int columna = 0; columna < NUM_COLUMNAS_CSV_INFO_ALGORITMO; columna++) {

		if (!es_un_numero(informacion_algoritmo->datos[fila_datos_algoritmo][columna], DECIMAL_NO_INCLUIDO)) {

			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error en la columna %d del CSV de datos del algoritmo %s no es un dato numerico\n",
				columna, informacion_algoritmo->datos[fila_datos_algoritmo][columna]);

			printf(mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}
	}
	return EXITO;
}
//
static int verificar_parametros_temporales(datos_csv_algoritmo_t* datos_algoritmo) {
	//Este subprograma se utiliza para verificar
	//que los datos temporales del algoritmo 
	//son correctos

	//Se comprueba que la fecha inicial y final
	//del algoritmo son correctos.

	//Tambien se comprueba que la diferencia
	//temporal entre los puntos de calculo
	//del algoritmo es de 1 a 60 minutos.

	struct tm fecha_inicial;
	struct tm fecha_final;

	//Cargo el puntero a donde apunta la posicion de memoria donde se encuentra la informacion
	//del algoritmo

	datos_csv_t* informacion_algoritmo = &(datos_algoritmo->informacion_algoritmo);
	//Cargo la fila donde se encuentra la informacion
	int fila_datos_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.fila_informacion;

	//Cargo las columnas en donde se encuentra la informacion de inicio del algoritmo
	int columna_anyo_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_anyo;
	int columna_mes_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_mes;
	int columna_dia_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_dia;
	int columna_hora_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_hora;
	int columna_minuto_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_minuto;

	//Cargo las columnas en donde se ecuentra la informacion de finalizaicon del algoritmo
	int columna_anyo_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_anyo;
	int columna_mes_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_mes;
	int columna_dia_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_dia;
	int columna_hora_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_hora;
	int columna_minuto_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_minuto;

	//Se procede a cargar la fecha inicial y final del algoritmo en las variables.
	cargar_fecha(informacion_algoritmo, &fecha_inicial, columna_anyo_inicio_algoritmo,
		columna_mes_inicio_algoritmo, columna_dia_inicio_algoritmo,
		columna_hora_inicio_algoritmo, columna_minuto_inicio_algoritmo,
		fila_datos_algoritmo, SI_INCLUIR_MINUTO);

	cargar_fecha(informacion_algoritmo, &fecha_final, columna_anyo_final_algoritmo,
		columna_mes_final_algoritmo, columna_dia_final_algoritmo,
		columna_hora_final_algoritmo, columna_minuto_final_algoritmo,
		fila_datos_algoritmo, SI_INCLUIR_MINUTO);

	if (verificar_orden_fechas(fecha_inicial, fecha_final, INCLUIR_FECHA_IGUAL) == ERROR) {
		printf("Las fechas inicial y final del algoritmo no son correctas\n");
		registrar_error("Las fechas inicial y final del algoritmo no son correctas\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (comprobar_resolucion_tiempo_simulacion(datos_algoritmo) == ERROR) {
    printf("La resolucion temporal de la simulacion no es correcta\n");
    registrar_error("La resolucion temporal de la simulacion no es correcta\n", REGISTRO_ERRORES);
		return ERROR;
	}

	return EXITO;

}


int verificar_datos_algoritmo(datos_csv_algoritmo_t* datos_algoritmo) {
	//En este subprograma se verifica que el formato de 
	//de los datos introducidos en el CSV que contiene
	//la informacion del algoritmo es correcta

	datos_csv_t* datos_algoritmo_informacion = &(datos_algoritmo->informacion_algoritmo);

	if (comprobar_dimensiones_csv_fijo(datos_algoritmo_informacion, NUM_FILAS_CSV_INFO_ALGORITMO,
		NUM_COLUMNAS_CSV_INFO_ALGORITMO, "Informacion Algoritmo") == ERROR) {
		printf("Las Dimensiones del CSV de los datos del algoritmo son incorrectas\n");
		registrar_error("Las Dimensiones del CSV de los datos del algoritmo son incorrectas", REGISTRO_ERRORES);
		return ERROR;
	}

	if (verificar_encabezados_algoritmo(datos_algoritmo_informacion) == ERROR) {
		printf("Los encabezados del algoritmo no son correctos \n");
		registrar_error("Los encabezados del algoritmo no son correctos\n", REGISTRO_ERRORES);
		return ERROR;
	}
	if (verificar_tipo_datos_csv_algoritmo(datos_algoritmo) == ERROR) {
		printf("Hay valores incorrectos en el CSV de datos del algoritmo\n");
		registrar_error("Hay valores incorrectos en el CSV de datos del algoritmo\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if ((verificar_parametros_temporales(datos_algoritmo)) == ERROR) {
		printf("Los parametros temporales del algorimto son incorrectos\n");
		registrar_error("Los parametros temporales del algoritmo son incorrectos\n", REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}


