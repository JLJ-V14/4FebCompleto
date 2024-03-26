#include "definiciones_globales.h"
#include "registrar_errores.h"
#include "verificar_baterias.h"
#include "verificar_conexiones_simultaneas.h"
#include "verificar_datos_algoritmo.h"
#include "verificar_datos_terminales.h"
#include "verificar_precios.h"
#include "verificar_restricciones.h"
#include "verificar_vehiculos.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <stdio.h>









/*En este archivo esta el subprograma que se encarga
* de la verificacion de las entradas.
*
*/
int verificar_entradas(informacion_entrada_t* informacion_sistema) {

	//Se extrae de la variable de entrada
	//los punteros a las posiciones de
	// memoria donde se encuentra la 
	// informacion de entrada.


	datos_csv_algoritmo_t* datos_algoritmo = &(informacion_sistema->datos_algoritmo);
	datos_csv_terminales_t* datos_terminales = &(informacion_sistema->datos_terminales);
	datos_csv_precio_t* datos_precio_compra = &(informacion_sistema->datos_precio_compra);
	datos_csv_precio_t* datos_precio_venta = &(informacion_sistema->datos_precio_venta);
	datos_csv_restricciones_t* datos_restricciones = &(informacion_sistema->datos_restricciones);
	datos_csv_vehiculos_t* datos_vehiculos = &(informacion_sistema->datos_vehiculos);
	datos_csv_baterias_t* datos_baterias = &(informacion_sistema->datos_baterias);




	if (verificar_datos_algoritmo(datos_algoritmo)) {
		printf("Los Datos del algoritmo son incorrectos\n");
		registrar_error("Los datos del algoritmo son incorrectos\n", REGISTRO_ERRORES);
		return ERROR;
	}

  
	if (verificar_datos_terminales(datos_terminales) == ERROR) {
		printf("Los Datos del CSV de los terminales son incorrectos\n");
		registrar_error("Los datos de los terminales son incorrectos\n", REGISTRO_ERRORES);
		return ERROR;
	}
  
	if (verificar_precios(datos_precio_compra, datos_precio_venta, datos_algoritmo) == ERROR) {
		printf("Los Datos del CSV de los precios son incorrectos\n");
		registrar_error("Los datos de los precios son incorrectos\n", REGISTRO_ERRORES);
		return ERROR;
	}
  
	if (verificar_datos_restricciones(datos_restricciones) == ERROR) {
		printf("Los datos del csv de las restricciones son incorrectos\n");
		registrar_error("Los datos de las restricciones son incorrectos\n", REGISTRO_ERRORES);
		return ERROR;
	}
  

	if (verificar_datos_vehiculos(datos_vehiculos, datos_algoritmo,datos_terminales) == ERROR) {
		printf("los datos del csv de los vehiculos son incorrectos\n");
		registrar_error("La informacion de los vehiculos es incorrecta\n", REGISTRO_ERRORES);
		return ERROR;
	}
 
  
	if (verificar_baterias(datos_baterias, datos_algoritmo,datos_terminales) == ERROR) {
		printf("los datos del csv de las baterias son incorrectas\n");
		registrar_error("La informacion de las baterias es incorrecta\n", REGISTRO_ERRORES);
		return ERROR;
	}
 
	if(verificar_simultaneidad_vehiculos_baterias(datos_vehiculos, datos_baterias) == ERROR){
		printf("Hay problemas de simultaneidad de elementos conectados al mismo terminal al mismo tiempo\n");
		registrar_error("Hay problemas de simultaneidad de elementos conectados al mismo terminal al mismo tiempo\n",REGISTRO_ERRORES);
		return ERROR;
	}

 
  /**/
	return EXITO;
}
