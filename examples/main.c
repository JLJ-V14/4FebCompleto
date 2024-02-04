#include "osqp.h"
#include <stdlib.h>
#include <stdio.h>



#define _CRT_SECURE_NO_WARNINGS
  //#pragma comment(lib, "OSQPLIB.lib")
#include <stdio.h>
#include <locale.h>
#include "comprobar_lectura_datos.h"
#include "definiciones_globales.h"
#include "inicializar_csv.h"
#include "leer_entradas.h"
#include "liberar_memoria.h"
#include "main.h"
#include <io.h>
#include "osqp.h"
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



  int main() {

  
    // Se crean variables para almacenar los datos de los CSV
    informacion_entrada_t informacion_sistema;

    //Se define la estructura donde van a ir contenidas las matrices del problema de optimizacion
    //problema_optimizacion_t problema_optimizacion = { 0 };

    // Se asume éxito inicialmente.
    int estado = EXITO;

    //Se establece como idioma el español.
    ajustes_idioma();

    //Se establece como separador decimal el punto
    ajustes_punto_decimal();
   
    
    
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
        registrar_error("Fallo las entradas son incorrectas", REGISTRO_ERRORES);
        goto fin_programa;
    }
    
    printf("Se han verificado las entradas\n");
    
    



  fin_programa:
    // Se libera la memoria reservada
    liberar_memoria_csvs(&informacion_sistema);
   // finalizar_problema_optimizacion(&problema_optimizacion);


    
  }


