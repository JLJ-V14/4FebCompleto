#include "definiciones_globales.h"
#include "registrar_errores.h"

#include "osqp.h"
#include "operaciones_fecha.h"
#include "operaciones_string.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>




//Se utiliza este subprograma para obtener las fechas adicionales de las baterias que hay que añadir
//Por cada bateria hay que añadir la fecha inicial a partir la cual la bateria esta conectada, la fecha
//final en la que la bateria se desconecta, y si se considera algun objetivo de carga de la bateria la fecha
//a la que se desea un determinado estado de carga para la bateria




//Este subprograma se utiliza para guardar las restricciones del sistema en la variable correspondiente.
void procesar_informacion_restricciones(datos_csv_restricciones_t* datos_csv_restricciones,
                                        informacion_restricciones_sistema_t* informacion_restricciones) {
//Cargo las posiciones donde se encuentra la informacion en el csv de entrada
  int fila_valores = datos_csv_restricciones->posiciones_informacion_restricciones.fila_valores;
  int columna_max_potencia_sistema = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_sistema;
  int columna_max_potencia_terminal_1 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_1;
  int columna_max_potencia_terminal_2 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_2;
  int columna_max_potencia_terminal_3 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_3;
  int columna_max_potencia_terminal_4 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_4;
  int columna_max_potencia_terminal_5 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_5;
  int columna_max_potencia_terminal_6 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_6;
  int columna_max_potencia_terminal_7 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_7;
  int columna_max_potencia_terminal_8 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_8;
  int columna_max_potencia_terminal_9 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_9;
  int columna_max_potencia_terminal_10 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_10;
  int columna_max_potencia_terminal_11 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_11;
  int columna_max_potencia_terminal_12 = datos_csv_restricciones->posiciones_informacion_restricciones.columna_max_potencia_terminal_12;
  //Cargo el puntero al csv de las restricciones
  datos_csv_t* datos_restricciones = &(datos_csv_restricciones->informacion_restricciones);
  informacion_restricciones->potencia_maxima_red = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_sistema] - '0');
  informacion_restricciones->potencia_maxima_red_R = informacion_restricciones->potencia_maxima_red / 3;
  informacion_restricciones->potencia_maxima_red_S = informacion_restricciones->potencia_maxima_red / 3;
  informacion_restricciones->potencia_maxima_red_T = informacion_restricciones->potencia_maxima_red / 3;
  informacion_restricciones->potencia_minima_red = -informacion_restricciones->potencia_maxima_red;
  informacion_restricciones->potencia_minima_red_R = informacion_restricciones->potencia_minima_red / 3;
  informacion_restricciones->potencia_minima_red_S = informacion_restricciones->potencia_minima_red / 3;
  informacion_restricciones->potencia_minima_red_T = informacion_restricciones->potencia_minima_red / 3;
  //Se carga los valores de potencia maxima que pueden intercambiar las estaciones de carga
  informacion_restricciones->potencia_maxima_estaciones_carga[0] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_1] );
  informacion_restricciones->potencia_maxima_estaciones_carga[1] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_2] );
  informacion_restricciones->potencia_maxima_estaciones_carga[2] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_3] );
  informacion_restricciones->potencia_maxima_estaciones_carga[3] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_4] );
  informacion_restricciones->potencia_maxima_estaciones_carga[4] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_5] );
  informacion_restricciones->potencia_maxima_estaciones_carga[5] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_6] );
  informacion_restricciones->potencia_maxima_estaciones_carga[6] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_7]);
  informacion_restricciones->potencia_maxima_estaciones_carga[7] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_8] );
  informacion_restricciones->potencia_maxima_estaciones_carga[8] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_9] );
  informacion_restricciones->potencia_maxima_estaciones_carga[9] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_10] );
  informacion_restricciones->potencia_maxima_estaciones_carga[10] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_11] );
  informacion_restricciones->potencia_maxima_estaciones_carga[11] = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_terminal_12] );
  // Se carga el valor mínimo de potencia que pueden intercambiar las estaciones de carga conectadas a los terminales
  for (int i = 0; i < NUMERO_TERMINALES; i++) {
    informacion_restricciones->potencia_minima_estaciones_carga[i] = 0;
  }
}

//Este subprograma se encarga de leer las fechas importantes que deben estar presentes en la simulacion
//como la llegada o partida de vehiculos.


int procesar_informacion_puntos_simulacion(informacion_entrada_t* informacion_entrada,informacion_procesada_t* informacion_procesada) {
  //Se procesa la informacion de cuantos puntos de simulacion hacen falta.
  datos_csv_vehiculos_t* datos_vehiculos = &(informacion_entrada->datos_vehiculos);
  datos_csv_baterias_t* datos_baterias   = &(informacion_entrada->datos_baterias);
  struct tm** fechas_adicionales = NULL;
  int numero_fechas_adicionales;
  //Se leen que fechas han de estar consideradas sí o sí en el calculo
  if (leer_fechas_adicionales(datos_vehiculos, fechas_adicionales, datos_baterias,&numero_fechas_adicionales) == ERROR) {
    return ERROR;
  }
  //Se eliminan las fechas adicionales repetidas.
  eliminar_fechas_repetidas(fechas_adicionales,&numero_fechas_adicionales);
  //Se configuran los puntos de simulacion 
  configurar_puntos_simulacion(informacion_entrada, informacion_procesada, fechas_adicionales, numero_fechas_adicionales);
  return EXITO;
}

void procesar_informacion_vehiculos() {

}

void procesar_informacion_baterias() {

}

void procesar_informacion_precio() {

}





//Este subprograma se utiliza para procesar la informacion de entrada para guardarlo
//facilmente en las matrices->

void procesar_informacion_entrada(informacion_entrada_t* informacion_entrada,
                                  informacion_procesada_t* informacion_procesada) {
  //Se almacena la informacion de restriccion leída del csv de las restricciones
  procesar_informacion_restricciones(&(informacion_entrada->datos_restricciones), &(informacion_procesada->informacion_restricciones_sistema));
 

}