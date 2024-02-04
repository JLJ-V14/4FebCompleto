#include "conversion_numeros.h"
#include "definiciones_Globales.h"
#include "locale.h"
#include "operaciones_csv.h"
#include "operaciones_fecha.h"
#include "operaciones_string.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "validaciones.h"

#include <stdio.h>
#include <wchar.h>
//Se definen una serie de constantes
//El numero de columnas que ha de
//tener el CSV del precio

//El numero de filas que ha de tener 
//el CSV de las columnas.

//La primera fila donde comienza la
//informacion

#define NUMERO_COLUMNAS_TIEMPO_PRECIO 4
#define NUMERO_COLUMNAS_PRECIO        5
#define NUMERO_MINIMO_FILAS_PRECIO    2
#define PRIMERA_FILA_CSV_PRECIO       1

#define PRIMERA_COLUMNA               0
#define FILA_ENCABEZADOS              0
#define SEGUNDOS_HORA                 3600
#define SEGUNDOS_DOS_HORAS            7200
#define COLUMNA_PRECIOS               4





/*  Este subprograma se utiliza para comprobar
 *  que los datos de los encabezados son
 *  correctos.
 */

static int comprobar_encabezados_csv_precios(datos_csv_precio_t* datos_precio) {
	/*
	Defino variables que indican en que posiciones del csv
	se encuentra la informacion
	*/

	int fila_encabezados = datos_precio->posiciones_informacion_precio.fila_encabezados;
	int numero_columnas = datos_precio->informacion_precio.columnas;

	/*
	Obtengo el puntero que apunta a la memoria donde se
	encuentra la informacion relativa a los precios de
	la electricidad.
	*/

	datos_csv_t* informacion_precio = &(datos_precio->informacion_precio.datos);
	const wchar_t* encabezados[] = { L"Año",L"Mes",L"Dia",L"Hora",L"Precio kWh" };

	for (int numero_columna = 0; numero_columna < numero_columna; numero_columnas++) {

		if (!strings_iguales(informacion_precio->datos[fila_encabezados][numero_columna],
			encabezados[numero_columna])) {

			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error el encabezado en la columna %d deberia ser %ls y es %ls",
				numero_columna, encabezados[numero_columna], informacion_precio->datos[fila_encabezados][numero_columna]);
			printf("%s\n", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}
	}
	return EXITO;
}

/*
Este subprograma se utiliza para comprobar que
los datos temporales relacionados con los
precios son numeros enteros, sin decimales.
*/

static int comprobar_formato_dato_tiempo_precio(datos_csv_t* datos_precio, const int numero_columna) {


	int numero_filas = datos_precio->filas;

	for (int numero_fila = PRIMERA_FILA_CSV_PRECIO; numero_fila < numero_filas; numero_fila++) {

		if (!es_un_numero(datos_precio->datos[numero_fila][numero_columna], DECIMAL_NO_INCLUIDO)) {
			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error en la fila %d y columna %d las fechas de los precios no pueden tener decimales \n",
				numero_fila + 1, numero_columna + 1);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}

	}
	return EXITO;
}

/*
En este subprograma se comprueba
que el dato de precio en el CSV
es correcto, es decir que sea
un numero y que ademas no sea
negativo.
*/


static int comprobar_precio(const datos_csv_t* datos_precio, const char* nombre_archivo) {

	/*
	Declaro variables auxiliares
	En una se guarda el precio
	en formato string otra en
	formato numerico
	*/

	char* precio_string;
	double    precio_num;

	int numero_filas = datos_precio->filas;

	/*
	Se comprueba que:
	Los precios sean valores numericos.
	Se pueden convertir a numeros.
	Que no sean negativos.
	*/

	for (int numero_fila = 1; numero_fila < numero_filas; numero_fila++) {

		precio_string = datos_precio->datos[numero_fila][COLUMNA_PRECIOS];
   // printf("%s\n", precio_string);
    //setlocale(LC_ALL, "C");

		if ((!es_un_numero(precio_string, DECIMAL_INCLUIDO))) {
			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error en la fila %d del CSV de %s de los precios hay un precio que no es un numero\n",
				numero_fila, nombre_archivo);


			printf("%s\n", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}
		
		if ((convertir_a_decimal(precio_string, &precio_num)) == ERROR) {
		
			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error en la fila %d del Csv de %s no se ha podido convertir a numero correctamente",
				numero_fila, nombre_archivo);
			printf("%s\n", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}

		if (es_negativo(precio_num)) {
			char mensaje_error[512];
			snprintf(mensaje_error, sizeof(mensaje_error), "Error en la fila %d del Csv de %s no puede ser negativo a numero correctamente",
				numero_fila, nombre_archivo);
			printf("%s\n", mensaje_error);
			registrar_error(mensaje_error, REGISTRO_ERRORES);
			return ERROR;
		}
	}
	return EXITO;
}

/*
Se comprueba que el formato de dato
en el CSV de los precios es correcto
Que los datos de tiempo no tengan
decimales.
*/


static int comprobar_formato_precios(const datos_csv_t* datos_precio_compra, const datos_csv_t* datos_precio_venta) {


	for (int numero_columna = PRIMERA_COLUMNA; numero_columna < NUMERO_COLUMNAS_TIEMPO_PRECIO; numero_columna++) {

		if (comprobar_formato_dato_tiempo_precio(datos_precio_compra, numero_columna) == ERROR) {

			printf("Error en el CSV de los precios de compra\n");
			registrar_error("Error en el CSV de los precios de compra\n", REGISTRO_ERRORES);
			return ERROR;
		}
   
		if (comprobar_formato_dato_tiempo_precio(datos_precio_venta, numero_columna) == ERROR) {

			printf("Error en el CSV de los precios de venta\n");
			registrar_error("Error en el CSV de los precios de venta\n", REGISTRO_ERRORES);
			return ERROR;
		}
	}

	/*
	Se pasa a comprobar que:
	Los precios no son negativos:
	Son valores numericos.
	*/

	if (comprobar_precio(datos_precio_compra, "Precio de compra del kilovatio") == ERROR) {
		return ERROR;
	}
	if (comprobar_precio(datos_precio_venta, "Precio de venta del kilovatio") == ERROR) {
		return ERROR;
	}
	return EXITO;
}

/*
Este subprograma se utiliza para
verificar que el numero de filas
y columnas en el csv de los precios
son correctos.
*/

static int comprobar_dimensiones_csv_precios(datos_csv_precio_t* datos_precio_compra, datos_csv_precio_t* datos_precio_venta) {

	//Cargo los punteros que apuntan a las posiciones de memoria donde se encuentra la informacion
	//de los precios

	datos_csv_t* informacion_precio_compra = &(datos_precio_compra->informacion_precio);
	datos_csv_t* informacion_precio_venta = &(datos_precio_venta->informacion_precio);

	int numero_filas_precio_compra = informacion_precio_compra->filas;
	int numero_columnas_precio_compra = informacion_precio_compra->columnas;
	int numero_filas_precio_venta = informacion_precio_venta->filas;
	int numero_columnas_precio_venta = informacion_precio_venta->columnas;

	if (comprobar_dimensiones_csv_variable(informacion_precio_compra,
		NUMERO_MINIMO_FILAS_PRECIO, NUMERO_COLUMNAS_PRECIO, "Compra") == ERROR) {
		return ERROR;
	}

	if (comprobar_dimensiones_csv_variable(informacion_precio_venta,
		NUMERO_MINIMO_FILAS_PRECIO, NUMERO_COLUMNAS_PRECIO, "Venta") == ERROR) {
		return ERROR;
	}

	return EXITO;
}
/*
Este subprograma se utiliza para
comprobar que la fecha inicial de
precio es correcto, es decir que
la hora inicial del algoritmo esta
cubierta.
*/

static int comprobar_fecha_inicial_precio(datos_csv_precio_t* datos_precios, datos_csv_algoritmo_t* datos_algoritmo) {


	struct tm Fecha_Inicial_Precio = { 0 };
	struct tm Fecha_Inicial_Algoritmo = { 0 };

	//Cargo las ubicaciones en donde se encuentran los datos del precio->
	int columna_anyo_precio = datos_precios->posiciones_informacion_precio.ubicacion_fecha_precios.columna_anyo;
	int columna_mes_precio = datos_precios->posiciones_informacion_precio.ubicacion_fecha_precios.columna_mes;
	int columna_dia_precio = datos_precios->posiciones_informacion_precio.ubicacion_fecha_precios.columna_dia;
	int columna_hora_precio = datos_precios->posiciones_informacion_precio.ubicacion_fecha_precios.columna_hora;

	//Cargo las ubicaciones donde se encuentran los datos del algoritmo->
	int columna_anyo_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_anyo;
	int columna_mes_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_mes;
	int columna_dia_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_dia;
	int columna_hora_inicio_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_hora;

	//Cargo los punteros que apuntan a las posiciones de memoria donde se encuentra la informacion
	datos_csv_t* informacion_precios = &(datos_precios->informacion_precio);
	datos_csv_t* informacion_algoritmo = &(datos_algoritmo->informacion_algoritmo);

	cargar_fecha(informacion_precios, &Fecha_Inicial_Precio, columna_anyo_precio,
		columna_mes_precio, columna_dia_precio, columna_hora_precio,
		NO_PROCEDE, PRIMERA_FILA, NO_INCLUIR_MINUTO);

	cargar_fecha(informacion_algoritmo, &Fecha_Inicial_Algoritmo, columna_anyo_inicio_algoritmo,
		columna_mes_inicio_algoritmo, columna_dia_inicio_algoritmo,
		columna_hora_inicio_algoritmo, NO_PROCEDE, PRIMERA_FILA, NO_INCLUIR_MINUTO);

	if (verificar_orden_fechas(Fecha_Inicial_Precio, Fecha_Inicial_Algoritmo, INCLUIR_FECHA_IGUAL) == ERROR) {

		printf("El primer dato de precios es posterior en el tiempo al inicio del algoritmo\n");
		registrar_error("El primer dato de precios es posterior en el tiempo al inicio del algoritmo", REGISTRO_ERRORES);

		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "La fecha inicial del algoritmo es Año %d Mes %d Dia %d Hora %d a numero correctamente\n",
			Fecha_Inicial_Algoritmo.tm_year + DESPLAZAMIENTO_ANYO,
			Fecha_Inicial_Algoritmo.tm_mon + DESPLAZAMIENTO_MES, Fecha_Inicial_Algoritmo.tm_mday,
			Fecha_Inicial_Algoritmo.tm_hour);

		printf("%s\n", mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);

		char mensaje_error_2[512];
		snprintf(mensaje_error_2, sizeof(mensaje_error_2), "La fecha inicial del CSV de los precios es Año %d Mes %d Dia %d Hora %d\n",
			Fecha_Inicial_Precio.tm_year + DESPLAZAMIENTO_ANYO,
			Fecha_Inicial_Precio.tm_mon + DESPLAZAMIENTO_MES, Fecha_Inicial_Precio.tm_mday,
			Fecha_Inicial_Precio.tm_hour);

		printf("%s\n", mensaje_error_2);
		registrar_error(mensaje_error_2, REGISTRO_ERRORES);


		return ERROR;
	}
	return EXITO;
}
/*
Este subprograma se utiliza
para comprobar que la fecha
final del algoritmo esta cubierta
en el CSV de los precios
*/

static int comprobar_fecha_final_precio(datos_csv_precio_t* datos_precios, datos_csv_algoritmo_t* datos_algoritmo) {

	struct tm fecha_final_algoritmo = { 0 };
	struct tm fecha_final_precio = { 0 };


	//Cargo el puntero que apunta a la posicion de memoria donde se encuentra la informacion de los precios
	datos_csv_t* informacion_precios = &(datos_precios->informacion_precio);
	datos_csv_t* informacion_algoritmo = &(datos_algoritmo->informacion_algoritmo);




	//Cargar la fila final del CSV de precios
	int fila_final = informacion_precios->filas - 1;

	//Se carga las ubicaciones donde se encuentra la informacion de los precios>
	int columna_anyo_precio = datos_precios->posiciones_informacion_precio.ubicacion_fecha_precios.columna_anyo;
	int columna_mes_precio = datos_precios->posiciones_informacion_precio.ubicacion_fecha_precios.columna_mes;
	int columna_dia_precio = datos_precios->posiciones_informacion_precio.ubicacion_fecha_precios.columna_dia;
	int columna_hora_precio = datos_precios->posiciones_informacion_precio.ubicacion_fecha_precios.columna_hora;

	//Se carga las ubicaciones donde se encuentra la informacion del algoritmo->
	int columna_anyo_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_anyo;
	int columna_mes_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_mes;
	int columna_dia_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_dia;
	int columna_hora_final_algoritmo = datos_algoritmo->posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_hora;





	cargar_fecha(informacion_precios, &(fecha_final_precio), columna_anyo_precio,
		columna_mes_precio, columna_dia_precio, columna_hora_precio,
		NO_PROCEDE, fila_final, NO_INCLUIR_MINUTO);


	cargar_fecha(informacion_algoritmo, &(fecha_final_algoritmo), columna_anyo_final_algoritmo,
		columna_mes_final_algoritmo, columna_dia_final_algoritmo,
		columna_hora_final_algoritmo, NO_PROCEDE, PRIMERA_FILA, NO_INCLUIR_MINUTO);

	if (verificar_orden_fechas(fecha_final_algoritmo, fecha_final_precio, INCLUIR_FECHA_IGUAL) == ERROR) {

		printf("La fecha final del algoritmo es posterior a la fecha final de los precios disponibles\n");
		char mensaje_error[512];
		snprintf(mensaje_error, sizeof(mensaje_error), "La fecha final del algoritmo es Año %d Mes%d Dia %d Hora %d a numero correctamente",
			fecha_final_algoritmo.tm_year + DESPLAZAMIENTO_ANYO,
			fecha_final_algoritmo.tm_mon + DESPLAZAMIENTO_MES,
			fecha_final_algoritmo.tm_mday, fecha_final_algoritmo.tm_hour);
		printf(mensaje_error);
		registrar_error(mensaje_error, REGISTRO_ERRORES);

		char mensaje_error_2[512];
		snprintf(mensaje_error_2, sizeof(mensaje_error_2), "La fecha final del CSV de los precios es Año %d Mes %d Dia %d Hora %d\n", fecha_final_precio.tm_year + DESPLAZAMIENTO_ANYO,
			fecha_final_precio.tm_mon + DESPLAZAMIENTO_MES,
			fecha_final_precio.tm_mday, fecha_final_precio.tm_hour);
		printf(mensaje_error_2);
		registrar_error(mensaje_error_2, REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}
/*
* En este subprograma se imprime que error ha habido con el precio de entrada
* y se guarda en el registro.log de errores.
*/
static void imprimir_mensaje_error_precio(const struct tm fecha_1_temp, const struct tm fecha_2_temp) {
	printf("Los precios no son consecutivos \n");
	printf("La fecha del primer precio es Año %d Mes %d Dia %d Hora %d \n",
		fecha_1_temp.tm_year + DESPLAZAMIENTO_ANYO,
		fecha_1_temp.tm_mon + DESPLAZAMIENTO_MES,
		fecha_1_temp.tm_mday,
		fecha_1_temp.tm_hour);
	printf("La fecha del segundo precio es Año %d Mes %d Dia %d Hora %d \n",
		fecha_2_temp.tm_year + DESPLAZAMIENTO_ANYO,
		fecha_2_temp.tm_mon + DESPLAZAMIENTO_MES,
		fecha_2_temp.tm_mday,
		fecha_2_temp.tm_hour);

	char mensaje_error[512];
	snprintf(mensaje_error, sizeof(mensaje_error), "Error los precios no son consecutivos la fecha del primer precio es año  %d mes %d dia %d hora %d la fecha del segundo precio es año %d mes %d dia %d hora %d\n",
		fecha_1_temp.tm_year + DESPLAZAMIENTO_ANYO, fecha_1_temp.tm_mon + DESPLAZAMIENTO_MES,
		fecha_1_temp.tm_mday, fecha_1_temp.tm_hour,
		fecha_2_temp.tm_year + DESPLAZAMIENTO_ANYO, fecha_2_temp.tm_mon + DESPLAZAMIENTO_MES,
		fecha_2_temp.tm_mday, fecha_2_temp.tm_hour);

	registrar_error(mensaje_error, REGISTRO_ERRORES);

}

/*
Este subprograma se utiliza para comprobar
que las dos fechas de entrada solo se llevan
una hora de diferencia.
*/

static int comprobar_consecutividad_horas(const struct tm fecha_1, const struct tm fecha_2) {

	struct tm fecha_1_temp = fecha_1;
	struct tm fecha_2_temp = fecha_2;

	time_t fecha_1_normalizada = mktime(&fecha_1_temp);
	time_t fecha_2_normalizada = mktime(&fecha_2_temp);

	double diferencia_tiempo = difftime(fecha_2_normalizada, fecha_1_normalizada);

	/*
	Se revisa que en la fecha
	que se esta comprobando
	no coincida con un dia
	que hay un cambio de hora->
	*/

	

	if (fecha_1_temp.tm_isdst != fecha_2_temp.tm_isdst) {

		/*
		Si hay una trancision
		de cambio de hora
		puede haber un tramo horario
		que no exista.
		*/
		if (abs(diferencia_tiempo) <= SEGUNDOS_DOS_HORAS) {
			return EXITO;
		}
		else {
			imprimir_mensaje_error_precio(fecha_1_temp, fecha_2_temp);
			return ERROR;
		}
	}

	else if (diferencia_tiempo == SEGUNDOS_HORA) {
		//La diferencia de tiempo coincide con una hora
		return EXITO;
	}

	else {
		imprimir_mensaje_error_precio(fecha_1_temp, fecha_2_temp);
		return ERROR;
	}

	return EXITO;
}

/*
En este subprograma se comprueba
que los datos del CSV de los precios
sean en horas consecutivas->
*/

static int comprobar_consecutividad_precios(datos_csv_precio_t* datos_precio) {

	/*
	Creo el puntero que apunta a la posicion de memoria donde se encuentra
	la informacion del precio
	*/

	datos_csv_t* informacion_precio = &(datos_precio->informacion_precio);

	//Cargo las filas y columnas donde se encuentra la informacion
	int columna_anyo_precio = datos_precio->posiciones_informacion_precio.ubicacion_fecha_precios.columna_anyo;
	int columna_mes_precio = datos_precio->posiciones_informacion_precio.ubicacion_fecha_precios.columna_mes;
	int columna_dia_precio = datos_precio->posiciones_informacion_precio.ubicacion_fecha_precios.columna_dia;
	int columna_hora_precio = datos_precio->posiciones_informacion_precio.ubicacion_fecha_precios.columna_hora;


	//Cargo el numero de filas->
	int numero_filas = datos_precio->informacion_precio.filas;

	/*
	Si solo se tiene una fila con datos
	no es necesario comprobar la consecutividad
	ya que solo hay una hora.
	*/

	struct tm fecha_1 = { 0 };
	struct tm fecha_2 = { 0 };

	//Se itera por todas las filas del CSV

	for (int numero_fila = 1; numero_fila < numero_filas - 1; numero_fila++) {

		cargar_fecha(informacion_precio, &fecha_1, columna_anyo_precio,
			columna_mes_precio, columna_dia_precio,
			columna_hora_precio, NO_PROCEDE, numero_fila,
			NO_INCLUIR_MINUTO);

		cargar_fecha(datos_precio, &fecha_2, columna_anyo_precio,
			columna_mes_precio, columna_dia_precio,
			columna_hora_precio, NO_PROCEDE, numero_fila + 1,
			NO_INCLUIR_MINUTO);

		if (comprobar_consecutividad_horas(fecha_1, fecha_2) == ERROR) {
			printf("Las fechas no son consecutivas \n");
			return ERROR;
		}
	}
	return EXITO;
}

/*En este subprograma se comprueba que
 la fecha a las que estan asociadas los
 precios tienen sentido.*/

static int comprobar_fecha_precios(datos_csv_precio_t* datos_precio_compra, datos_csv_precio_t* datos_precio_venta,
	datos_csv_precio_t* datos_algoritmo) {

	/*
	Se comprueba que:
	1.Todas las horas de ejecucion del
	Algoritmo tienen un precio asociado.
	2.Los precios estan en orden cronologico.
	*/

	if (comprobar_fecha_inicial_precio(datos_precio_compra, datos_algoritmo) == ERROR) {
		printf("En el CSV de compra el algoritmo empieza antes que el primer precio\n");
		return ERROR;
	}

	if (comprobar_fecha_inicial_precio(datos_precio_venta, datos_algoritmo) == ERROR) {
		printf("En el CSV de venta el algoritmo empieza antes que el primer precio\n");
		registrar_error("En el CSV de venta el algoritmo empieza antes que el primer precio\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (comprobar_fecha_final_precio(datos_precio_compra, datos_algoritmo) == ERROR) {
		printf("En el CSV de compra el algoritmo termina más tarde que el ultimo precio\n");
		registrar_error("En el CSV de compra el algoritmo termina más tarde que el ultimo precio", REGISTRO_ERRORES);
		return ERROR;
	}

	if (comprobar_fecha_final_precio(datos_precio_venta, datos_algoritmo) == ERROR) {
		printf("En el CSV de venta el algoritmo termina más tarde que el ultimo precio\n");
		registrar_error("En el CSV de venta el algoritmo termina más tarde que el ultimo precio\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (comprobar_consecutividad_precios(datos_precio_compra) == ERROR) {
		printf("Los precios no son consecutivos en el CSV de los precios de compra\n");
		registrar_error("Los precios no son consecutivos en el CSV de los precios de compra\n", REGISTRO_ERRORES);
		return ERROR;
	}

	if (comprobar_consecutividad_precios(datos_precio_venta) == ERROR) {
		printf("Los precios no son consecutivos en el CSV de los precios de venta\n");
		registrar_error("Los precios no son consecutivos en el CSV de los precios de venta\n", REGISTRO_ERRORES);
		return ERROR;
	}
	return EXITO;
}
/*
Este subprograma se utiliza para comprobar
que los encabezados del CSV de los precios
son los correctos-> Primero se definen los
valores de estos encabezados y despues se
llama al subprograma que comprueba que los
encabezados del CSV coinciden.
*/

int comprobar_encabezados_precios(datos_csv_precio_t* datos_precio_compra, datos_csv_precio_t* datos_precio_venta) {

  

	const char* valores_aceptables[] = {"Año","Mes","Dia","Hora","Precio kWh" };
  printf("%s", valores_aceptables[0]);
	const int numero_encabezados = sizeof(valores_aceptables) / sizeof(valores_aceptables[0]);
	datos_csv_t* informacion_precio_compra = &(datos_precio_compra->informacion_precio);
  datos_csv_t* informacion_precio_venta  = &(datos_precio_venta->informacion_precio);


	if (verificar_encabezado_csv(informacion_precio_compra, valores_aceptables,
		numero_encabezados, "Datos de los Precios de compra") == ERROR) {
		printf("Los encabezados del CSV de los precios de compra son incorrectos\n");
    registrar_error("Los encabezados del csv de los precios de compra son incorrectos\n",
    REGISTRO_ERRORES);
		return ERROR;
	}

	if (verificar_encabezado_csv(informacion_precio_venta, valores_aceptables,
		numero_encabezados, "Datos de los Precios de Venta") == ERROR) {
		printf("Los encabezados del CSV de los precios de venta son incorrectos\n");
    registrar_error("Los encabezados del csv de los precios de venta son incorrectos\n",
    REGISTRO_ERRORES);
		return ERROR;
	}


	return EXITO;

}
/*
Este subprograma se utiliza para
verificar que los datos de los
precios en el csv que contiene
los datos de los precios son
correctos->

Para ello se comprueba que:
1.El numero de filas y
columnas es correcto.
2.Los datos del Csv tienen
sentido. (Son datos numericos)
y no hay precios negativos.
3.El orden cronologio de los
precios tiene sentido.
*/

int verificar_precios(datos_csv_precio_t* datos_precio_compra, datos_csv_precio_t* datos_precio_venta,
	datos_csv_algoritmo_t* datos_algoritmo) {



	if (comprobar_dimensiones_csv_precios(datos_precio_compra, datos_precio_venta) == ERROR) {
		return ERROR;
	}

	if (comprobar_encabezados_precios(datos_precio_compra, datos_precio_venta) == ERROR) {
		return ERROR;
	}

  
	if (comprobar_formato_precios(datos_precio_compra, datos_precio_venta) == ERROR) {
		return ERROR;
	}
  
	if (comprobar_fecha_precios(datos_precio_compra, datos_precio_venta, datos_algoritmo) == ERROR) {
		return ERROR;
	}
	return EXITO;

}