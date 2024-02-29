#include "osqp.h"
#include <stdlib.h>
#include <stdio.h>



#define _CRT_SECURE_NO_WARNINGS
  //#pragma comment(lib, "OSQPLIB.lib")
#include <stdio.h>
#include <locale.h>
#include <io.h>
#include "comprobar_lectura_datos.h"
#include "comprobar_informacion_procesada.h"
#include "definiciones_globales.h"
#include "inicializar_csv.h"
#include "inicializar_informacion_procesada.h"
#include "leer_entradas.h"
#include "liberar_memoria.h"
#include "main.h"
#include "osqp.h"
#include "preparar_problema_optimizacion.h"
#include "procesar_informacion_entrada.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include "verificar_entradas.h"



  static void ajustes_idioma() {

#ifdef _WIN32
    setlocale(LC_ALL, "es_ES.UTF-8");
#elif __linux__
    setlocale(LC_ALL, "es_ES.UTF-8");
#endif
  }

  static void ajustes_punto_decimal() {
    setlocale(LC_NUMERIC, "C");
  }

  static void printTm_4(const struct tm* date) {
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



  int main() {
    
  
    // Se crean variables para almacenar los datos de los CSV
    informacion_entrada_t   informacion_sistema;
    informacion_procesada_t informacion_procesada;
    //Se define la estructura donde van a ir contenidas las matrices del problema de optimizacion
    problema_optimizacion_t problema_optimizacion = { 0 };

    // Se asume éxito inicialmente.
    int estado = EXITO;

  
    //Se establece como idioma el español.
    ajustes_idioma();

    //Se establece como separador decimal el punto
    ajustes_punto_decimal();
   
    

    //Se borran los errores de ejecuciones anteriores del programa
    borrar_contenido_log(REGISTRO_ERRORES);

    // Se inicializan las variables que almacenan los datos de entrada.
    if (inicializar_informacion_entrada(&informacion_sistema) == ERROR) {
        printf("Fallo en la inicialización del algoritmo\n");
        registrar_error("Fallo en la inicializacion de los datos de entrada", REGISTRO_ERRORES);
        goto fin_programa;
    }

    
  
    //Se pasa a leer los datos de entrada->
    if (leer_entradas(&informacion_sistema) == ERROR) {
        printf("Fallo en la lectura de las entradas\n");
        registrar_error("Fallo en la lectura de los datos de entrada", REGISTRO_ERRORES);
        goto fin_programa;
    }
    
    
    //Se inspecciona si la lectura se esta haciendo adecuadamente
    if (comprobar_informacion_entrada(&informacion_sistema) == ERROR) {
        printf("Fallo en la comprobacion de la informacion de entrada\n");
        registrar_error("Fallo en la comprobacion de la informacion de entrada", REGISTRO_ERRORES);
        goto fin_programa;
    }

    
    
    //Se verifica que la informacion de entrada es correcta
    if (verificar_entradas(&informacion_sistema) == ERROR) {
        printf("Las entradas son incorrectas\n");
        registrar_error("Fallo las entradas son incorrectas\n", REGISTRO_ERRORES);
        goto fin_programa;
    }
  
   

    //Se procesa la informacion de entrada
    
    inicializar_informacion_procesada(&informacion_procesada);
    
    
    
    if (procesar_informacion_entrada(&informacion_sistema, &informacion_procesada) == ERROR) {
      printf("La informacion no ha podido ser procesada correctamente\n");
      registrar_error("La informacion no ha podido ser procesada correctamente\n", REGISTRO_ERRORES);
      goto fin_programa;
    }

   
    //Se procesa la informacion 
    if (comprobar_informacion_procesada(informacion_procesada) == ERROR) {
      printf("La informacion procesada no ha sido correctamente\n");
      registrar_error("La informacion procesada no ha sido correctamente\n", REGISTRO_ERRORES);
      goto fin_programa;
    }

    
    if (preparar_problema_optimizacion(&informacion_procesada,&problema_optimizacion) == ERROR) {
      printf("Las matrices del problema de optimizacion no han sido calculadas correctamente\n");
      registrar_error("Las matrices del problema de optimización no han sido calculadas correctamente\n", REGISTRO_ERRORES);
      goto fin_programa;
    }
    

   fin_programa:

    // Se libera la memoria reservada
    liberar_memoria_informacion_procesada(&informacion_procesada);
    printf("Memoria de informacion procesada liberada\n");
    liberar_memoria_csvs(&informacion_sistema);
    //liberar_memoria_problema_optimizacion(&problema_optimizacion);

    
    
  }


