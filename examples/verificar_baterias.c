
#include "conversion_numeros.h"
#include "definiciones_globales.h"
#include "operaciones_csv.h"
#include "operaciones_fecha.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "validaciones.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>


//En este archivo .c vienen definidos los subprogramas 
//que se encargan de comprobar que la informacion del csv de las baterias
//son correctas

//Defino el numero de filas y columnas que ha de tener el csv de las baterias->
#define MINIMO_FILAS_CSV_BATERIAS     1
#define NUMERO_COLUMNAS_CSV_BATERIAS 21

/*
* En este archivo se encuentran los subprogramas que se encargan de verificar
* que la informacion de la bateria es correcta->
*/

//----revisado-----------------------------------------//


static int comprobar_objetivo_bateria(const char* objetivo) {

  if (strcmp("si", objetivo) == 0) {
    return EXITO;
  }
  else if (strcmp("no", objetivo) == 0) {
    return EXITO;
  }
  else {
    printf("en el campo de considerar la carga debe introducirse un si o un no \n");
    registrar_error("En el campo de considerar la carga debe introducirse un si o un no", REGISTRO_ERRORES);
    return ERROR;
  }
}





static int comprobar_datos_bateria(const datos_csv_baterias_t* datos_bateria, const int numero_bateria) {

	/*
	Este subprograma se utiliza para comprobar si:
	El terminal al que esta conectado es valido.
	El dato de la capacidad de la bateria tiene sentido
	El dato de la potencia maxima que se puede aceptar tiene sentido
	El dato de la bateria inicial tiene sentido
	El dato de la bateria deseada tiene sentido
	*/


	/*Cargo el puntero que apunta a la posicon en memoria donde se encuentra la informacion de las
	  baterias*/

	datos_csv_t informacion_baterias = datos_bateria->informacion_baterias;


	//Cargo las columnas y filas en las que se encuentra la informacion en el csv de las baterias->
	int  columna_csv_baterias_num_terminal = datos_bateria->posiciones_informacion_baterias.columna_terminal;
	int  columna_csv_baterias_capacidad = datos_bateria->posiciones_informacion_baterias.columna_capacidad_bateria;
	int  columna_csv_baterias_max_potencia = datos_bateria->posiciones_informacion_baterias.columna_capacidad_bateria;
	int  columna_csv_baterias_inicial = datos_bateria->posiciones_informacion_baterias.columna_porcentaje_bateria_inicial;
	int  columna_csv_baterias_final = datos_bateria->posiciones_informacion_baterias.columna_porcentaje_bateria_deseada;
  int  columna_objetivo_baterias = datos_bateria->posiciones_informacion_baterias.columna_consideracion_objetivo;


	//Cargo los datos del csv de las baterias
	char*    numero_terminal_char = informacion_baterias.datos[numero_bateria][columna_csv_baterias_num_terminal];
	char*    capacidad_bateria_char = informacion_baterias.datos[numero_bateria][columna_csv_baterias_capacidad];
	char*    maxima_potencia_char = informacion_baterias.datos[numero_bateria][columna_csv_baterias_max_potencia];
	char*    bateria_inicial_char = informacion_baterias.datos[numero_bateria][columna_csv_baterias_inicial];
	char*    bateria_deseada_char = informacion_baterias.datos[numero_bateria][columna_csv_baterias_final];
  char* objetivo_bateria_char = informacion_baterias.datos[numero_bateria][columna_objetivo_baterias];


	//Creo las variables que sirven para almacenar la informacion de tipo numerico
	int numero_terminal_num;
	double capacidad_bateria_num;
	double maxima_potencia_num;
	double bateria_inicial_num;
	double bateria_deseada_num;

	//Se convierte las variables a tipo int o double
	if (convertir_a_entero(numero_terminal_char,&numero_terminal_num) == ERROR) {
		printf("El numero de terminal no se pudo convertir a numero\n");
		registrar_error("El numero de terminal no se pudo a convertir a numero\n", REGISTRO_ERRORES);
		return ERROR;
	}
	if (convertir_a_decimal(capacidad_bateria_char, &capacidad_bateria_num) == ERROR) {
		printf("La capacidad de bateria no se pudo convertir a numero\n");
		registrar_error("La capacidad de la bateria no se pudo convertir a numero\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (convertir_a_decimal(maxima_potencia_char, &maxima_potencia_num) == ERROR) {
		printf("La potencia maxima no se pudo convertir a numero\n");
		registrar_error("La maxima potencia no se pudo convertir a numero\n", REGISTRO_ERRORES);
		return ERROR;
	}
	if (convertir_a_decimal(bateria_inicial_char, &bateria_inicial_num) == ERROR) {
		printf("La bateria inicial no se pudo convertir a variable numerica\n");
		registrar_error("La bateria inicial no se pudo convertir a variable numerica\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (convertir_a_decimal(bateria_deseada_char, &bateria_deseada_num) == ERROR) {
		printf("La bateria deseada no se pudo convertir a variable numerica\n");
		registrar_error("La bateria deseada no se pudo convertir a varaible numerica\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if ((numero_terminal_num < 0) || (numero_terminal_num > 12)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "El numero de terminal al que esta conectado la bateria %d\n", numero_bateria);
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	if (capacidad_bateria_num <= 0) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "La capacidad de la bateria %d no puede ser 0 o inferior a 0\n", numero_bateria);
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	else if (maxima_potencia_num < 0) {
    char mensaje_error[512];
    snprintf(mensaje_error, sizeof(mensaje_error), "La potencia máxima de la batería %d no puede ser 0 o inferior a 0\n", numero_bateria);
		printf(mensaje_error);
    registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	else if ((bateria_inicial_num < 0) || (bateria_inicial_num > 1)) {
    char mensaje_error[512];
    snprintf(mensaje_error, sizeof(mensaje_error), "el porcentaje inicial de la bateria %d ha de estar entre 0 o 1 (0% y 100%)", numero_bateria);
    printf(mensaje_error);
    registrar_error(mensaje_error, REGISTRO_ERRORES);
    return ERROR;
	}
	else if ((bateria_deseada_num < 0) || (bateria_deseada_num > 1)) {
    char mensaje_error[512];
    snprintf(mensaje_error, sizeof(mensaje_error), "El porcentaje de bateria deseada %d ha de estar entre 0 o 1 (0% y 100%\n", numero_bateria);
    printf(mensaje_error);
    registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
  else if (comprobar_objetivo_bateria(objetivo_bateria_char) == ERROR) {
    printf("No se reconoció´\n");
    char mensaje_error[512];
    snprintf(mensaje_error, sizeof(mensaje_error), "El campo que sirve para determinar que si la bateria tiene un objetivo de carga tiene que ser si o no\n");
    printf(mensaje_error);
    registrar_error(mensaje_error, REGISTRO_ERRORES);
    return ERROR;
  }
	return EXITO;
}

//Este subprograma se utiliza para comprobar si el termino de considerar la carga de la bateria es correcto





static int comprobar_fecha_inicial_bateria(struct tm* fecha_inicial_bateria, const datos_csv_baterias_t* datos_baterias,
	const int numero_bateria, const datos_csv_algoritmo_t* datos_algoritmo) {

	//Cargo la fecha inicial del algoritmo->
	struct tm fecha_inicial_algoritmo = { 0 };

	//Cargo los punteros a las posiciones de las memoria en la que se encuentra la informacion
	//del algoritmo y la bateria.
	datos_csv_t informacion_algoritmo = (datos_algoritmo->informacion_algoritmo);
	datos_csv_t informacion_bateria   = (datos_baterias->informacion_baterias);

	//Cargo las columnas y filas donde se encuentra la informacion
	//Fecha inicial del algoritmo

	int fila_informacion_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.fila_informacion;
	int columna_anyo_inicial_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_anyo;
	int columna_mes_inicial_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_mes;
	int columna_dia_inicial_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_dia;
	int columna_hora_inicial_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_hora;
	int columna_minuto_inicial_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_minuto;

	//Cargo las columnas y filas donde se encuentra la informacion de 
	//las baterias
	int columna_csv_baterias_anyo_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_anyo;
	int columna_csv_baterias_mes_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_mes;
	int columna_csv_baterias_dia_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_dia;
	int columna_csv_baterias_hora_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_hora;
	int columna_csv_baterias_minuto_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_minuto;

	//Se carga la fecha inicial del algoritmo 
	cargar_fecha(&informacion_algoritmo, &fecha_inicial_algoritmo, columna_anyo_inicial_algoritmo,
	columna_mes_inicial_algoritmo, columna_dia_inicial_algoritmo, columna_hora_inicial_algoritmo,
	columna_minuto_inicial_algoritmo, fila_informacion_algoritmo,SI_INCLUIR_MINUTO);



	//Se comprueba si la fecha inicial de conexion de la bateria es posterior a la fecha de inicio del algoritmo.
	cargar_fecha(datos_baterias, fecha_inicial_bateria, columna_csv_baterias_anyo_inicial,
	columna_csv_baterias_mes_inicial, columna_csv_baterias_dia_inicial,
	columna_csv_baterias_hora_inicial, columna_csv_baterias_minuto_inicial,
	numero_bateria,SI_INCLUIR_MINUTO);



	if (verificar_orden_fechas(fecha_inicial_algoritmo, *fecha_inicial_bateria,INCLUIR_FECHA_IGUAL) == ERROR) {
		printf("La fecha final de la bateria no puede ser posterior a la del algoritmo\n");
		registrar_error("La fecha final de la bateria no puede ser posterior a la del algoritmo\n", REGISTRO_ERRORES);
		return ERROR;
	}


	return EXITO;
}

static int comprobar_fecha_final_bateria(struct tm* fecha_final_bateria, const datos_csv_baterias_t* datos_baterias,
	const int numero_bateria, const datos_csv_algoritmo_t * datos_algoritmo) {

	//Se comprueba que la fecha final de la bateria es correcta->

	struct tm fecha_final_algoritmo = { 0 };
	

	//Cargo los punteros que apuntan a las posiciones en memoria donde se encuentra la informacion
	//del algoritmo y la bateria

	datos_csv_t* informacion_algoritmo = &(datos_algoritmo->informacion_algoritmo);
	datos_csv_t* informacion_baterias = &(datos_baterias->informacion_baterias);

	//Se carga las columnas y filas donde se encuentra la informacion
	int fila_informacion_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.fila_informacion;
	int columna_anyo_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_anyo;
	int columna_mes_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_mes;
	int columna_dia_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_dia;
	int columna_hora_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_hora;
	int columna_minuto_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_minuto;

	//Se carga las columnas y filas donde se encuentra la informacion de las baterias
	int columna_csv_baterias_anyo_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_anyo;
	int columna_csv_baterias_mes_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_mes;
	int columna_csv_baterias_dia_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_dia;
	int columna_csv_baterias_hora_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_hora;
	int columna_csv_baterias_minuto_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_minuto;

	cargar_fecha(informacion_algoritmo, &fecha_final_algoritmo, columna_anyo_final_algoritmo,
	columna_mes_final_algoritmo, columna_dia_final_algoritmo, columna_hora_final_algoritmo,
	columna_minuto_final_algoritmo, fila_informacion_algoritmo,SI_INCLUIR_MINUTO);

	//se comprueba si la fecha de desconexion de la bateria no es posterior a la finalizacion del algoritmo.
	int resultado_comparacion;

	
	
	cargar_fecha(datos_baterias, fecha_final_bateria, columna_csv_baterias_anyo_final,
	columna_csv_baterias_mes_final, columna_csv_baterias_dia_final,
	columna_csv_baterias_hora_final, columna_csv_baterias_minuto_final,
	numero_bateria,SI_INCLUIR_MINUTO);

	
    
	if (verificar_orden_fechas(*fecha_final_bateria,fecha_final_algoritmo,INCLUIR_FECHA_IGUAL) == ERROR) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error),
			"La fecha final de la bateria es Año %d  Mes %d Dia %d Hora %d Minuto %d "
			"La fecha final del algoritmo es Año %d  Mes %d Dia %d Hora %d Minuto %d "
			"La fecha final de la bateria no puede ser posterior a la del algoritmo\n",
			fecha_final_bateria->tm_year + DESPLAZAMIENTO_ANYO, fecha_final_bateria->tm_mon + DESPLAZAMIENTO_MES, fecha_final_bateria->tm_mday,
			fecha_final_bateria->tm_hour, fecha_final_bateria->tm_min,
			fecha_final_algoritmo.tm_year + DESPLAZAMIENTO_ANYO, fecha_final_algoritmo.tm_mon + DESPLAZAMIENTO_MES, fecha_final_algoritmo.tm_mday,
			fecha_final_algoritmo.tm_hour, fecha_final_algoritmo.tm_min);
		
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	return EXITO;
}

bool considerar_objetivo_bateria(const char *objetivo) {
	if (strcmp("si", objetivo) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Este subprograma se utiliza apra comprobar que la fecha objetivo de la bateria es correcta.

static int comprobar_fecha_objetivo_baterias(const datos_csv_baterias_t* datos_baterias, const int numero_bateria,
	const struct tm fecha_inicial_bateria, const struct tm fecha_final_bateria) {
	

	struct tm fecha_objetivo_bateria;
	int       resultado_comparacion;

	//Cargo el puntero donde que apunta a la informacion donde se encuentra la informacion de la bateria
	datos_csv_t* informacion_baterias = &(datos_baterias->informacion_baterias);
	
	//Cargo las columna donde se encuentra la informacion de la bateria
	int columna_csv_baterias_anyo_objetivo   = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_anyo;
	int columna_csv_baterias_mes_objetivo    = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_mes;
	int columna_csv_baterias_dia_objetivo    = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_dia;
	int columna_csv_baterias_hora_objetivo   = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_hora;
	int columna_csv_baterias_minuto_objetivo = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_minuto;
	
	cargar_fecha(informacion_baterias, &fecha_objetivo_bateria, columna_csv_baterias_anyo_objetivo,
	columna_csv_baterias_mes_objetivo, columna_csv_baterias_dia_objetivo,
	columna_csv_baterias_hora_objetivo, columna_csv_baterias_minuto_objetivo,
	numero_bateria,SI_INCLUIR_MINUTO);

	
	if (verificar_orden_fechas(fecha_inicial_bateria, fecha_objetivo_bateria,INCLUIR_FECHA_IGUAL) == ERROR) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error),"La fecha objetivo de la bateria no puede ser anterior a la fecha inicial de la bateria \n"
			    "La fecha objetivo de la bateria es Anyo %d Mes %d Dia %d Hora %d Minuto %d \n"
		        "La fecha inicial  de la bateria es Anyo %d Mes %d Dia %d Hora %d Minuto %d \n",
			    fecha_objetivo_bateria.tm_year, fecha_objetivo_bateria.tm_mon,
			    fecha_objetivo_bateria.tm_mday, fecha_objetivo_bateria.tm_hour,
			    fecha_objetivo_bateria.tm_min,
			    fecha_inicial_bateria.tm_year,  fecha_inicial_bateria.tm_mon,
			    fecha_inicial_bateria.tm_mday,  fecha_inicial_bateria.tm_hour,
			    fecha_inicial_bateria.tm_min
		);
		printf(mensaje_error);
		registrar_error(mensaje_error,REGISTRO_ERRORES);
		return ERROR;
	}
	
	if (verificar_orden_fechas(fecha_objetivo_bateria, fecha_final_bateria,INCLUIR_FECHA_IGUAL) == ERROR) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "La fecha objetivo de la bateria no puede ser anterior a la fecha inicial de la bateria\n"
			"La fecha objetivo de la bateria es Año %d Mes %d Dia %d Hora %d Minuto %d \n"
			"La fecha inicial  de la bateria es Año %d Mes %d Dia %d Hora %d Minuto %d \n",
			fecha_objetivo_bateria.tm_year + DESPLAZAMIENTO_ANYO, fecha_objetivo_bateria.tm_mon +DESPLAZAMIENTO_MES,
			fecha_objetivo_bateria.tm_mday, fecha_objetivo_bateria.tm_hour,
			fecha_objetivo_bateria.tm_min,
			fecha_final_bateria.tm_year +  DESPLAZAMIENTO_ANYO, fecha_final_bateria.tm_mon + DESPLAZAMIENTO_MES,
			fecha_final_bateria.tm_mday, fecha_final_bateria.tm_hour,
			fecha_final_bateria.tm_min);
		
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}


	return EXITO;
}

    /*
	* Este subprograma se utiliza con el objetivo de comprobar que las fechas de inicio y finalizacion
	* de carga de las diversas baterias tienen sentido.
	*/


static int comprobar_fecha_baterias(const int numero_bateria, const datos_csv_baterias_t* datos_baterias,
	const datos_csv_algoritmo_t * datos_algoritmo) {
	
	struct tm fecha_inicial_bateria;
	struct tm fecha_final_bateria;
	struct tm fecha_objetivo_bateria;
	int       variable_control;

	//Cargo las posiciones donde se encuentran algunas informaciones del csv
	int columna_objetivo= datos_baterias->posiciones_informacion_baterias.columna_consideracion_objetivo;
	//Cargo el puntero que apunta a la posicion de la memoria donde se encuentra la informacion de las baterias
	datos_csv_t* informacion_baterias = &(datos_baterias->informacion_baterias);

	//Se comprueba si la fecha inicial de la bateria es correcta
	if (comprobar_fecha_inicial_bateria(&fecha_inicial_bateria, datos_baterias, numero_bateria, datos_algoritmo) == ERROR) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "La fecha inicial de conexion de la bateria %d no es correcta\n",numero_bateria);
		printf("la fecha inicial de conexion de la bateria %d no es correcta", numero_bateria);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	//Se comprueba si la fecha final de la bateria es correcta
	if (comprobar_fecha_final_bateria(&fecha_final_bateria, datos_baterias, numero_bateria, datos_algoritmo) == ERROR) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "La fecha final de conexion de la bateria %d no es correcta", numero_bateria);
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	
	
	//Se comprueba que la fecha inicial de la bateria es anterior a la de finalizacion.
	if (verificar_orden_fechas(fecha_inicial_bateria, fecha_final_bateria,INCLUIR_FECHA_IGUAL) == ERROR) {
		printf("Error la fecha inicial de carga de la bateria no puede ser posterior a la fecha final de carga\n");
		registrar_error("Error la fecha inicial de carga de la bateria no puede ser posterior a la fecha final de carga\n", REGISTRO_ERRORES);
		return ERROR;
	}

  
	//Se revisa si para la bateria en cuestion se desea si se desea que tenga un cierto nivel de carga para alguna
	//fecha en particular.
	if (considerar_objetivo_bateria(informacion_baterias->datos[numero_bateria][columna_objetivo])==ERROR) {
  
	//Se comprueba que la fecha objetivo de carga de la bateria sea correcta.
		if (comprobar_fecha_objetivo_baterias(datos_baterias, numero_bateria, fecha_inicial_bateria, fecha_final_bateria) == ERROR) {
			printf("La fecha objetivo de carga de la bateria no es correcta\n");
			registrar_error("La fecha objetivo de carga de la bateria no es correcta\n",REGISTRO_ERRORES);
			return ERROR;
		}


	}
	return EXITO;
}

/*
* se verifica que los encabezados del csv de las baterias son correctos->
*/
static int verificar_encabezados_baterias(datos_csv_t* datos_baterias) {

	const char* valores_aceptables[] = { "Terminal","Capacidad Bateria","Bateria Inicial",
										   "Bateria Final","Maxima Potencia","Año Inicial",
										   "Mes Inicial","Dia Inicial","Hora Inicial",
										   "Minuto Inicial","Año Final","Mes Final",
										   "Dia Final","Hora Final","Minuto Final",
										   "¿Considerar Objetivo?","Año Objetivo",
	                                       "Mes Objetivo","Dia Objetivo","Hora Objetivo",
	                                       "Minuto Objetivo"};

	const int numero_encabezados = sizeof(valores_aceptables) / sizeof(valores_aceptables[0]);

	if (verificar_encabezado_csv(datos_baterias, valores_aceptables, numero_encabezados, "baterias") == ERROR) {
		printf("los encabezados del csv de las baterias son incorrectos\n");
		registrar_error("los encabezados del csv de las baterias son incorrectos\n", REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}

/*
* En este subprograma se va comprobando que los datos del csv que contiene la informacion de 
* las baterias es correcto
*/

int verificar_baterias(const datos_csv_baterias_t* datos_csv_baterias, const datos_csv_algoritmo_t* datos_csv_algoritmo) {

	// Cargo el  numero de filas que hay en el csv de las baterias
	int numero_filas_csv_baterias = datos_csv_baterias->informacion_baterias.filas;


	// Se crea un puntero que apunta a la posicion en memoria donde se encuentra la informacion en
	// la bateria

	datos_csv_t* informacion_baterias = &(datos_csv_baterias->informacion_baterias);


	//Se comprueba las dimensiones del csv de las baterias

	if (comprobar_dimensiones_csv_variable(informacion_baterias, MINIMO_FILAS_CSV_BATERIAS,
		NUMERO_COLUMNAS_CSV_BATERIAS, "baterias") == ERROR) {
		printf("error en las dimensiones del csv de las baterias\n");
		registrar_error("error en las dimensiones del csv de las baterias", REGISTRO_ERRORES);
		return ERROR;
	}

	if ((verificar_encabezados_baterias(informacion_baterias)) == ERROR) {
		printf("error en los encabezados del csv de las baterias\n");
		registrar_error("error en los encabezados del csv de las baterias\n", REGISTRO_ERRORES);
		return ERROR;
	}

	//Se pasa a comprobar si los datos de cada bateria en el csv son incorrectos->
	for (int numero_bateria = 1; numero_bateria < numero_filas_csv_baterias; numero_bateria++) {

		//Se comprueba que las fechas de conexion y desconexion de las baterias son correctas.
		if (comprobar_fecha_baterias(numero_bateria, datos_csv_baterias, datos_csv_algoritmo) == ERROR) {
			printf("Las fechas de las baterias son incorrectas\n");
			registrar_error("Las fechas de las baterias son incorrectas\n",REGISTRO_ERRORES);
			return ERROR;
		}

		//Se comprueba que los datos de la bateria son correctos.
		if (comprobar_datos_bateria(datos_csv_baterias, numero_bateria) == ERROR) {
			printf("Los datos de las baterias son incorrectos\n");
			registrar_error("Los datos de las baterias son incorrectos\n", REGISTRO_ERRORES);
			return ERROR;
		}
	}
	return EXITO;
}