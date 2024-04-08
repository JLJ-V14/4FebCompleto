
#include "conversion_numeros.h"
#include "definiciones_globales.h"
#include  "operaciones_string.h"
#include  "operaciones_csv.h"
#include  "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "validaciones.h"
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

//Defino las dimensiones que tiene el CSV de la informacion de los terminales
#define   NUMERO_FILAS_CSV_TERMINALES    13
#define   NUMERO_COLUMNAS_CSV_TERMINALES 2

//Defino los valores que han de tener los encabezados->

#define  ENCABEZADO_NUMERO_TERMINAL L"Numero Terminal"
#define  ENCABEZADO_FASE            L"Fase "
//Defino las posiciones en el CSV donde se encuentra alguna
//informacion->

#define  COLUMNA_ENCABEZADOS  0

	//Este subprogram se utiliza para commprobar
	//que  el dato de numero de terminal es
	//correcto, comprobando que :
	//Es un numero y que
	//Los terminales son consecutivos.

static int comprobar_numero_terminal(char* terminal, const int terminal_actual) {


	if (!es_un_numero(terminal, DECIMAL_NO_INCLUIDO)) {
		printf("El numero del terminal ha de ser un dato numerico sin decimales \n");
		registrar_error("El numero del terminal ha de ser un dato numerico sin decimales\n", REGISTRO_ERRORES);
		return ERROR;
	}

	int numero_terminal;
	if (convertir_a_entero(terminal, &numero_terminal) == ERROR) {
		printf("La conversión de caracter a numero entero no ha sido exitosa \n");
		registrar_error("No se ha podido convertir los caracteres de tipo wchar a tipo numerico\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (numero_terminal != terminal_actual) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error el numero de terminal ha de ser %d y es %d\n",
			terminal_actual, numero_terminal);
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}
//Este subprograma se utiliza para comprobar
//que la fase asignada a cada terminal 
//corresponde con R S T N o Neutro y Nada

static int comprobar_fase_terminal(const char* fase) {


	const char* valores_aceptables[] = { "R","S","T","Neutro","Nada" };
	const int numero_valores_aceptables = sizeof(valores_aceptables) / sizeof(valores_aceptables[0]);


	for (int i = 0; i < numero_valores_aceptables; i++) {
		if (strings_iguales(fase, valores_aceptables[i])) {
			return EXITO;
		}
	}

	printf("Error la fase debe ser R,S,T,Neutro o Nada\n");
	registrar_error("Error la fase debe ser R,S,T Neutro o Nada", REGISTRO_ERRORES);
	return ERROR;

}
//Este subprograma se utiliza para comprobar
//que la informacion de los diferentes terminales
//es correcta.
//Se comprueba que los numeros de los terminales
//estan asignados correctamente, y la fase de estos
//es un valor valido, R S T Neutro o nada.

static int comprobar_informacion_terminales(datos_csv_terminales_t* datos_terminales) {

	//Cargo el las columnas en las que se encuentra la informacion
	int columna_numero_terminal = datos_terminales->posiciones_informacion_terminales.columna_numero_terminal;
	int columna_fase = datos_terminales->posiciones_informacion_terminales.columna_fase;

	//Cargo el puntero que apunta a la posicion de memoria donde se encuentra la informacion relativa 
	//a los terminales.
	datos_csv_t* informacion_terminales = &(datos_terminales->informacion_terminales);

	for (int numero_terminal = 1; numero_terminal <= NUMERO_TERMINALES; numero_terminal++) {

		if (comprobar_numero_terminal(informacion_terminales->datos[numero_terminal][columna_numero_terminal],
			numero_terminal) == ERROR) {

			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error el Numero de terminal en la fila %d no es correcto",
				numero_terminal + 1);
			printf("%s\n", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}
		if (comprobar_fase_terminal(informacion_terminales->datos[numero_terminal][columna_fase]) == ERROR) {

			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error el Numero de terminal en la fila %d no es correcto",
				numero_terminal + 1);
			printf("%s\n", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;

		}
	}
	return EXITO;
}

//Este subprograma se utiliza para
//verificar que los encabezados del 
//CSV de los terminales son correctos->

static int verificar_encabezados_terminales(datos_csv_t* datos_terminales) {


	const char* valores_aceptables[] = { "Numero Terminal","Fase" };
	const int numero_encabezados = sizeof(valores_aceptables) / sizeof(valores_aceptables[0]);

	if (verificar_encabezado_csv(datos_terminales, valores_aceptables,
		numero_encabezados, "Datos de los terminales de la placa de redistribucion") == ERROR) {
		printf("Los encabezados del CSV de los datos de los terminales son incorrectos\n");
		registrar_error("Los encabezados del csv de los terminales son incorrectos\n",
			REGISTRO_ERRORES);
		return ERROR;
	}


	return EXITO;
}

/*  Este subprograma se utiliza para verificar que la
	informacion en el CSV de los terminales es correcto:
	Se verificara que las dimensiones del CSV son las esperadas
	y que los terminales estan en orden consecutivo, y que las fases
	son las esperadas.
	Obtengo el puntero que apunta a la posicion en memoria donde
*/

int verificar_datos_terminales(datos_csv_terminales_t* datos_terminales) {


	datos_csv_t* informacion_terminales = &(datos_terminales->informacion_terminales);

	if (comprobar_dimensiones_csv_fijo(informacion_terminales, NUMERO_FILAS_CSV_TERMINALES,
		NUMERO_COLUMNAS_CSV_TERMINALES, "Informacion Terminales") == ERROR) {
		printf("Las Dimensiones del CSV de los terminales son incorrectas\n");
		registrar_error("Las Dimensiones del CSV de los terminales son incorrectas\n",
			REGISTRO_ERRORES);
		return ERROR;
	}

	//Se verifica que los encabezados del csv de los terminales son correctos.
	if (verificar_encabezados_terminales(informacion_terminales) == ERROR) {
		printf("Los encabezados en el CSV de los terminales son incorrectos\n");
		registrar_error("Las Dimensiones del CSV de los terminales son incorrectas\n",
			REGISTRO_ERRORES);
		return ERROR;
	}

	//Se comprueban que los datos de los terminales sean correctos
	if (comprobar_informacion_terminales(datos_terminales) == ERROR) {
		printf("Los Datos del CSV de los terminales son incorrectos\n");
		registrar_error("Las Dimensiones del CSV de los terminales son incorrectas\n",
			REGISTRO_ERRORES);
		return ERROR;
	}

	return EXITO;
}