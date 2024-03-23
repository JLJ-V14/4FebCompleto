#include "calcular_puntos_simulacion.h"
#include "definiciones_globales.h"
#include "registrar_errores.h"
#include "osqp.h"
#include "operaciones_fecha.h"
#include "operaciones_string.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

void printTm_2(const struct tm* date) {
  if (date == NULL) {
    printf("The date is NULL\n");
    return;
  }

  // Note: tm_year is years since 1900, tm_mon is 0-11, and tm_mday is 1-31
  printf("Year: %d, Month: %02d, Day: %02d\n",
    date->tm_year + 1900, date->tm_mon + 1, date->tm_mday);
  printf("Hour: %02d, Minute: %02d, Second: %02d\n",
    date->tm_hour, date->tm_min, date->tm_sec);

  // Optionally, you can print the day of the week and day of the year
  printf("Weekday (Sunday=0): %d, Day of the year: %d\n",
    date->tm_wday, date->tm_yday);
}

/*Este subprograma se utiliza para */
void procesar_informacion_terminales(informacion_entrada_t *informacion_entrada,
                                     informacion_procesada_t* informacion_procesada) {
  // primero cargo las ubicaciones donde se cuentra la informacion de a que fase está conectado a cada
  // terminal
  int columna_valores_fases = informacion_entrada->datos_terminales.posiciones_informacion_terminales.columna_fase;

  //Se almacena la fase a la que está conectada los terminales y se almacena en la variable correspondiente.
  for (int fila_actual = 0; fila_actual < NUMERO_TERMINALES; fila_actual++) {

    if (strings_iguales(informacion_entrada->datos_terminales.informacion_terminales.datos[fila_actual+1][columna_valores_fases], "Nada")) {
      informacion_procesada->informacion_terminales.fases_electricas[fila_actual] = '0';
    }
    else if (strings_iguales(informacion_entrada->datos_terminales.informacion_terminales.datos[fila_actual+1][columna_valores_fases],"Neutro")){
      informacion_procesada->informacion_terminales.fases_electricas[fila_actual] = 'N';
    }
    else {
      informacion_procesada->informacion_terminales.fases_electricas[fila_actual] = *(informacion_entrada->datos_terminales.informacion_terminales.datos[fila_actual + 1][columna_valores_fases]);
    }
    
  }
}




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
  informacion_restricciones->potencia_maxima_red = (OSQPFloat)atof(datos_restricciones->datos[fila_valores][columna_max_potencia_sistema]);
 
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
int procesar_informacion_vehiculos(informacion_entrada_t* informacion_entrada, informacion_procesada_t* informacion_procesada) {

  //Defino una variable para acceder a los puntos adicionales que se consideran adicionalmente en la simulación
  puntos_adicionales_t* puntos_adicionales = informacion_procesada->informacion_puntos_adicionales.puntos;

  informacion_procesada->informacion_vehiculos.numero_vehiculos = informacion_entrada->datos_vehiculos.informacion_vehiculos.filas -1 ;
 

  informacion_procesada->informacion_vehiculos.vehiculos = (vehiculos_t*)malloc(informacion_procesada->informacion_vehiculos.numero_vehiculos * sizeof(vehiculos_t));

  if (informacion_procesada->informacion_vehiculos.vehiculos == NULL) {
    printf("Error no se ha podido procesar la informacion de los vehiculos\n");
    registrar_error("No se ha podido procesar la informacion de los vehiculos\n", REGISTRO_ERRORES);
    return ERROR;
  }

  
  int vehiculos_totales = informacion_procesada->informacion_vehiculos.numero_vehiculos;
  // informacion_procesada->informacion_vehiculos.vehiculos = malloc(informacion_procesada->informacion_vehiculos.numero_vehiculos * sizeof(vehiculos_t));
  
  //Se procede a cargar las ubicaciones donde se encuentra la informacion en el csv de los vehiculos.
  int columna_capacidad = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.columna_capacidad_bateria;
  int columna_maxima_potencia = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.columna_maxima_potencia;
  int columna_modo_carga = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.columna_modo_carga;
  int columna_bat_deseada = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.columna_porcentaje_bateria_deseada;
  int columna_bat_inicial = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.columna_porcentaje_bateria_inicial;
  int columna_terminal = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.columna_terminales;
  //Se carga las ubicaciones donde se encuentra la informacion de las fechas iniciales y finales de los vehiculos
  int columna_anyo_inicial = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_anyo;
  int columna_mes_inicial = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_mes;
  int columna_dia_inicial = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_dia;
  int columna_hora_inicial = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_hora;
  int columna_minuto_inicial = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_inicial_vehiculo.columna_minuto;

  //Se carga las ubicaciones donde se encuentra la informacion de la fecha final
  int columna_anyo_final = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_anyo;
  int columna_mes_final = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_mes;
  int columna_dia_final = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_dia;
  int columna_hora_final = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_hora;
  int columna_minuto_final = informacion_entrada->datos_vehiculos.posiciones_informacion_vehiculos.ubicacion_fecha_final_vehiculo.columna_minuto;
  
  for (int numero_vehiculo = 0; numero_vehiculo < vehiculos_totales; numero_vehiculo++) {
    //Se cargan los datos del vehiculos.
    informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].bateria_final     = (OSQPFloat)atof(informacion_entrada->datos_vehiculos.informacion_vehiculos.datos[numero_vehiculo + 1][columna_bat_deseada]);
    informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].bateria_inicial   = (OSQPFloat)atof(informacion_entrada->datos_vehiculos.informacion_vehiculos.datos[numero_vehiculo + 1][columna_bat_inicial]);
    informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].capacidad_bateria = (OSQPFloat)atof(informacion_entrada->datos_vehiculos.informacion_vehiculos.datos[numero_vehiculo + 1][columna_capacidad]);
    informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].numero_terminal   = (OSQPInt)atof(informacion_entrada->datos_vehiculos.informacion_vehiculos.datos[numero_vehiculo + 1][columna_terminal]);
    informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].modo_carga        = informacion_entrada->datos_vehiculos.informacion_vehiculos.datos[numero_vehiculo + 1][columna_modo_carga];
    informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].potencia_maxima   = (OSQPFloat)atof(informacion_entrada->datos_vehiculos.informacion_vehiculos.datos[numero_vehiculo + 1][columna_maxima_potencia]);
    //Se carga la fechas inicial del vehiculo
   

    //Se carga la fecha inicial del vehiculo
    cargar_fecha(&(informacion_entrada->datos_vehiculos.informacion_vehiculos), &(informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].fecha_inicio),
      columna_anyo_inicial, columna_mes_inicial, columna_dia_inicial, columna_hora_inicial, columna_minuto_inicial,
      numero_vehiculo + 1, SI_INCLUIR_MINUTO);

    //Se carga la fecha final del vehiculo
    cargar_fecha(&(informacion_entrada->datos_vehiculos.informacion_vehiculos), &(informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].fecha_final),
      columna_anyo_final, columna_mes_final, columna_dia_final, columna_hora_final, columna_minuto_final,
      numero_vehiculo + 1, SI_INCLUIR_MINUTO);

  

    //Se obtiene de simulacion inicial del vehiculo
   
  
    obtener_punto_simulacion(&(informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].punto_inicio),
                             &(informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].fecha_inicio), puntos_adicionales);
    
    //Se obtiene el punto de simulacion final del vehiculo

    obtener_punto_simulacion(&(informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].punto_final),
                             &(informacion_procesada->informacion_vehiculos.vehiculos[numero_vehiculo].fecha_final), puntos_adicionales);
    /*  */
    
  }

  
  return EXITO;
}

/*Este subprograma se utiliza para proceder la informacion de las baterías*/
int procesar_informacion_baterias(informacion_entrada_t * informacion_entrada,informacion_procesada_t* informacion_procesada) {

  //Se carga el puntero que apunta a las posiciones de memoria de los puntos adicionales
  puntos_adicionales_t* puntos_adicionales = informacion_procesada->informacion_puntos_adicionales.puntos;
  informacion_procesada->informacion_baterias.numero_baterias = informacion_entrada->datos_baterias.informacion_baterias.filas - 1;
  informacion_procesada->informacion_baterias.baterias = (bateria_t*)malloc(informacion_procesada->informacion_baterias.numero_baterias * sizeof(bateria_t));

  if (informacion_procesada->informacion_baterias.baterias == NULL) {
    printf("Error no se ha podido procesar la informacion de las baterias\n");
    registrar_error("No se ha podido procesar la informacion de las baterias\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se carga el numero de baterias totales
  int baterias_totales = informacion_procesada->informacion_baterias.numero_baterias;
  //Se  procede a cargar las ubicaciones sobre donde se encuentra la informacion
  int columna_capacidad_bateria = informacion_entrada->datos_baterias.posiciones_informacion_baterias.columna_capacidad_bateria;
  int columna_consideracion_objetivo_carga = informacion_entrada->datos_baterias.posiciones_informacion_baterias.columna_consideracion_objetivo;
  int columna_bateria_objetivo = informacion_entrada->datos_baterias.posiciones_informacion_baterias.columna_porcentaje_bateria_deseada;
  int columna_bateria_inicial  = informacion_entrada->datos_baterias.posiciones_informacion_baterias.columna_porcentaje_bateria_inicial;
  int columna_terminal = informacion_entrada->datos_baterias.posiciones_informacion_baterias.columna_terminal;
  int columna_potencia_maxima = informacion_entrada->datos_baterias.posiciones_informacion_baterias.columna_potencia_maxima_bateria;
  //Se procede a cargar la ubicacion de las diferentes fechas importantes de las baterías
  // Fecha inicial de la bateria
  int columna_anyo_inicial = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_anyo;
  int columna_mes_inicial = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_mes;
  int columna_dia_inicial = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_dia;
  int columna_hora_inicial = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_hora;
  int columna_minuto_inicial = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_inicial_baterias.columna_minuto;
  //Se procede a cargar la ubicacion de la fecha final
  int columna_anyo_final = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_anyo;
  int columna_mes_final = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_mes;
  int columna_dia_final = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_dia;
  int columna_hora_final = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_hora;
  int columna_minuto_final = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_final_baterias.columna_minuto;
  //Se procede a cargar la ubicacion de la fecha del objetivo
  int columna_anyo_objetivo = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_anyo;
  int columna_mes_objetivo = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_mes;
  int columna_dia_objetivo = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_dia;
  int columna_hora_objetivo = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_hora;
  int columna_minuto_objetivo = informacion_entrada->datos_baterias.posiciones_informacion_baterias.ubicacion_fecha_objetivo_baterias.columna_minuto;

  /*Se procede a cargar la informacion de las baterias*/
  for (int numero_bateria = 0; numero_bateria < baterias_totales; numero_bateria++) {

    char* maxima_potencia = informacion_entrada->datos_baterias.informacion_baterias.datos[numero_bateria + 1][columna_potencia_maxima];

    //Se carga los datos de la bateria
    if (maxima_potencia != NULL && *maxima_potencia != '\0') { // Verificar que el dato no sea NULL y no esté vacío
      informacion_procesada->informacion_baterias.baterias[numero_bateria].maxima_potencia = (OSQPFloat)atof(maxima_potencia);
    }
    else {
      printf("Error: el dato de potencia máxima es NULL o está vacío.\n");
      // Manejar el error como sea apropiado
    }
    //Se procede a cargar la bateria inicial
    char* bateria_inicial = informacion_entrada->datos_baterias.informacion_baterias.datos[numero_bateria + 1][columna_bateria_inicial];
    if (bateria_inicial != NULL && *bateria_inicial != '\0') {
      informacion_procesada->informacion_baterias.baterias[numero_bateria].bateria_inicial = (OSQPFloat)atof(bateria_inicial);
    }
    else {
      printf("Error: el dato de bateria inicial es NULL o está vacío.\n");
      // Manejar el error como sea apropiado
    }
    char* bateria_objetivo = informacion_entrada->datos_baterias.informacion_baterias.datos[numero_bateria + 1][columna_bateria_objetivo];

    if (bateria_objetivo != NULL && *bateria_objetivo != '\0') {
      informacion_procesada->informacion_baterias.baterias[numero_bateria].bateria_objetivo = (OSQPFloat)atof(bateria_objetivo);
     }
     else {
      printf("Error el dato de bateria objetivo es NULL o está vacío.\n");
     }
    char * considerar_objetivo = informacion_entrada->datos_baterias.informacion_baterias.datos[numero_bateria + 1][columna_consideracion_objetivo_carga];

     if (considerar_objetivo != NULL && *considerar_objetivo != '\0') { // Verificar que el dato no sea NULL y no esté vacío
      informacion_procesada->informacion_baterias.baterias[numero_bateria].considerar_objetivo = (OSQPFloat)atof(considerar_objetivo);
    }
    else {
      printf("Error: el dato de potencia máxima es NULL o está vacío.\n");
      // Manejar el error como sea apropiado
    }


     char* capacidad_bateria = informacion_entrada->datos_baterias.informacion_baterias.datos[numero_bateria + 1][columna_capacidad_bateria];

     if (capacidad_bateria != NULL && *capacidad_bateria != '\0') { // Verificar que el dato no sea NULL y no esté vacío
       informacion_procesada->informacion_baterias.baterias[numero_bateria].capacidad_bateria = (OSQPFloat)atof(capacidad_bateria);
     }
     else {
       printf("Error: el dato de potencia máxima es NULL o está vacío.\n");
       // Manejar el error como sea apropiado
     }

     char* numero_terminal = informacion_entrada->datos_baterias.informacion_baterias.datos[numero_bateria + 1][columna_terminal];

     if (numero_terminal != NULL && *numero_terminal != '\0') { // Verificar que el dato no sea NULL y no esté vacío
       informacion_procesada->informacion_baterias.baterias[numero_bateria].numero_terminal = (OSQPFloat)atof(numero_terminal);
     }
     else {
       printf("Error: el dato de potencia máxima es NULL o está vacío.\n");
       // Manejar el error como sea apropiado
     }

     //Se carga la fecha inicial de la bateria 
     cargar_fecha(&(informacion_entrada->datos_baterias.informacion_baterias), &(informacion_procesada->informacion_baterias.baterias[numero_bateria].fecha_inicio),
       columna_anyo_inicial, columna_mes_inicial, columna_dia_inicial, columna_hora_inicial, columna_minuto_inicial,
       numero_bateria + 1, SI_INCLUIR_MINUTO);

     obtener_punto_simulacion(&(informacion_procesada->informacion_baterias.baterias[numero_bateria].punto_inicio),
       &(informacion_procesada->informacion_baterias.baterias[numero_bateria].fecha_inicio), puntos_adicionales);

     //Se carga la fecha final de la bateria
     cargar_fecha(&(informacion_entrada->datos_baterias.informacion_baterias), &(informacion_procesada->informacion_baterias.baterias[numero_bateria].fecha_final),
       columna_anyo_final, columna_mes_final, columna_dia_final, columna_hora_final, columna_minuto_final, numero_bateria + 1, SI_INCLUIR_MINUTO);

     obtener_punto_simulacion(&(informacion_procesada->informacion_baterias.baterias[numero_bateria].punto_final),
       &(informacion_procesada->informacion_baterias.baterias[numero_bateria].fecha_final), puntos_adicionales);


     char* considerar_objetivo_cadena = informacion_entrada->datos_baterias.informacion_baterias.datos[numero_bateria + 1][columna_consideracion_objetivo_carga];
  

     //Se comprueba si hay que cargar fecha objetivo
     if(considerar_objetivo_cadena != NULL && strings_iguales(considerar_objetivo_cadena, "si") == true) {
       
       //Se carga la fecha objetivo
       cargar_fecha(&(informacion_entrada->datos_baterias.informacion_baterias), &(informacion_procesada->informacion_baterias.baterias[numero_bateria].fecha_objetivo),
         columna_anyo_objetivo, columna_mes_objetivo, columna_dia_objetivo, columna_hora_objetivo, columna_minuto_objetivo,
         numero_bateria + 1, SI_INCLUIR_MINUTO);

       obtener_punto_simulacion(&(informacion_procesada->informacion_baterias.baterias[numero_bateria].punto_objetivo),
         &(informacion_procesada->informacion_baterias.baterias[numero_bateria].fecha_objetivo), puntos_adicionales);

       informacion_procesada->informacion_baterias.baterias[numero_bateria].considerar_objetivo = true;
     }
     else {
       
       informacion_procesada->informacion_baterias.baterias->punto_objetivo = 0;
       memset(&(informacion_procesada->informacion_baterias.baterias[numero_bateria].fecha_objetivo), 0, sizeof(struct tm));
       informacion_procesada->informacion_baterias.baterias[numero_bateria].considerar_objetivo = false;
     }
  }
  return EXITO;
}

/*Este subprograma se utiliza para procesar la informacion de los precios*/
int procesar_informacion_precio(informacion_entrada_t* informacion_entrada, informacion_procesada_t* informacion_procesada) {
  
  // Se carga el numero de precios que hay = numero de horas
  informacion_procesada->informacion_precio_compra.numero_horas = informacion_entrada->datos_precio_compra.informacion_precio.filas - 1;
  informacion_procesada->informacion_precio_venta.numero_horas = informacion_entrada->datos_precio_venta.informacion_precio.filas - 1;
  //Se reserva memoria para almacenar la informacion procesada de los precios:
  informacion_procesada->informacion_precio_compra.precios = (precio_t*)malloc(informacion_procesada->informacion_precio_compra.numero_horas * sizeof(precio_t));
  informacion_procesada->informacion_precio_venta.precios = (precio_t*)malloc(informacion_procesada->informacion_precio_venta.numero_horas * sizeof(precio_t));
  

  //Se crea una variable booleana para identificar a cuantos puntos de simulacion hay que asignarles un precio de la
  //electricidad
  bool fin_bucle = false;
  //Se carga una variable para acceder a un precio de la electricidad u otro
  int  index_precio_actual = 1;
  //Cargo las posiciones donde se encuentra la informacion en el csv de los precios de compra y venta
  int columna_precio_compra = informacion_entrada->datos_precio_compra.posiciones_informacion_precio.columna_precio;
  int columna_precio_venta = informacion_entrada->datos_precio_venta.posiciones_informacion_precio.columna_precio;
  int columna_anyo_precio_compra = informacion_entrada->datos_precio_compra.posiciones_informacion_precio.ubicacion_fecha_precios.columna_anyo;
  int columna_mes_precio_compra = informacion_entrada->datos_precio_compra.posiciones_informacion_precio.ubicacion_fecha_precios.columna_mes;
  int columna_dia_precio_compra = informacion_entrada->datos_precio_compra.posiciones_informacion_precio.ubicacion_fecha_precios.columna_dia;
  int columna_hora_precio_compra = informacion_entrada->datos_precio_compra.posiciones_informacion_precio.ubicacion_fecha_precios.columna_hora;
  int columna_anyo_precio_venta = informacion_entrada->datos_precio_venta.posiciones_informacion_precio.ubicacion_fecha_precios.columna_anyo;
  int columna_mes_precio_venta = informacion_entrada->datos_precio_venta.posiciones_informacion_precio.ubicacion_fecha_precios.columna_mes;
  int columna_dia_precio_venta = informacion_entrada->datos_precio_venta.posiciones_informacion_precio.ubicacion_fecha_precios.columna_dia;
  int columna_hora_precio_venta = informacion_entrada->datos_precio_venta.posiciones_informacion_precio.ubicacion_fecha_precios.columna_hora;
  int fila_inicial_precio_compra = 1;
  int fila_inicial_precio_venta = 1;
  /*Se carga el primer precio de compra y venta asi como el primer punto de simulacion, asi como la primera fecha*/
 
  OSQPFloat precio_actual_compra = atof(informacion_entrada->datos_precio_compra.informacion_precio.datos[index_precio_actual][columna_precio_compra]);
  OSQPFloat precio_actual_venta = atof(informacion_entrada->datos_precio_venta.informacion_precio.datos[index_precio_actual][columna_precio_venta]);


  informacion_procesada->informacion_precio_compra.precios[0].precio = precio_actual_compra;
  informacion_procesada->informacion_precio_venta.precios[0].precio = precio_actual_venta;

  /*Se carga el numero de puntos de simulacion que hay en total*/
  int numero_puntos_simulacion = informacion_procesada->informacion_puntos_simulacion.numero_puntos_simulacion;
  

  if (cargar_fecha(&(informacion_entrada->datos_precio_compra.informacion_precio), &(informacion_procesada->informacion_precio_compra.precios[0].fecha_asociada),
    columna_anyo_precio_compra, columna_mes_precio_compra, columna_dia_precio_compra, columna_hora_precio_compra,
    0, fila_inicial_precio_compra, NO_INCLUIR_MINUTO) == ERROR) {
    printf("No se ha podido cargar la fecha inicial del precio de compra\n");
    registrar_error("No se ha podido cargar la fecha inicial del precio de compra\n", REGISTRO_ERRORES);
    return ERROR;
  }

  if (cargar_fecha(&(informacion_entrada->datos_precio_venta.informacion_precio), &(informacion_procesada->informacion_precio_venta.precios[0].fecha_asociada),
    columna_anyo_precio_venta, columna_mes_precio_venta, columna_dia_precio_venta, columna_hora_precio_venta, 0,
    fila_inicial_precio_venta, NO_INCLUIR_MINUTO) == ERROR) {
    printf("No se ha podido cargar la fecha inicial del precio de venta\n");
    registrar_error("No se ha podido cargar la fecha inicial del precio de venta\n", REGISTRO_ERRORES);
    return ERROR;
  }

  /*Se introduce el punto inicial de los precios*/
  informacion_procesada->informacion_precio_compra.precios[0].punto_inicial = 0;
  informacion_procesada->informacion_precio_venta.precios[0].punto_inicial = 0;

  /*Se carga el valor del punto actual que se está examinando*/
  int punto_actual = 0;
  int precio_actual = 0;

  
  while (!fin_bucle) {
    
    //Cargo el precio de la hora actual
    if (comprobar_hora(informacion_procesada->informacion_precio_compra.precios[precio_actual].fecha_asociada,
      informacion_procesada->informacion_puntos_simulacion.puntos_simulacion[punto_actual].fecha_punto) == false) {

      
      informacion_procesada->informacion_precio_compra.precios[precio_actual].punto_final = punto_actual - 1;
      informacion_procesada->informacion_precio_venta.precios[precio_actual].punto_final = punto_actual - 1;
      precio_actual++;

      if (cargar_fecha(&(informacion_entrada->datos_precio_compra.informacion_precio), &(informacion_procesada->informacion_precio_compra.precios[precio_actual].fecha_asociada),
        columna_anyo_precio_compra, columna_mes_precio_compra, columna_dia_precio_compra, columna_hora_precio_compra, 0, precio_actual + 1,
        NO_INCLUIR_MINUTO) == ERROR) {
        printf("Error procesando la informacion de los precios, en la carga de las fechas de los precios de compra\n");
        registrar_error("Error procesando la informacion de los precios, en la carga de las fechas de los precios de compra\n", REGISTRO_ERRORES);
        return ERROR;
      }

      if (cargar_fecha(&(informacion_entrada->datos_precio_venta.informacion_precio), &(informacion_procesada->informacion_precio_venta.precios[precio_actual].fecha_asociada),
        columna_anyo_precio_venta, columna_mes_precio_venta, columna_dia_precio_venta, columna_hora_precio_venta, 0, precio_actual + 1,
        NO_INCLUIR_MINUTO) == ERROR) {
        printf("Error procesando la informacion de los precios, en la carga de las fechas de los precios de venta\n");
        registrar_error("Error procesando la informacion de los precios en la carga de las fechas de los precios de venta\n", REGISTRO_ERRORES);
        return ERROR;
      }
      OSQPFloat precio_actual_compra = atof(informacion_entrada->datos_precio_compra.informacion_precio.datos[precio_actual][columna_precio_compra]);
      OSQPFloat precio_actual_venta = atof(informacion_entrada->datos_precio_venta.informacion_precio.datos[precio_actual][columna_precio_venta]);
      informacion_procesada->informacion_precio_compra.precios[precio_actual].precio = precio_actual_compra;
      informacion_procesada->informacion_precio_venta.precios[precio_actual].precio = precio_actual_venta;
      informacion_procesada->informacion_precio_compra.precios[precio_actual].punto_inicial = punto_actual;
      informacion_procesada->informacion_precio_venta.precios[precio_actual].punto_inicial = punto_actual;

    }
    punto_actual++;
    
    if (numero_puntos_simulacion - 1 < punto_actual) {
      fin_bucle = true;
    }
  }
  informacion_procesada->informacion_precio_compra.precios[precio_actual].punto_final = punto_actual - 1;
  informacion_procesada->informacion_precio_venta.precios[precio_actual].punto_final = punto_actual - 1;

 
  return EXITO;
}
int configurar_puntos_simulacion(informacion_entrada_t* informacion_entrada, informacion_procesada_t* informacion_procesada) {
  //Se cargan elementos temporales claves de la simulacion, como la resolucion de la simulacion, la fecha inicial
  //y la fecha final.
  //

  
  struct tm* puntero_auxiliar = NULL;
  struct tm** fechas_adicionales = &puntero_auxiliar;

  //Se carga la informacion de los puntos adicionales
  informacion_puntos_adicionales_t* informacion_puntos_adicionales = &(informacion_procesada->informacion_puntos_adicionales);
 
  //Cargo la ubicacion de las fechas iniciales y finales del algoritmo
  //Se definen variables para almacenar las fechas iniciales y finales del cálculo de optimizacion
  struct tm* fecha_inicial_algoritmo;
  struct tm* fecha_final_algoritmo;
  /*Se crea un array para almacenar las fechas adicionales que deben ser añadidas (ida y partida de vehiculos por
    ejemplo*/
  
  int numero_fechas_adicionales = 0;

  fecha_inicial_algoritmo = malloc(sizeof(struct tm));
  fecha_final_algoritmo   = malloc(sizeof(struct tm));
  if ((fecha_inicial_algoritmo == NULL) || (fecha_final_algoritmo == NULL)) {
    printf("Problema con las fechas iniciales y finales del algoritmo\n");
    registrar_error("Problema con las fechas iniciales y finales del algoritmo\n",REGISTRO_ERRORES);
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
  int delta_resolucion = atoi(informacion_entrada->datos_algoritmo.informacion_algoritmo.datos[fila_valores][columna_resolucion_minutos]);
  

  

  //Se cargan las fechas iniciales y finales del algoritmo
  cargar_fecha(&(informacion_entrada->datos_algoritmo.informacion_algoritmo), fecha_inicial_algoritmo, columna_anyo_inicial,
    columna_mes_inicial, columna_dia_inicial, columna_hora_inicial, columna_minuto_inicial,
    fila_valores, SI_INCLUIR_MINUTO);
 
  
  cargar_fecha(&(informacion_entrada->datos_algoritmo.informacion_algoritmo), fecha_final_algoritmo, columna_anyo_final,
    columna_mes_final, columna_dia_final, columna_hora_final, columna_minuto_final,
    fila_valores, SI_INCLUIR_MINUTO);

  

 
  
  if (leer_fechas_adicionales(&(informacion_entrada->datos_vehiculos),fechas_adicionales, &(informacion_entrada->datos_baterias),
    &(informacion_puntos_adicionales->numero_puntos), fecha_inicial_algoritmo, fecha_final_algoritmo) == ERROR) {
    printf("No se ha podido añadir las fechas adicionales a la simulacion\n");
    registrar_error("No se ha podido añadir las fechas adicionales a la simulacion", REGISTRO_ERRORES);
    return ERROR;
  }
  
  //Se guarda la informacion de las fechas adicionales a añadir en las variables correspondiente
   
 
  informacion_puntos_adicionales->puntos = NULL;
  puntos_adicionales_t ** puntos_adicionales = &(informacion_puntos_adicionales->puntos);
 


  
  

  if (cacular_puntos_simulacion(informacion_entrada, fechas_adicionales, informacion_procesada,
    fecha_inicial_algoritmo, fecha_final_algoritmo, delta_resolucion,
    &(informacion_puntos_adicionales->numero_puntos)) == ERROR) {
    printf("No se ha podido calcular el numero de puntos de simulacion\n");
    registrar_error("No se ha podido calcular el numero de puntos de simulacion\n", REGISTRO_ERRORES);
    return ERROR;
  }
  /*
  
  /* */
  return EXITO;
}



//Este subprograma se utiliza para procesar la informacion de entrada para guardarlo
//facilmente en las matrices->

int procesar_informacion_entrada(informacion_entrada_t*    informacion_entrada,
                                  informacion_procesada_t* informacion_procesada) {

  //Se crea una variable para almacenar las fechas adicionales a añadir, esta variable va a ser utilizada para
  //reconocer más rápidamente a que punto de simulación corresponde la ida o partida de los vehículos o baterías
 

  //Se almacena la informacion de restriccion leída del csv de las restricciones
  procesar_informacion_restricciones(&(informacion_entrada->datos_restricciones), &(informacion_procesada->informacion_restricciones_sistema));

  

  
  //Se configuran cuantos puntos de simulacion son necesarios 
  if (configurar_puntos_simulacion(informacion_entrada, informacion_procesada) == ERROR) {
    printf("No se ha podido configurar los puntos de simulacion correctamente\n");
    registrar_error("No se ha podido configurar los puntos de simulacion correctamente\n", REGISTRO_ERRORES);
    return ERROR;
  }
  
  /*
  
  if (&(informacion_procesada->informacion_puntos_adicionales )== NULL) {
    // Handle memory allocation failure
    printf("Error al asignar memoria para puntos adicionales\n");
    registrar_error("Error al asignar memoria para puntos adicionales\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se proceda la informacion de los vehiculos 
  if (procesar_informacion_vehiculos(informacion_entrada, informacion_procesada) == ERROR) {
    printf("No se ha podido configurar la informacion de los vehiculos correctamente\n");
    registrar_error("No se ha podido configurar la informacion de los vehiculos correctamente\n",REGISTRO_ERRORES);
    return ERROR;
  }

  //Se procesa la informacion de las baterias 
  if (procesar_informacion_baterias(informacion_entrada, informacion_procesada) == ERROR) {
    printf("No se ha podido configurar la informacion de las baterias\n");
    registrar_error("No se ha podido configurar la informacion de las baterias\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se procesa la informacion de los precios 
  if (procesar_informacion_precio(informacion_entrada, informacion_procesada) == ERROR) {
    printf("No se ha podido configurar la informacion de los precios\n");
    registrar_error("No se ha podido configurar la informacion de los precios\n", REGISTRO_ERRORES);
    return ERROR;
  }
  procesar_informacion_terminales(informacion_entrada, informacion_procesada);


  */
  return EXITO;
}