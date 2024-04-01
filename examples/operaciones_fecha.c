/*
En este subprograma se encuentran
las funciones auxiliares relacionadas
con operar las fechas.
*/

#include "definiciones_globales.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
/*
Defino las columnas en donde se encuentran
los diferentes tipos de datos del algoritmo
*/


//Se convierte los numeros al sistema decimal.
#define BASE_NUMERO 10 
//Defino la fila donde se encuentran los datos.
#define FILA_DATOS_ALGORITMO 1   

//Hay columnas que son iguales en el CSV de las
//baterias y en el de los vehiculos 
#define COLUMNA_CSV_CARGA_ANYO_INICIAL    6
#define COLUMNA_CSV_CARGA_MES_INICIAL     7
#define COLUMNA_CSV_CARGA_DIA_INICIAL     8
#define COLUMNA_CSV_CARGA_HORA_INICIAL    9
#define COLUMNA_CSV_CARGA_MINUTO_INICIAL  10 
#define COLUMNA_CSV_CARGA_ANYO_FINAL      11
#define COLUMNA_CSV_CARGA_MES_FINAL       12
#define COLUMNA_CSV_CARGA_DIA_FINAL       13
#define COLUMNA_CSV_CARGA_HORA_FINAL      14
#define COLUMNA_CSV_CARGA_MINUTO_FINAL    15

/*
Verifica si el orden de dos fechas es válido.
Devuelve EXITO si la primera fecha es anterior, ERROR en caso contrario.
*/



int  verificar_orden_fechas(const struct tm fecha_anterior, const struct tm fecha_posterior,
	const int incluir_igual) {



	struct tm fecha_anterior_aux = fecha_anterior;
	struct tm fecha_posterior_aux = fecha_posterior;
	time_t    tiempo_anterior, tiempo_posterior;

  /*
  printf("Fecha anterior: %d-%02d-%02d %02d:%02d:%02d\n",
    fecha_anterior.tm_year + 1900, // Year since 1900, so add 1900
    fecha_anterior.tm_mon + 1,     // Months since January, so add 1
    fecha_anterior.tm_mday,        // Day of the month
    fecha_anterior.tm_hour,        // Hour of the day
    fecha_anterior.tm_min,         // Minute after the hour
    fecha_anterior.tm_sec);        // Second after the minute

  printf("Fecha posterior: %d-%02d-%02d %02d:%02d:%02d\n",
    fecha_posterior.tm_year + 1900, // Year since 1900, so add 1900
    fecha_posterior.tm_mon + 1,     // Months since January, so add 1
    fecha_posterior.tm_mday,        // Day of the month
    fecha_posterior.tm_hour,        // Hour of the day
    fecha_posterior.tm_min,         // Minute after the hour
    fecha_posterior.tm_sec);        // Second after the minute
    */

	tiempo_anterior = mktime(&fecha_anterior_aux);
	tiempo_posterior = mktime(&fecha_posterior_aux);

	if ((tiempo_anterior == -1) || (tiempo_posterior == -1)) {
		return ERROR;
	}

	if (tiempo_anterior < tiempo_posterior) {
		return EXITO;
	}

	return (tiempo_anterior == tiempo_posterior) && (incluir_igual == INCLUIR_FECHA_IGUAL) ? EXITO : ERROR;


}


/*
Carga una fecha desde un archivo CSV.
*/

int cargar_fecha(const datos_csv_t* datos_entrada, struct tm* fecha, const int columna_csv_anyo,
	const int columna_csv_mes, const int columna_csv_dia, const int columna_csv_hora,
	const int columna_csv_minuto, const int numero_fila, const int incluir_minuto) {
	/*
	Carga una fecha desde un archivo CSV.
	*/

	// Usando errno para el manejo de errores durante la conversión
	errno = 0;

	fecha->tm_year = strtol(datos_entrada->datos[numero_fila][columna_csv_anyo], NULL, BASE_NUMERO) - DESPLAZAMIENTO_ANYO;
	
	if (errno == ERANGE) {
		return ERROR;
	}

	fecha->tm_mon = strtol(datos_entrada->datos[numero_fila][columna_csv_mes], NULL, BASE_NUMERO) - DESPLAZAMIENTO_MES;
	if (errno == ERANGE) {
		return ERROR;

	}

	fecha->tm_mday = strtol(datos_entrada->datos[numero_fila][columna_csv_dia], NULL, BASE_NUMERO);
	if (errno == ERANGE) {
		return ERROR;
	}
	fecha->tm_hour = strtol(datos_entrada->datos[numero_fila][columna_csv_hora], NULL, BASE_NUMERO);

	if (errno == ERANGE) {
		return ERROR;
	}
	fecha->tm_sec = 0;

	// Indica si la fecha corresponde a un cambio en el horario de verano
	fecha->tm_isdst = -1;

	if (incluir_minuto) {
		fecha->tm_min = strtol(datos_entrada->datos[numero_fila][columna_csv_minuto], NULL, 10);
		if (errno == ERANGE) return ERROR; {

		}
	}
	else {
		fecha->tm_min = 0;
	}
	
	if (mktime(fecha) == (time_t)-1) {
		return ERROR;
	}
	
	return EXITO;
}

/* Este subprograma se utiliza para cargar la fecha de inicio y finalizacion*/
/* Puede ser del algoritmo, de un vehiculo o una bateria */

int cargar_fecha_generico(datos_csv_t* datos, struct tm* fecha_inicio, struct tm* fecha_final,
	int columna_anyo_inicio, int columna_mes_inicio, int columna_dia_inicio,
	int columna_hora_inicio, int columna_minuto_inicio, int columna_anyo_final,
	int columna_mes_final, int columna_dia_final, int columna_hora_final,
	int columna_minuto_final, int numero_fila, int incluir_minuto) {

	if (cargar_fecha(datos, fecha_inicio, columna_anyo_inicio, columna_mes_inicio, columna_dia_inicio,
		columna_hora_inicio, columna_minuto_inicio, numero_fila, incluir_minuto) == ERROR) {
		printf("Error al cargar la fecha de inicio\n");
    registrar_error("Error al cargar la fecha de inicio\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (cargar_fecha(datos, fecha_final, columna_anyo_final, columna_mes_final, columna_dia_final,
		columna_hora_final, columna_minuto_final, numero_fila, incluir_minuto) == ERROR) {
		printf("Error al cargar la fecha final\n");
    registrar_error("Error al cargar la fecha final\n", REGISTRO_ERRORES);
		return ERROR;
	}

	return EXITO;
}





/*
Este subprograma se utiliza para
cargar la fecha de inicio y finalización
del Algoritmo en dos variables.
*/

int cargar_fecha_algoritmo(datos_csv_algoritmo_t* datos_algoritmo, struct tm* fecha_inicial_algoritmo,
	struct tm* fecha_final_algoritmo) {

	//Cargo las columnas donde se encuentra la informacion
	int columna_anyo_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_anyo;
	int columna_mes_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_mes;
	int columna_dia_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_dia;
	int columna_hora_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_hora;
	int columna_minuto_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_minuto;
	int columna_anyo_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_anyo;
	int columna_mes_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_mes;
	int columna_dia_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_dia;
	int columna_hora_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_hora;
	int columna_minuto_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_minuto;

	//Cargo el puntero que apunta a la posicion en memoria donde se encuentra la
	//informacion del algoritmo
	datos_csv_t* informacion_algoritmo = &(datos_algoritmo->informacion_algoritmo);

	return cargar_fecha_generico(informacion_algoritmo, fecha_inicial_algoritmo, fecha_final_algoritmo,
		columna_anyo_inicio_algoritmo, columna_mes_inicio_algoritmo,
		columna_dia_inicio_algoritmo, columna_hora_inicio_algoritmo,
		columna_minuto_inicio_algoritmo, columna_anyo_final_algoritmo,
		columna_mes_final_algoritmo, columna_dia_final_algoritmo,
		columna_hora_final_algoritmo, columna_minuto_final_algoritmo,
		FILA_DATOS_ALGORITMO, SI_INCLUIR_MINUTO);

}
/*
Este subprograma se utiliza para
cargar la fecha de inicio o
finalización de un vehiculo
o bateria.
*/

int cargar_fecha_carga(datos_csv_t* datos_carga, struct tm* fecha_inicial_carga,
	struct tm* fecha_final_carga, const int numero_fila) {

	return cargar_fecha_generico(datos_carga, fecha_inicial_carga, fecha_final_carga,
		COLUMNA_CSV_CARGA_ANYO_INICIAL, COLUMNA_CSV_CARGA_MES_INICIAL,
		COLUMNA_CSV_CARGA_DIA_INICIAL, COLUMNA_CSV_CARGA_HORA_INICIAL,
		COLUMNA_CSV_CARGA_MINUTO_INICIAL, COLUMNA_CSV_CARGA_ANYO_FINAL,
		COLUMNA_CSV_CARGA_MES_FINAL, COLUMNA_CSV_CARGA_DIA_FINAL,
		COLUMNA_CSV_CARGA_HORA_FINAL, COLUMNA_CSV_CARGA_MINUTO_FINAL,
		numero_fila, SI_INCLUIR_MINUTO);
}
/*
Para un vehiculo o bateria se comprueba que :
Las fechas de inicio y finalización de carga
están dentro de la ventana temporal del algoritmo.
La fecha de inicio y finalización tienen sentido
es decir la fecha de finalización es posterior a
la de inicio.
*/

int comprobar_fecha_carga(struct tm fecha_inicial_algoritmo, struct tm fecha_final_algoritmo,
	struct tm fecha_inicial_carga, struct tm fecha_final_carga) {

	if (verificar_orden_fechas(fecha_inicial_algoritmo, fecha_inicial_carga,
		INCLUIR_FECHA_IGUAL) == ERROR) {
		printf("Error la carga del vehiculo no puede empezar antes que el inicio del algoritmo\n");
    registrar_error("Error la carga del vehiculo no puede empezar antes que el inicio del algoritmo\n",REGISTRO_ERRORES);
		return ERROR;
	}
	if (verificar_orden_fechas(fecha_final_carga, fecha_final_algoritmo,
		INCLUIR_FECHA_IGUAL) == ERROR) {
		printf("Error la carga del vehiculo no puede terminar más tarde que el fin del algoritmo\n");
    registrar_error("Error la carga del vehiculo no puede terminar más tarde que el fin del algoritmo\n",REGISTRO_ERRORES);
		return ERROR;
	}
	if (verificar_orden_fechas(fecha_inicial_carga, fecha_final_carga,
		NO_INCLUIR_FECHA_IGUAL) == ERROR) {
		printf("Error la fecha de inicio de la carga debe ser anterior a la fecha del posterior\n");
    registrar_error("Error la fecha de inicio de la carga debe ser anterior a la fecha del posterior\n", REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}
/*
Este subprograma se utiliza
para verificar que la fecha
de carga de un vehiculo o
batería es correcta.

Para un vehiculo o batería se comprueba que:
Las fechas de inicio y finalización de carga
están dentro de la ventana temporal del algoritmo.
La fecha de inicio y finalización tienen sentido
es decir la fecha de finalización es posterior a
la de inicio.

Las entradas para este subprograma son:

1. Los datos del CSV que contiene la información
de los vehículos o baterías,
2. El CSV que contiene la información del algoritmo
3. El número de fila del CSV de los vehículos o baterías
en otras palabras el número de vehículo o batería a revisar.
*/
int verificar_fecha_carga(datos_csv_t* datos_carga, datos_csv_algoritmo_t* datos_algoritmo,
	const int numero_fila) {

	struct tm fecha_inicial_algoritmo = { 0 };
	struct tm fecha_final_algoritmo = { 0 };
	struct tm fecha_inicial_carga = { 0 };
	struct tm fecha_final_carga = { 0 };

	if (cargar_fecha_algoritmo(datos_algoritmo, &fecha_inicial_algoritmo,
		&fecha_final_algoritmo) == ERROR) {
    printf("Error no se ha podido cargar la fecha inicial y la fecha final del algoritmo\n");
    registrar_error("Error no se ha podido cargar la fecha inicial y la fecha final del algoritmo\n", REGISTRO_ERRORES);
		return ERROR;
	};

	if (cargar_fecha_carga(datos_carga, &fecha_inicial_carga,
		&fecha_final_carga, numero_fila) == ERROR) {
    printf("Error no se ha podido cargar la fecha inicial y la fecha final de carga\n");
    registrar_error("Error no se ha podido cargar la fecha inicial y la fecha final de carga\n", REGISTRO_ERRORES);
		return ERROR;
	};


	if (comprobar_fecha_carga(fecha_inicial_algoritmo,
		fecha_final_algoritmo, fecha_inicial_carga,
		fecha_final_carga) == ERROR) {
    printf("No se ha podido comprobar el orden temporal de las fechas del algoritmo y de carga\n");
    registrar_error("Erro no se ha podido comprobar el orden temporal de las fechas del algoritmo y de carga\n",REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}

//Se utiliza este subprograma para comprobar si dos fechas son iguales
int comparar_fechas(const void* a, const void* b) {
  struct tm* fecha_1 = *(struct tm**)a;
  struct tm* fecha_2 = *(struct tm**)b;
  //Se pasa a formato time_t para comparar->
  time_t t1 = mktime(fecha_1);
  time_t t2 = mktime(fecha_2);

  if (t1 < t2) return -1;
  if (t1 > t2) return 1;
  return 0;
}
/*Se crea un subprograma para obtener la diferencia en minutos entre dos fechas */
int obtener_diferencia_minutos(const struct tm* fecha_1, const struct tm* fecha_2) {
  time_t t1 = mktime((struct tm*)fecha_1);
  time_t t2 = mktime((struct tm*)fecha_2);

  //Se calcula la diferencia en segundos
  double diff_segundos = difftime(t2, t1);

  //Se convierte lo segundos a minutos
  int diff_minutos = (int)(diff_segundos / 60);

  return diff_minutos;
}

/*Este subprograma se utiliza para comprobar que una fecha está dentro del rango :00 -:59 durante
  otra esto sirve para identificar cuando hay que avanzar al siguiente precio.*/
bool comprobar_hora(struct tm fecha_comprobar, struct tm fecha_referencia) {

  /*Se comprubeba si el año, mes y día es la misma*/
  if (fecha_comprobar.tm_year == fecha_referencia.tm_year &&
    fecha_comprobar.tm_mon == fecha_referencia.tm_mon &&
    fecha_comprobar.tm_mday == fecha_referencia.tm_mday) {


  //Se comrpbueba si la hora es la misma

    if (fecha_comprobar.tm_hour == fecha_referencia.tm_hour) {
      return  true;
    }

  }
  return false;
}
