
#include "conversion_numeros.h"
#include "definiciones_globales.h"
#include "operaciones_csv.h"
#include "operaciones_fecha.h"
#include "operaciones_string.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "validaciones.h"
#include <stdio.h>
#include <stdbool.h>

#define MINIMO_FILAS_CSV_VEHICULOS    1
#define NUMERO_COLUMNAS_CSV_VEHICULOS 16

//Este subprograma se utiliza para
//comprobar que las dimensiones
//del CSV de los vehiculos son
//correctos.


static int comprobar_dimensiones_csv_vehiculos(datos_csv_t* datos_vehiculos) {


	if (comprobar_dimensiones_csv_variable(datos_vehiculos, MINIMO_FILAS_CSV_VEHICULOS,
		NUMERO_COLUMNAS_CSV_VEHICULOS, "Vehiculos") == ERROR) {
		printf("El CSV de los vehiculos no tienen dimensiones correctas\n");
		registrar_error("El CSV de los vehiculos no tienen dimensiones correctas", REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}

//Este subprograma se utiliza para 
//verificar que los encabezados del 
//CSV que contiene la informacion
//de los vehiculos es correcta.s



int comprobar_encabezados_vehiculos(datos_csv_t* datos_vehiculos) {


	const char* valores_aceptables[] = { "Terminal","Modo Carga","Capacidad Bateria",
										   "Bateria Inicial","Bateria Final","Maxima Potencia",
										   "Año Inicial Carga","Mes Inicial Carga","Dia Inicial Carga",
										   "Hora Inicial Carga","Minuto Inicial Carga","Año Final Carga",
										   "Mes Final Carga","Dia Final Carga","Hora Final Carga",
										   "Minuto Final Carga" };
	const int numero_encabezados = sizeof(valores_aceptables) / sizeof(valores_aceptables[0]);

	if (verificar_encabezado_csv(datos_vehiculos, valores_aceptables, numero_encabezados,
		"Vehiculos") == ERROR) {
		printf("Los encabezados del CSV de los vehiculos son incorrectos\n");
		registrar_error("Los encabezados del CSV de los vehiculos son incorrectos", REGISTRO_ERRORES);
		return ERROR;
	}


	return EXITO;
}

/*Este subprograma se utiliza para comprobar que la fase
  a la que está conectada el vehículo sea R,S o T*/

static int verificar_fase_vehiculo(datos_csv_vehiculos_t* datos_vehiculos,datos_csv_terminales_t* datos_terminales) {


  //Se carga el numero de filas que tiene el csv
  int numero_filas_csv_vehiculos = datos_vehiculos->informacion_vehiculos.filas;
  //Se carga la columna donde esta el terminal al que está conectado en el csv de los vehículos
  int columna_terminal = datos_vehiculos->posiciones_informacion_vehiculos.columna_terminales;
  //Se carga la columna donde está la fase del terminal en el csv que indican a que fase están
  //conectados los terminales
  int columna_fase = datos_terminales->posiciones_informacion_terminales.columna_fase;
  //Variable booleana para 
  bool fase_correcta;

  for (int numero_fila = 1; numero_fila < numero_filas_csv_vehiculos; numero_fila++) {

    char* terminal = datos_vehiculos->informacion_vehiculos.datos[numero_fila][columna_terminal];
    int   numero_terminal;
    fase_correcta = false;

    if (convertir_a_entero(terminal, &numero_terminal) == ERROR) {
      printf("Ha habido un error comprobando la fase a la que están conectados los vehículos\n");
      registrar_error("Ha habido un error comprobando la fase a la que están conectados los vehículos", REGISTRO_ERRORES);
      return ERROR;
    }

    
    //Se accede a la fase la que está conectado el vehiculo
    char* fase = datos_terminales->informacion_terminales.datos[numero_terminal][columna_fase];
    const char* valores_aceptables[] = { "R","S","T" };
    const int numero_valores_aceptables = sizeof(valores_aceptables) / sizeof(valores_aceptables[0]);

    for (int i = 0; i < numero_valores_aceptables; i++) {
      if (strings_iguales(valores_aceptables[i], fase)) {
        fase_correcta = true;
      }
    }

    if (fase_correcta == false) {
      printf("Error los vehiculos deben estar conectados a fase R,S o T\n");
      printf("Error en la fila %d\n", numero_fila);
      registrar_error("Error en la fila %d\n",REGISTRO_ERRORES);
      registrar_error("Error los vehiculos deben estar conectados a fase R,S o T\n", REGISTRO_ERRORES);
      return ERROR;

    
    }
  }

  return EXITO;
}






//Este subprograma se utiliza
//para verificar que los modos
//de carga de los Vehiculos 
//estan escritos correctamente.

static int verificar_modo_carga(const char* modo_carga, const int numero_fila) {


	if (strings_iguales(modo_carga, "Rapida") || strings_iguales(modo_carga, "Normal")) {
		return EXITO;
	}
	char mensaje_error[512];
	snprintf(mensaje_error, sizeof(mensaje_error) / sizeof(char), "Error el modo de carga de los vehiculos debe ser Normal o Rapida y es %s\n", modo_carga);
	printf("%s", mensaje_error);

	registrar_error(mensaje_error, REGISTRO_ERRORES);
	return ERROR;
}

//Este subprograma se utiliza para verificar los
//datos relacionados con la bateria y la maxima
//potencia que puede intercambiar el vehiculo.

static int verificar_datos_bateria_vehiculo(datos_csv_vehiculos_t* datos_csv, const int numero_fila) {

	//Obtengo el puntero que apunta a las posiciones de memoria donde se encuentra la informacion
	//de los vehiculos
	datos_csv_t* informacion_vehiculos = &(datos_csv->informacion_vehiculos);

	//Cargo las columna donde se encuentra la informacion
	int columna_csv_vehiculos_capacidad = datos_csv->posiciones_informacion_vehiculos.columna_capacidad_bateria;
	int columna_csv_vehiculos_max_potencia = datos_csv->posiciones_informacion_vehiculos.columna_maxima_potencia;
	int columna_csv_vehiculos_bat_inicial = datos_csv->posiciones_informacion_vehiculos.columna_porcentaje_bateria_inicial;
	int columna_csv_vehiculos_bat_deseada = datos_csv->posiciones_informacion_vehiculos.columna_porcentaje_bateria_deseada;

	char* capacidad_bateria = informacion_vehiculos->datos[numero_fila][columna_csv_vehiculos_capacidad];
	char* maxima_potencia = informacion_vehiculos->datos[numero_fila][columna_csv_vehiculos_max_potencia];
	char* bateria_inicial = informacion_vehiculos->datos[numero_fila][columna_csv_vehiculos_bat_inicial];
	char* bateria_deseada = informacion_vehiculos->datos[numero_fila][columna_csv_vehiculos_bat_deseada];

	if (!es_un_numero(capacidad_bateria, DECIMAL_INCLUIDO)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error la bateria del vehiculo de la fila %d del CSV de los vehiculos no es un numero",
			numero_fila);
		printf("%s\n", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	if (!es_un_numero(maxima_potencia, DECIMAL_INCLUIDO)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error la maxima potencia que puede intercambiar el vehiculo de la fila %d del CSV de los vehiculos no es valido\n", numero_fila);
		printf("%s\n", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	if (!es_un_numero(bateria_inicial, DECIMAL_INCLUIDO)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error),
			"Error el porcentaje de bateria inicial del vehiculo de la fila %d del CSV de los vehiculos no es un numero valido", numero_fila);
		printf("%s\n", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	if (!es_un_numero(bateria_deseada, DECIMAL_INCLUIDO)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error),
			"Error el porcentaje de bateria deseada del vehiculo de la fila %d del CSV de los vehiculos no es un numero valido", numero_fila);
		printf("%s\n", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	double capacidad_bateria_num = 0;
	double maxima_potencia_num = 0;
	double bateria_inicial_num = 0;
	double bateria_deseada_num = 0;

	if ((convertir_a_decimal(capacidad_bateria, &capacidad_bateria_num) == ERROR) ||
		(convertir_a_decimal(maxima_potencia, &maxima_potencia_num) == ERROR) ||
		(convertir_a_decimal(bateria_inicial, &bateria_inicial_num) == ERROR) ||
		(convertir_a_decimal(bateria_deseada, &bateria_deseada_num) == ERROR)) {


		printf("No se ha podido convertir a dato numerico la informacion de la bateria\n");
		registrar_error("No se ha podido convertir a dato numerico la informacion de la bateria\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (!comprobar_porcentaje_bateria(bateria_inicial_num)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error en la fila %d,el porcentaje de bateria inicial ha de estar entre 0 y 1\n", numero_fila);
		printf("%s\n", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	if (!comprobar_porcentaje_bateria(bateria_deseada_num)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error en la fila %d el porcentaje de bateria ha de estar entre 0 y 1\n", numero_fila);
		printf("%s", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	if (es_negativo(capacidad_bateria_num)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error en la fila %d del CSV de los vehiculos la capacidad de la bateria no puede ser negativa\n", numero_fila);
		printf("%s", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}

	if (es_negativo(maxima_potencia_num)) {
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "Error en la fila %d del CSV de los vehiculos, la potencia maxima del vehiculo no puede ser negativa\n",
			numero_fila);
		registrar_error(mensaje_error, REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;

}

//Este subprograma se utiliza para
//comprobar que los datos de los 
//vehiculos son correctos.


static int comprobar_datos_csv_vehiculos(datos_csv_vehiculos_t* datos_vehiculos, datos_csv_algoritmo_t* datos_algoritmo,
                                         datos_csv_terminales_t* datos_terminales) {

	//Cargo el puntero que apunta a la posicion de memoria donde se encuentra la informacion de los
	//vehiculos y el algoritmo
	datos_csv_t* informacion_vehiculos = &(datos_vehiculos->informacion_vehiculos);
	datos_csv_t* informacion_algoritmo = &(datos_algoritmo->informacion_algoritmo);

	//Cargo el numero de filas que hay en el csv
	int numero_filas = informacion_vehiculos->filas;

	//Se carga las columnas donde se encuentra la diferente informacion en el csv.
	int columna_vehiculos_terminal = datos_vehiculos->posiciones_informacion_vehiculos.columna_terminales;
	int columna_modo_carga = datos_vehiculos->posiciones_informacion_vehiculos.columna_modo_carga;

	//Se van comprobando los datos de 
	//la fila del CSV, para comprobar
	//que son correctos.

	for (int numero_fila = 1; numero_fila < numero_filas; numero_fila++) {

		if (verificar_numero_terminal(informacion_vehiculos->datos[numero_fila][columna_vehiculos_terminal], numero_fila) == ERROR) {

			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "El campo del numero del terminal en la fila %d es incorrecto", numero_fila);
			printf("%s", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}


		if (verificar_modo_carga(informacion_vehiculos->datos[numero_fila][columna_modo_carga], numero_fila) == ERROR) {
			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "El Modo de carga en la fila % d del CSV de los vehiculos es incorrecto\n", numero_fila);
			printf("%s", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}

		if (verificar_datos_bateria_vehiculo(datos_vehiculos, numero_fila) == ERROR) {
			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Los datos de la bateria del vehiculo en la fila %d son incorrectos",
				numero_fila);
			printf(mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}

		if (verificar_fecha_carga(informacion_vehiculos, datos_algoritmo, numero_fila) == ERROR) {
			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Los datos de fecha del vehiculo en la fila %d son incorrectos", numero_fila);
			printf(mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}

	}

  if (verificar_fase_vehiculo(datos_vehiculos, datos_terminales) == ERROR) {
    char mensaje_error[512];
    printf("La fase a la que están conectados los vehículos debe ser R,S o T \n");
    registrar_error(mensaje_error, REGISTRO_ERRORES);
    printf("Llega aquí\n");
    return ERROR;
  }

	return EXITO;
}

int verificar_datos_vehiculos(datos_csv_vehiculos_t* datos_vehiculos, datos_csv_algoritmo_t* datos_algoritmo,
                              datos_csv_terminales_t* datos_terminales) {
	//Este subprograma se utiliza para
	//comprobar que el CSV 
	//que contiene los datos de los 
	//vehiculos es correcto->

	if (comprobar_dimensiones_csv_vehiculos(&(datos_vehiculos->informacion_vehiculos)) == ERROR) {
		return ERROR;
	}
	if (comprobar_encabezados_vehiculos(&(datos_vehiculos->informacion_vehiculos)) == ERROR) {
		return ERROR;
	}
	if (comprobar_datos_csv_vehiculos(datos_vehiculos, datos_algoritmo,datos_terminales) == ERROR) {
		return ERROR;
	}

	return EXITO;

}