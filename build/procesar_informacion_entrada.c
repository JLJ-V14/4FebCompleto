#include "calcular_puntos_simulacion.h"
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


/*Este subprograma se encarga de guardar la informacion de los diferentes del vehiculos del sistema en la
  variable correspondiente*/
void procesar_informacion_vehiculos(informacion_entrada_t* informacion_entrada, informacion_procesada_t* informacion_procesada,
                                    puntos_adicionales_t* puntos_adicionales) {

}

void procesar_informacion_baterias() {

}

void procesar_informacion_precio() {

}

int configurar_puntos_simulacion(informacion_entrada_t* informacion_entrada, informacion_procesada_t* informacion_procesada,
                                 puntos_adicionales_t**  puntos_adicionales) {
  //Se cargan elementos temporales claves de la simulacion, como la resolucion de la simulacion, la fecha inicial
  //y la fecha final.
  //
   struct tm** fechas_adicionales = NULL;
  //Cargo la ubicacion de las fechas iniciales y finales del algoritmo
  //Se definen variables para almacenar las fechas iniciales y finales del cálculo de optimizacion
  struct tm* fecha_inicial_algoritmo;
  struct tm* fecha_final_algoritmo;
  /*Se crea un array para almacenar las fechas adicionales que deben ser añadidas (ida y partida de vehiculos por
    ejemplo*/
  
  int numero_fechas_adicionales = 0;

  fecha_inicial_algoritmo = malloc(sizeof(struct tm));
  fecha_final_algoritmo = malloc(sizeof(struct tm));
  if ((fecha_inicial_algoritmo == NULL) || (fecha_final_algoritmo == NULL)) {
    free(fecha_inicial_algoritmo);
    free(fecha_final_algoritmo);
    return ERROR;
  }

  //Se carga la fecha inicial del algoritmo.
  int fila_valores = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.fila_informacion;
  int columna_anyo_inicial = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_anyo;
  int columna_mes_inicial = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_mes;
  int columna_dia_inicial = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_dia;
  int columna_hora_inicial = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_hora;
  int columna_minuto_inicial = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_inicial_algoritmo.columna_minuto;
  //Se carga la fecha final del algoritmo
  int columna_anyo_final = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_anyo;
  int columna_mes_final = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_mes;
  int columna_dia_final = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_dia;
  int columna_hora_final = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_hora;
  int columna_minuto_final = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.ubicacion_fecha_final_algoritmo.columna_minuto;
  //Se carga la ubicacion de la resolucion temporal de la simulacion
  int columna_resolucion_minutos = informacion_entrada->datos_algoritmo.posiciones_informacion_algoritmo.resolucion_minutos;

  //Se carga la resolution temporal de la simulacion
  int delta_resolucion = informacion_entrada->datos_algoritmo.informacion_algoritmo.datos[fila_valores][columna_resolucion_minutos];

  //Se cargan las fechas iniciales y finales del algoritmo
  cargar_fecha(&(informacion_entrada->datos_algoritmo), fecha_inicial_algoritmo, columna_anyo_inicial,
    columna_mes_inicial, columna_dia_inicial, columna_hora_inicial, columna_minuto_inicial,
    fila_valores, SI_INCLUIR_MINUTO);

  cargar_fecha(&(informacion_entrada->datos_algoritmo), fecha_final_algoritmo, columna_anyo_final,
    columna_mes_final, columna_dia_final, columna_hora_final, columna_minuto_final,
    fila_valores, SI_INCLUIR_MINUTO);

  if (leer_fechas_adicionales(&(informacion_entrada->datos_vehiculos), fechas_adicionales, &(informacion_entrada->datos_baterias),
    &numero_fechas_adicionales, fecha_inicial_algoritmo, fecha_final_algoritmo) == ERROR) {
    printf("No se ha podido añadir las fechas adicionales a la simulacion\n");
    registrar_error("No se ha podido añadir las fechas adicionales a la simulacion", REGISTRO_ERRORES);
    return ERROR;
  }

  if (cacular_puntos_simulacion(informacion_entrada, fechas_adicionales, informacion_procesada,
    fecha_inicial_algoritmo, fecha_final_algoritmo, delta_resolucion,
    numero_fechas_adicionales,puntos_adicionales) == ERROR) {
    printf("No se ha podido calcular el numero de puntos de simulacion\n");
    registrar_error("No se ha podido calcular el numero de puntos de simulacion\n", REGISTRO_ERRORES);
  }

  return EXITO;
}



//Este subprograma se utiliza para procesar la informacion de entrada para guardarlo
//facilmente en las matrices->

void procesar_informacion_entrada(informacion_entrada_t* informacion_entrada,
                                  informacion_procesada_t* informacion_procesada) {

  //Se crea una variable para almacenar las fechas adicionales a añadir, esta variable va a ser utilizada para
  //reconocer más rápidamente a que punto de simulación corresponde la ida o partida de los vehículos o baterías
  puntos_adicionales_t** puntos_adicionales = NULL;
 
  //Se almacena la informacion de restriccion leída del csv de las restricciones
  procesar_informacion_restricciones(&(informacion_entrada->datos_restricciones), &(informacion_procesada->informacion_restricciones_sistema));
  configurar_puntos_simulacion(informacion_entrada, informacion_procesada,puntos_adicionales);

}