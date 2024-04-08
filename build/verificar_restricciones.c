
//*+++++++++++++++++++++++ Pendiente de revision

#include "conversion_numeros.h"
#include "definiciones_globales.h"
#include "operaciones_csv.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "validaciones.h"
#include <stdio.h>

//Defino las dimensiones que ha de tener
//el CSV de las restricciones 

#define NUMERO_FILAS_CSV_RESTRICCIONES    2
#define NUMERO_COLUMNAS_CSV_RESTRICCIONES 14


static int comprobar_dimension_csv_restricciones(datos_csv_t* datos_restricciones) {
	//Este subprograma se utiliza
	//para verificar que las dimensiones
	//del CSV de las restricciones son
	//correctas.

	int numero_columnas = datos_restricciones->columnas;
	int numero_filas = datos_restricciones->filas;

	if ((numero_columnas != NUMERO_COLUMNAS_CSV_RESTRICCIONES) || (numero_filas != NUMERO_FILAS_CSV_RESTRICCIONES)) {

		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error el csv de las restricciones ha de tener %d columnas y %d filas\n",
			numero_columnas, numero_filas);

		printf("%s\n", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);

		return ERROR;
	}
	return EXITO;
}


//Subprograma que se utiliza para
//comprobar si un dato del CSV de
//restricciones es un numero no
//negativo.


static int comprobar_dato_restricciones(char* dato) {

	if (!es_un_numero(dato, DECIMAL_INCLUIDO)) {
		return ERROR;
	}

	double valor = convertir_a_decimal(dato, &valor);

	if (es_negativo(valor)) {
		return ERROR;
	}
	return EXITO;
}

//Este subprograma se utiliza para
//comprobar que los datos del
//CSV de las restricciones son
//Numeros validos.

static int comprobar_formato_csv_restricciones(datos_csv_restricciones_t* datos_restricciones) {

	//Cargo el puntero que apunta a la posicion en memoria donde se enecuentran las restricciones
	//del sistema
	datos_csv_t* informacion_restricciones = &(datos_restricciones->informacion_restricciones);

	//Cargo la primera columna donde se encuentra la informacion de las restricciones
	int primera_columna_datos_restricciones = datos_restricciones->posiciones_informacion_restricciones.columna_max_potencia_sistema;

	//Cargo el numero de filas y de columnas
	int numero_filas = informacion_restricciones->filas;
	int numero_columnas = informacion_restricciones->columnas;

	//Cargo la fila donde se encuentea la informacion del csv de las restricciones->
	int fila_informacion_csv_restricciones = datos_restricciones->posiciones_informacion_restricciones.fila_valores;

	for (int numero_columna = primera_columna_datos_restricciones; numero_columna < numero_columnas; numero_columna++) {



		if (comprobar_dato_restricciones(informacion_restricciones->datos[fila_informacion_csv_restricciones][numero_columna]) == ERROR) {

			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error el dato en la fila %d y la columna %d el dato ha de ser numerico y positivo",
				fila_informacion_csv_restricciones, numero_columna);

			printf("%s\n", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);



			return ERROR;
		}
	}
	return EXITO;
}

//Este subprograma se utiliza
//para verificar que los
//encabezados del CSV de
//las restricciones son
//correctos.

static int comprobar_encabezados_restricciones(datos_csv_t* datos_restricciones) {


	const char* valores_aceptables[] = { "Variable Sistema","Maxima Potencia Electrolinera Red","Maxima Potencia Terminal 1 kWh",
											"Maxima Potencia Terminal 2 kWh","Maxima Potencia Terminal 3 kWh",
											"Maxima Potencia Terminal 4 kWh","Maxima Potencia Terminal 5 kWh",
											"Maxima Potencia Terminal 6 kWh","Maxima Potencia Terminal 7 kWh",
											"Maxima Potencia Terminal 8 kWh","Maxima Potencia Terminal 9 kWh",
                      "Maxima Potencia Terminal 10 kWh","Maxima Potencia Terminal 11 kWh",
											"Maxima Potencia Terminal 12 kWh" };

	const int numero_encabezados = sizeof(valores_aceptables) / sizeof(valores_aceptables[0]);

	if (verificar_encabezado_csv(datos_restricciones, valores_aceptables,
		numero_encabezados, "Datos de las restricciones") == ERROR) {

		printf("Los encabezados del CSV de las restricciones son incorrectos\n");
		registrar_error("Los encabezados del CSV de las restricciones son incorrectos\n", REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}

//Este subprograma se utiliza
//para verificar que los datos
//en el CSV de las restricciones
//son correctas->

int verificar_datos_restricciones(datos_csv_restricciones_t* datos_restricciones) {

	//Cargo el puntero que apunta a las posicion en memoria donde se encuentra
	//la informacion en memoria de las restricciones

	datos_csv_t* informacion_restricciones = &(datos_restricciones->informacion_restricciones);

	if (comprobar_dimensiones_csv_fijo(informacion_restricciones, NUMERO_FILAS_CSV_RESTRICCIONES,
		NUMERO_COLUMNAS_CSV_RESTRICCIONES, "Restricciones del Sistema") == ERROR) {

		printf("Error en las dimensiones del CSV de las restricciones\n");
		registrar_error("Error en las dimensiones del CSV de las restricciones", REGISTRO_ERRORES);
		return ERROR;
	}
	if (comprobar_encabezados_restricciones(informacion_restricciones) == ERROR) {
		printf("Error los encabezados del CSV de las restricciones son incorrectas\n");
		registrar_error("Error los encabezados del CSV de las restricciones son incorrectas", REGISTRO_ERRORES);
		return ERROR;
	}

	if (comprobar_formato_csv_restricciones(datos_restricciones) == ERROR) {
		printf("Error en los datos de las restricciones\n");
		registrar_error("Error en los datos de las restricciones\n", REGISTRO_ERRORES);
		return ERROR;
	}

	return EXITO;
}