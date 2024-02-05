#include "definiciones_globales.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "osqp.h"
#include "operaciones_string.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

//Se utiliza este subprograma para obtener las fechas adicionales de las baterias que hay que añadir
//Por cada bateria hay que añadir la fecha inicial a partir la cual la bateria esta conectada, la fecha
//final en la que la bateria se desconecta, y si se considera algun objetivo de carga de la bateria la fecha
//a la que se desea un determinado estado de carga para la bateria

int cargar_fechas_adicionales_baterias(datos_csv_baterias_t* datos_baterias, struct tm** fechas_adicionales_baterias,
  const int numero_vehiculos) {

  //Se carga un puntero que apunta a donde se encuentra la informacion leida de los csv de las baterias
  datos_csv_t* informacion_baterias = &(datos_baterias->informacion_baterias);
  //Se carga el tamaño actual del array de fechas
  int tamanyo_actual = numero_vehiculos * 2;
  //Se carga el numero de filas que contiene el csv de las baterias
  int numero_filas_baterias = datos_baterias->informacion_baterias.filas - 1;
  //Se carga la ubicacion donde se encuentra la informacion en el csv de las baterias
  //Se carga las ubicaciones donde se encuentra la fecha inicial de la bateria
  int columna_anyo_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_anyo;
  int columna_mes_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_mes;
  int columna_dia_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_dia;
  int columna_hora_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_hora;
  int columna_minuto_inicial_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_minuto;
  //Se carga las ubicaciones donde se encuentra la fecha final de la bateria
  int columna_anyo_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_anyo;
  int columna_mes_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_mes;
  int columna_dia_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_dia;
  int columna_hora_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_hora;
  int columna_minuto_final_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_minuto;
  //Se carga las ubicaciones donde se encuentra fecha objetivo de la bateria
  int columna_anyo_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_anyo;
  int columna_mes_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_mes;
  int columna_dia_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_dia;
  int columna_hora_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_hora;
  int columna_minuto_objetivo_bateria = datos_baterias->posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_minuto;

  for (int bateria = 0; bateria < numero_filas_baterias; bateria++) {

    if (strings_iguales(datos_baterias->informacion_baterias.datos, "si") == EXITO) {
      tamanyo_actual += 3;
      struct tm* tempPtr = (struct tm*)realloc(*fechas_adicionales_baterias, sizeof(struct tm) * tamanyo_actual);
      if (tempPtr == NULL) {
        printf("Fallo en la adición de las fechas adicionales en los puntos de simulación, fallo al reservar memoria\n");
        registrar_error("Fallo en la adición de las fechas adicionales en los puntos de simulación, fallo al reservar memoria\n", REGISTRO_ERRORES);
        return ERROR;
      }
        //Se pasa a situar en el array de fechas iniciales la fecha inicial de la bateria
        *fechas_adicionales_baterias = tempPtr;

        
        fechas_adicionales_baterias[tamanyo_actual - 3]->tm_year = informacion_baterias->datos[bateria + 1][columna_anyo_inicial_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 3]->tm_mon = informacion_baterias->datos[bateria + 1][columna_mes_inicial_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 3]->tm_mday = informacion_baterias->datos[bateria + 1][columna_dia_inicial_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 3]->tm_hour = informacion_baterias->datos[bateria + 1][columna_hora_inicial_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 3]->tm_min = informacion_baterias->datos[bateria + 1][columna_minuto_inicial_bateria];

        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_year = informacion_baterias->datos[bateria + 1][columna_anyo_objetivo_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_mon = informacion_baterias->datos[bateria + 1][columna_mes_objetivo_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_mday = informacion_baterias->datos[bateria + 1][columna_dia_objetivo_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_hour = informacion_baterias->datos[bateria + 1][columna_hora_objetivo_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_min = informacion_baterias->datos[bateria + 1][columna_minuto_objetivo_bateria];

        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_year = informacion_baterias->datos[bateria + 1][columna_anyo_final_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_mon = informacion_baterias->datos[bateria + 1][columna_mes_final_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_mday = informacion_baterias->datos[bateria + 1][columna_dia_final_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_hour = informacion_baterias->datos[bateria + 1][columna_hora_final_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_min = informacion_baterias->datos[bateria + 1][columna_minuto_final_bateria];

      }
      else {
        tamanyo_actual += 2;
        struct tm* tempPtr = (struct tm*)realloc(*fechas_adicionales_baterias, sizeof(struct tm) * tamanyo_actual);
        if (tempPtr == NULL) {
          printf("Fallo en la adición de las fechas adicionales en los puntos de simulación, fallo al reservar memoria\n");
          registrar_error("Fallo en la adición de las fechas adicionales en los puntos de simulación, fallo al reservar memoria\n", REGISTRO_ERRORES);
          return ERROR;
        }
        *fechas_adicionales_baterias = tempPtr;
        //Se cargan la fecha inicial y final de la bateria en el array de fechas adicionales a considerar en la simulacion 
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_year = informacion_baterias->datos[bateria + 1][columna_anyo_inicial_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_mon = informacion_baterias->datos[bateria + 1][columna_mes_inicial_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_mday = informacion_baterias->datos[bateria + 1][columna_dia_inicial_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_hour = informacion_baterias->datos[bateria + 1][columna_hora_inicial_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 2]->tm_min = informacion_baterias->datos[bateria + 1][columna_minuto_inicial_bateria];

        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_year = informacion_baterias->datos[bateria + 1][columna_anyo_final_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_mon = informacion_baterias->datos[bateria + 1][columna_mes_final_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_mday = informacion_baterias->datos[bateria + 1][columna_dia_final_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_hour = informacion_baterias->datos[bateria + 1][columna_hora_final_bateria];
        fechas_adicionales_baterias[tamanyo_actual - 1]->tm_min = informacion_baterias->datos[bateria + 1][columna_minuto_final_bateria];


      }
    }
  return EXITO;
  }
 


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

int leer_fechas_adicionales(datos_csv_vehiculos_t* datos_vehiculos, struct tm** fechas_adicionales,
                             datos_csv_baterias_t* datos_baterias) {

  //Cargo las ubicaciones de donde se encuentra la informacion relevante en el csv de los vehiculos.
  
  
  //Ubicacion de la fecha inicial de carga del vehiculo
  int anyo_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_anyo;
  int mes_inicial_vehiculo  = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_mes;
  int dia_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_dia;
  int hora_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_hora;
  int minuto_inicial_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_minuto;
  //Ubicacion de la fecha final del vehiculo
  int anyo_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_anyo;
  int mes_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_mes;
  int dia_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_dia;
  int hora_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_hora;
  int minuto_final_vehiculo = datos_vehiculos->posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_minuto;

  //Tantas vehiculos como filas-1 tenga el csv de los vehículos y tantas baterias como filas-1 tenga el csv de las baterias.

  int numero_vehiculos = datos_vehiculos->informacion_vehiculos.filas - 1;
 
  *fechas_adicionales = (struct tm*)malloc(sizeof(struct tm) * numero_vehiculos* 2);
  //Se pasa a rellenar el array de fechas de tipo tm* con todas las fechas adicionales de los vehiculos que
  //hay que añadir

  if (*fechas_adicionales == NULL) {
    fprintf(stderr, "No se ha podido reservar memoria para la informacion procesada.\n");
    registrar_error("No se ha podido reservar memoria para la informacion procesada", REGISTRO_ERRORES);
    return ERROR;
  }

  for (int i = 0; i < 2*numero_vehiculos; i++) {
    //Se carga la fecha inicial del vehiculo
    (*fechas_adicionales)[2 * i].tm_year = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][anyo_inicial_vehiculo])-1900;
    (*fechas_adicionales)[2 * i].tm_mon  = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][mes_inicial_vehiculo])-1;
    (*fechas_adicionales)[2 * i].tm_mday = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][dia_inicial_vehiculo]);
    (*fechas_adicionales)[2 * i].tm_hour = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][hora_inicial_vehiculo]);
    (*fechas_adicionales)[2 * i].tm_min  = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][minuto_inicial_vehiculo]);
    //Se carga la fecha final del vehiculo
    (*fechas_adicionales)[2 * i + 1].tm_year = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][anyo_final_vehiculo]) - 1900;
    (*fechas_adicionales)[2 * i + 1].tm_mon  = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][mes_final_vehiculo]) - 1;
    (*fechas_adicionales)[2 * i + 1].tm_mday = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][dia_final_vehiculo]);
    (*fechas_adicionales)[2 * i + 1].tm_hour = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][hora_final_vehiculo]);
    (*fechas_adicionales)[2 * i + 1].tm_min = atoi(datos_vehiculos->informacion_vehiculos.datos[i + 1][minuto_final_vehiculo]);
  }



}

void procesar_informacion_puntos_simulacion(informacion_entrada_t* informacion_entrada,informacion_procesada_t* informacion_procesada) {
  //Se procesa la informacion de cuantos puntos de simulacion hacen falta.

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