#include "calcular_puntos_simulacion.h"
#include "definiciones_globales.h"
#include "operaciones_fecha.h"
#include "operaciones_string.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdbool.h>
#include <stdio.h>

#define INICIO_CSV 0 //Constante que sirve para indicar la primera fila con datos en el CSV para verificar la
                     //simultaneidad
/*
*  Este subprograma se utiliza para comprobar si hay superposicion entre dos elementos vehiculos
*  o baterias en sus fechas. Es decir si estan conectados al mismo tiempo
*/
bool comprobar_superposicion(struct tm fecha_inicio_elemento_1, struct tm fecha_final_elemento_1,
	                         struct tm fecha_inicio_elemento_2, struct tm fecha_final_elemento_2) {





		struct tm start1_copy = fecha_inicio_elemento_1;
		struct tm end1_copy = fecha_final_elemento_1;
		struct tm start2_copy = fecha_inicio_elemento_2;
		struct tm end2_copy = fecha_final_elemento_2;

		return (difftime(mktime(&start1_copy), mktime(&start2_copy)) <= 0 && difftime(mktime(&end1_copy), mktime(&start2_copy)) >= 0) ||
		(difftime(mktime(&start2_copy), mktime(&start1_copy)) <= 0 && difftime(mktime(&end2_copy), mktime(&start1_copy)) >= 0);
	}



/* Este subprograma se utiliza para verificar que un vehiculo o bateria en concreto no tiene problemas de simultaneidad
   con el resto de elementos del csv de las baterias o vehiculos
   De parametros de entrada tiene los datos de entrada en el que se almacena la informacion, las columnas
   * donde se encuentra la informacion, la fecha inicial de conexion del vehiculo o bateria que se quiere
   * comprobar, el numero de terminal al que esta conectado, y el numero de fila del cual se comienza a
   * comprobar.
   */
int comprobar_simultaneidad_vehiculo (datos_csv_vehiculos_t* datos_vehiculos,
	const struct tm* fecha_inicial_elemento, const struct tm* fecha_final_elemento,
	const char* numero_terminal, const int fila_inicial) {

	//Cargo el puntero que apunta a las posiciones en memoria donde se encuentra la informacion de los vehiculos
	datos_csv_t* informacion_vehiculos = &(datos_vehiculos->informacion_vehiculos);
	
	//Cargo el numero de filas que tiene el csv que contiene los datos.
	int numero_filas = informacion_vehiculos->filas;

	//Cargo algunas columnas y filas donde se encuentra la informacion
	int columna_terminal = datos_vehiculos->posiciones_informacion_vehiculos.columna_terminales;

    //Se carga las columnas donde se encuentra la fecha inicial
	int columna_anyo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_anyo;
	int columna_mes_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_mes;
	int columna_dia_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_dia;
	int columna_hora_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_hora;
	int columna_minuto_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_minuto;
	
	//Se carga las columnas donde se encuentra la fecha final
	int columna_anyo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_anyo;
	int columna_mes_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_mes;
	int columna_dia_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_dia;
	int columna_hora_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_hora;
	int columna_minuto_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_minuto;

	for (int numero_fila = fila_inicial + 1; numero_fila < numero_filas; numero_fila++) {

		//Se carga el numero de terminal al cual esta conectado el elemento de la fila actual
		char* numero_terminal_elemento = informacion_vehiculos->datos[numero_fila][columna_terminal];

		//Si estan conectados al mismo terminal se comprueba que no estan conectados simultaneamente
		if (strings_iguales(numero_terminal_elemento, numero_terminal)) {
			
			//Se carga la fecha del vehiculo con el que se va a comparar si hay simultaneidad
			struct tm fecha_inicial_vehiculo = { 0 };
			struct tm fecha_final_vehiculo   = { 0 };

			cargar_fecha(informacion_vehiculos,&fecha_inicial_vehiculo, columna_anyo_inicial,
			columna_mes_inicial, columna_dia_inicial, columna_hora_inicial,
			columna_minuto_inicial, numero_fila, SI_INCLUIR_MINUTO);
			
			cargar_fecha(informacion_vehiculos,&fecha_final_vehiculo, columna_anyo_final,
			columna_mes_final, columna_dia_final, columna_hora_final, columna_minuto_final,
			numero_fila, SI_INCLUIR_MINUTO);

			if (comprobar_superposicion(fecha_inicial_vehiculo,fecha_final_vehiculo,
				                        *fecha_inicial_elemento,*fecha_final_elemento)) {

        
				char mensaje_error[512];
				snprintf(mensaje_error, 512, "Error en el terminal \n%s "
				"Hay un problema de simultaneidad con el vehiculo en la fila \n%d"
				"la fecha inicial del vehiculo es Año %d Mes %d Dia %d Hora %d Minuto \n%d"
				"la fecha final del vehiculo es Año %d Mes %d Dia %d Hora %d Minuto \n%d",
				numero_terminal,fila_inicial,fecha_inicial_vehiculo.tm_year+DESPLAZAMIENTO_ANYO, 
				fecha_inicial_vehiculo.tm_mon + DESPLAZAMIENTO_MES,
				fecha_inicial_vehiculo.tm_mday, fecha_inicial_vehiculo.tm_hour,
				fecha_inicial_vehiculo.tm_min, fecha_final_vehiculo.tm_year + DESPLAZAMIENTO_ANYO, 
				fecha_final_vehiculo.tm_mon + DESPLAZAMIENTO_MES,
				fecha_final_vehiculo.tm_mday, fecha_final_vehiculo.tm_hour, fecha_final_vehiculo.tm_min);
				 
				printf(mensaje_error);
				registrar_error(mensaje_error, REGISTRO_ERRORES);
				return ERROR;
			}
	}
	}
	return EXITO;
}

/*Este subprograma se utiliza para verificar que no hay problemas de simultaneidad con los vehiculos conectados
  en la electrolinera */

int verificar_simultaneidad_vehiculos(datos_csv_vehiculos_t* datos_vehiculos) {
	//Cargo la columna donde se encuentra la informacion terminal
	int columna_terminal = datos_vehiculos->posiciones_informacion_vehiculos.columna_terminales;

	//Cargo las columnas donde se encuentra la informacion de la fecha inicial
	int columna_anyo_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_anyo;
	int columna_mes_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_mes;
	int columna_dia_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_dia;
	int columna_hora_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_hora;
	int columna_minuto_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_minuto;
	
	//Cargo las columnas donde se encuentra la informacion de la fecha final
	int columna_anyo_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_anyo;
	int columna_mes_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_mes;
	int columna_dia_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_dia;
	int columna_hora_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_hora;
	int columna_minuto_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_minuto;

    //Cargo el puntero que apunta a donde se encuentra la informacion del vehiculo
	datos_csv_t* informacion_vehiculos = &(datos_vehiculos->informacion_vehiculos);

	//Cargo el numero de filas que hay en el csv de los vehiculos
	int numero_filas = informacion_vehiculos->filas;

	//Se crean dos variables para almacenar las fecha del vehiculo, 
	struct tm fecha_inicial_vehiculo;
	struct tm fecha_final_vehiculo;
	

	//Se crea una variable para almacenar el numero de terminal;
	char* numero_terminal; 
	
	for (int numero_vehiculo = 1; numero_vehiculo < numero_filas; numero_vehiculo ++ ) {

		//Se carga el numero de terminal al que está conectado el vehiculo
		numero_terminal = informacion_vehiculos->datos[numero_vehiculo][columna_terminal];

		//Se carga la fecha inicial del vehiculo
		cargar_fecha(informacion_vehiculos, &fecha_inicial_vehiculo, columna_anyo_vehiculo_inicial,
		columna_mes_vehiculo_inicial, columna_dia_vehiculo_inicial, columna_hora_vehiculo_inicial,
		columna_minuto_vehiculo_inicial,numero_vehiculo, SI_INCLUIR_MINUTO);

		//Se carga la fecha final del vehiculo 
		cargar_fecha(informacion_vehiculos, &fecha_final_vehiculo, columna_anyo_vehiculo_final,
		columna_mes_vehiculo_final, columna_dia_vehiculo_final, columna_hora_vehiculo_final,
		columna_minuto_vehiculo_final, numero_vehiculo, SI_INCLUIR_MINUTO);

		if (comprobar_simultaneidad_vehiculo(datos_vehiculos, &fecha_inicial_vehiculo,
			&fecha_final_vehiculo, numero_terminal, numero_vehiculo) == ERROR) {
			
			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Hay un problema de simultanead con el vehiculo en la fila %d\n", numero_vehiculo + 1);
			printf(mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}
		
	}
	return EXITO;
}

/*Subprograma para comprobar si una bateria en particular tiene problemas de simultaneidad*/

int comprobar_simultaneidad_baterias (datos_csv_baterias_t* datos_baterias,
	const struct tm* fecha_inicial_elemento, const struct tm* fecha_final_elemento,
	const char* numero_terminal, const int fila_inicial) {

	
	//Cargo el puntero que apunta a las posiciones en memoria donde se encuentra la informacion de los vehiculos
	datos_csv_t* informacion_baterias = &(datos_baterias->informacion_baterias);

	//Cargo el numero de filas que tiene el csv que contiene los datos.
	int numero_filas = informacion_baterias->filas;

	//Cargo algunas columnas y filas donde se encuentra la informacion
	int columna_terminal = datos_baterias->posiciones_informacion_baterias.columna_terminal;

	//Se carga las columnas donde se encuentra la fecha inicial
	int columna_anyo_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_anyo;
	int columna_mes_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_mes;
	int columna_dia_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_dia;
	int columna_hora_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_hora;
	int columna_minuto_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_minuto;

	//Se carga las columnas donde se encuentra la fecha final
	int columna_anyo_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_anyo;
	int columna_mes_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_mes;
	int columna_dia_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_dia;
	int columna_hora_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_hora;
	int columna_minuto_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_minuto;

	for (int numero_fila = fila_inicial+1 ; numero_fila < numero_filas; numero_fila++) {

		//Se carga el numero de terminal al cual esta conectado el elemento de la fila actual
		char* numero_terminal_elemento = informacion_baterias->datos[numero_fila][columna_terminal];

		
		//Si estan conectados al mismo terminal se comprueba que no estan conectados simultaneamente
		if (strings_iguales(numero_terminal_elemento, numero_terminal)) {

			
			//Se carga la fecha del vehiculo con el que se va a comparar si hay simultaneidad
			struct tm fecha_inicial_bateria = { 0 };
			struct tm fecha_final_bateria = { 0 };

			cargar_fecha(informacion_baterias, &fecha_inicial_bateria, columna_anyo_inicial,
				columna_mes_inicial, columna_dia_inicial, columna_hora_inicial,
				columna_minuto_inicial, numero_fila, SI_INCLUIR_MINUTO);

			cargar_fecha(informacion_baterias, &fecha_final_bateria, columna_anyo_final,
				columna_mes_final, columna_dia_final, columna_hora_final, columna_minuto_final,
				numero_fila, SI_INCLUIR_MINUTO);

			if (comprobar_superposicion(fecha_inicial_bateria, fecha_final_bateria,
				*fecha_inicial_elemento, *fecha_final_elemento)) {

				char mensaje_error[512];
				snprintf(mensaje_error, 512, "Error en el terminal %s \n "
					"Hay un problema de simultaneidad con la bateria en la fila %d \n"
					"la fecha inicial de la bateria es Año %d Mes %d Dia %d Hora %d Minuto %d \n"
					"la fecha final de la bateria es Año %d Mes %d Dia %d Hora %d Minuto %d \n",
					numero_terminal, fila_inicial, fecha_inicial_bateria.tm_year + DESPLAZAMIENTO_ANYO,
					fecha_inicial_bateria.tm_mon + DESPLAZAMIENTO_MES,
					fecha_inicial_bateria.tm_mday, fecha_inicial_bateria.tm_hour,
					fecha_inicial_bateria.tm_min, fecha_final_bateria.tm_year + DESPLAZAMIENTO_ANYO,
					fecha_final_bateria.tm_mon + DESPLAZAMIENTO_MES,
					fecha_final_bateria.tm_mday, fecha_final_bateria.tm_hour, fecha_final_bateria.tm_min);

				printf(mensaje_error);
				registrar_error(mensaje_error, REGISTRO_ERRORES);
				return ERROR;
			}
		}
	}
	return EXITO;
}

/*Se utiliza este subprograma para verificar que no hay baterias conectadas al mismo terminal en el mismo 
* tiempo
*/

int verificar_simultaneidad_baterias(datos_csv_baterias_t* datos_baterias) {
	//Cargo la columna donde se encuentra el  numero de terminal
	int columna_terminal = datos_baterias->posiciones_informacion_baterias.columna_terminal;
	
	
	//Cargo donde se encuentra la fecha inicial 
	int columna_anyo_bateria_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_anyo;
	int columna_mes_bateria_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_mes;
	int columna_dia_bateria_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_dia;
	int columna_hora_bateria_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_hora;
	int columna_minuto_bateria_inicial = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_minuto;

	//Cargo donde se encuentra la fecha final 
	int columna_anyo_bateria_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_anyo;
	int columna_mes_bateria_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_mes;
	int columna_dia_bateria_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_dia;
	int columna_hora_bateria_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_hora;
	int columna_minuto_bateria_final = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_minuto;

	//Cargo el puntero donde se encuentra la informacion de la bateria
	datos_csv_t* informacion_baterias = &(datos_baterias->informacion_baterias);

	//Cargo el numero de filas de las baterias
	int numero_filas_baterias = informacion_baterias->filas;

	//Se crea variables para almacenar la fecha de conexión y desconexión de las baterías
	struct tm fecha_inicial_bateria;
	struct tm fecha_final_bateria;

	//Se crea una variable para almacenar el numero de terminal
	char* numero_terminal;

	for (int numero_bateria = 1; numero_bateria < numero_filas_baterias; numero_bateria++) {

		//Se carga el numero de terminal al que está conectado el vehiculo
		numero_terminal = informacion_baterias->datos[numero_bateria][columna_terminal];

		//Se carga la fecha inicial de la bateria
		cargar_fecha(informacion_baterias, &fecha_inicial_bateria, columna_anyo_bateria_inicial,
			columna_mes_bateria_inicial, columna_dia_bateria_inicial, columna_hora_bateria_inicial,
			columna_minuto_bateria_inicial, numero_bateria, SI_INCLUIR_MINUTO);

		//Se carga la fecha final del vehiculo 
		cargar_fecha(informacion_baterias, &fecha_final_bateria, columna_anyo_bateria_final,
			columna_mes_bateria_final, columna_dia_bateria_final, columna_hora_bateria_final,
			columna_minuto_bateria_final, numero_bateria, SI_INCLUIR_MINUTO);

		if (comprobar_simultaneidad_baterias(datos_baterias, &fecha_inicial_bateria,
			&fecha_final_bateria, numero_terminal, numero_bateria) == ERROR) {

			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Hay un problema de simultanead con el vehiculo en la fila %d\n", numero_bateria + 1);
			printf(mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}

	}
	return EXITO;

}
int verificar_simultaneidad_cruzada_vehiculos_baterias(datos_csv_vehiculos_t* datos_vehiculos, datos_csv_baterias_t* datos_baterias) {
	//Cargo la columna donde se encuentra la informacion terminal
	int columna_terminal = datos_vehiculos->posiciones_informacion_vehiculos.columna_terminales;

	//Cargo las columnas donde se encuentra la informacion de la fecha inicial
	int columna_anyo_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_anyo;
	int columna_mes_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_mes;
	int columna_dia_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_dia;
	int columna_hora_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_hora;
	int columna_minuto_vehiculo_inicial = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_minuto;

	//Cargo las columnas donde se encuentra la informacion de la fecha final
	int columna_anyo_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_anyo;
	int columna_mes_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_mes;
	int columna_dia_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_dia;
	int columna_hora_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_hora;
	int columna_minuto_vehiculo_final = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_minuto;

	//Cargo el puntero que apunta a donde se encuentra la informacion del vehiculo
	datos_csv_t* informacion_vehiculos = &(datos_vehiculos->informacion_vehiculos);

	//Cargo el numero de filas que hay en el csv de los vehiculos
	int numero_filas = informacion_vehiculos->filas;

	//Se crean dos variables para almacenar las fecha del vehiculo, 
	struct tm fecha_inicial_vehiculo;
	struct tm fecha_final_vehiculo;


	//Se crea una variable para almacenar el numero de terminal;
	char* numero_terminal;

	for (int numero_vehiculo = 1; numero_vehiculo < numero_filas; numero_vehiculo++) {

		//Se carga el numero de terminal al que está conectado el vehiculo
		numero_terminal = informacion_vehiculos->datos[numero_vehiculo][columna_terminal];

		//Se carga la fecha inicial del vehiculo
		cargar_fecha(informacion_vehiculos, &fecha_inicial_vehiculo, columna_anyo_vehiculo_inicial,
			columna_mes_vehiculo_inicial, columna_dia_vehiculo_inicial, columna_hora_vehiculo_inicial,
			columna_minuto_vehiculo_inicial, numero_vehiculo, SI_INCLUIR_MINUTO);

		//Se carga la fecha final del vehiculo 
		cargar_fecha(informacion_vehiculos, &fecha_final_vehiculo, columna_anyo_vehiculo_final,
			columna_mes_vehiculo_final, columna_dia_vehiculo_final, columna_hora_vehiculo_final,
			columna_minuto_vehiculo_final, numero_vehiculo, SI_INCLUIR_MINUTO);

		

		if (comprobar_simultaneidad_baterias(datos_baterias, &fecha_inicial_vehiculo,
			&fecha_final_vehiculo, numero_terminal,INICIO_CSV) == ERROR) {

			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Hay un problema de simultanead con el vehiculo en la fila %d\n", numero_vehiculo + 1);
			printf(mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}

	}
	return EXITO;
}
int verificar_simultaneidad_vehiculos_baterias(datos_csv_vehiculos_t* datos_vehiculos,datos_csv_baterias_t* datos_baterias) {

	if (verificar_simultaneidad_vehiculos(datos_vehiculos) == ERROR) {
		printf("Hay vehiculos que estan conectados al mismo terminal al mismo tiempo \n");
		registrar_error("Hay vehiculos que estan conectados al mismo terminal al mismo tiempo\n", REGISTRO_ERRORES);
		return ERROR;
	}
  
	
	if (verificar_simultaneidad_baterias(datos_baterias) == ERROR) {
		printf("Hay baterias conectadas al mismo terminal al mismo tiempo \n");
		registrar_error("Hay baterias conectadas al mismo terminal al mismo tiempo\n", REGISTRO_ERRORES);
		return ERROR;
	}

  
	if (verificar_simultaneidad_cruzada_vehiculos_baterias(datos_vehiculos, datos_baterias) == ERROR) {
		printf("Hay vehiculos y baterias conectadas al mismo tiempo \n");
		registrar_error("Hay vehiculos y baterias conectadas al mismo terminal al mismo tiempo\n", REGISTRO_ERRORES);
		return ERROR;
	}

  
	return EXITO;
}