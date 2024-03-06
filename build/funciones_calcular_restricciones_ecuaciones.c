
#include "definiciones_globales.h"
#include "funciones_calcular_a_x.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/*Este subprograma se utiliza para crear un array en el que se almacenan los estados de baterias iniciales*/

int crear_array_baterias_iniciales(informacion_procesada_t* informacion_sistema,int numero_elementos_terminales,
  OSQPFloat** baterias_iniciales, int* puntos_iniciales) {

  //Cargo el numero de vehiculos que se tienen en el sistema
  int numero_elementos_vehiculos = informacion_sistema->informacion_vehiculos.numero_vehiculos;
  int numero_elementos_baterias = informacion_sistema->informacion_baterias.numero_baterias;



  //Creo una serie de variables booleanas para los bucles tipo while
  bool bateria_anyadida;
  bool vehiculo_anyadido;
  bool elemento_anyadido;
  int index_bateria_inicial = 0;
  int index_adicional = 0;
  int index_vehiculos = 0;
  int index_baterias = 0;

  //Reservo memoria para el array que va a almacenar el array de los estados de batería inicial
  *baterias_iniciales = (OSQPFloat*)malloc(numero_elementos_terminales * sizeof(OSQPFloat));

  //Se comprueba si la reserva de memoria ha sido exitosa
  if (*baterias_iniciales == NULL) {
    printf("No se ha podido reservar memoria para el array de los estados de baterías iniciales\n");
    registrar_error("No se ha podido reservar memoria para el array de los estados de baterías iniciales\n", REGISTRO_ERRORES);
    return ERROR;
  }

  for (int numero_elemento = 0; numero_elemento < numero_elementos_terminales; numero_elemento++) {
    elemento_anyadido = false;
    index_vehiculos = 0;
    index_baterias = 0;

    while ((elemento_anyadido == false)||(index_vehiculos < numero_elementos_vehiculos)) {

      if (informacion_sistema->informacion_vehiculos.vehiculos[index_vehiculos].punto_inicio == puntos_iniciales[numero_elemento]) {
        *baterias_iniciales[numero_elemento] = informacion_sistema->informacion_vehiculos.vehiculos[index_vehiculos].bateria_inicial;
        elemento_anyadido = true;
      }
      index_vehiculos++;
    }

    while ((elemento_anyadido == false) || (index_baterias < numero_elementos_baterias)) {
      if (informacion_sistema->informacion_baterias.baterias[index_baterias].punto_inicio == puntos_iniciales[numero_elemento]) {
        *baterias_iniciales[numero_elemento] = informacion_sistema->informacion_baterias.baterias[index_baterias].bateria_inicial;
        elemento_anyadido = false;
      }
      index_baterias++;
    }
   

  }

  return EXITO;
}



/*El siguiente subprograma se utiliza para calcular los terminos de la igualdad del balance en la fase,
  por caracterísiticas de la ecuación (ver paper ecuación nº 1) */

void calcular_resultado_balance_fase(informacion_procesada_t* informacion_sistema,OSQPFloat* l, OSQPFloat *u) {

  //Podría haber términos diferentes de 0, si hay cargas o energía adicional viniendo por ejemplo de energías
  //renovables.

  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Cargo los offset para poder situar los términos en las posiciones correctas de los vectores l
  int offset_balance_fase_R = NUMERO_VARIABLES * numero_puntos_simulacion;
  int offset_balance_fase_S = offset_balance_fase_R + numero_puntos_simulacion;
  int offset_balance_fase_T = offset_balance_fase_S + numero_puntos_simulacion;


  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    l[offset_balance_fase_R + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_balance_fase_S + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_balance_fase_T + punto_simulacion] = (OSQPFloat)0.0;
  }
}

void calcular_ecuacion_balance_terminal(informacion_procesada_t* informacion_sistema, OSQPFloat* l, OSQPFloat* u,
     int numero_terminal, informacion_carga_terminales_t* elementos_carga_terminales) {


  // Se define una variable en la que se carga el numero de elementos que tienen su carga programada en el terminal
  int numero_elementos_terminales = elementos_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;

  //Se carga los puntos iniciales y finales en los que hay conectado un vehiculo o bateria conectado al terminal

  //Se pasan a rellenar los terminos de las ecuaciones del balance de baterias
  //Se crea una variable para poder acceder al array que almacena los puntos iniciales
  int index_elementos_adicionales = 0;

  //Se carga el numero de puntos de simulacion

  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se define una variable para acceder a las posiciones correctas del vector l y u
  int offset_ecuacion_balance_bateria = (NUMERO_VARIABLES + (numero_terminal))*numero_puntos_simulacion;

  if (index_elementos_adicionales < numero_elementos_terminales) {

    //Se carga el punto inicial y final del siguiente vehiculo o bateria que tiene su carga programada en
    //el terminal

    int punto_inicial = elementos_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elementos_adicionales].punto_inicio;
   

    for (int punto_simulacion = 0; punto_simulacion < numero_elementos_terminales; punto_simulacion++) {

      if (index_elementos_adicionales < numero_elementos_terminales) {

        if (punto_simulacion == punto_inicial) {
          l[offset_ecuacion_balance_bateria + punto_simulacion] = elementos_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elementos_adicionales].bateria_inicial;
          u[offset_ecuacion_balance_bateria + punto_simulacion] = elementos_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elementos_adicionales].bateria_inicial;
          index_elementos_adicionales++;
        }

        else {
          l[offset_ecuacion_balance_bateria + punto_simulacion] = (OSQPFloat) 0.0;
          u[offset_ecuacion_balance_bateria + punto_simulacion] = (OSQPFloat) 0.0;
        }

      }
      else {
        l[offset_ecuacion_balance_bateria + punto_simulacion] =(OSQPFloat) 0.0;
        u[offset_ecuacion_balance_bateria + punto_simulacion] =(OSQPFloat) 0.0;
      }

    }
  }
  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {

    l[offset_ecuacion_balance_bateria + punto_simulacion] = (OSQPFloat)0.0;
    u[offset_ecuacion_balance_bateria + punto_simulacion] = (OSQPFloat)0.0;
  
  }
    }
  
   


void calcular_ecuaciones_balance_bateria(informacion_procesada_t* informacion_sistema, OSQPFloat* l, OSQPFloat* u,
  informacion_carga_terminales_t* elementos_carga_terminal) {

  //Cargo el numero de puntos de simulacion  que hay en la simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int offset_ecuaciones = (NUMERO_VARIABLES + 3) * numero_puntos_simulacion;


  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    calcular_ecuacion_balance_terminal(informacion_sistema, l, u, numero_terminal, elementos_carga_terminal);
  }
  
}


/*Se utiliza el siguiente subprograma para calcular los resultados de las ecuaciones de restricción restantes*/

void calcular_resto_ecuaciones(informacion_procesada_t* informacion_sistema, OSQPFloat* l, OSQPFloat* u) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  int offset_balance_entrada_salida = 51*numero_puntos_simulacion;
  int offset_balance_entrada_salida_R = 52 * numero_puntos_simulacion;
  int offset_balance_entrada_salida_S = 53 * numero_puntos_simulacion;
  int offset_balance_entrada_salida_T = 54 * numero_puntos_simulacion;
  int offset_balance_entrada = 55 * numero_puntos_simulacion;
  int offset_balance_salida = 56 * numero_puntos_simulacion;
  int offset_balance_fase_red = 57 * numero_puntos_simulacion;

  //Cargo los diferentes offset para situar los resultados de los terminos en las ecuaciones apropiadas
  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {

    //Se calcula los terminos l  de las ecuaciones (son todos 0)
    l[offset_balance_entrada_salida + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_balance_entrada_salida_R + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_balance_entrada_salida_S + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_balance_entrada_salida_T + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_balance_entrada + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_balance_salida + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_balance_fase_red + punto_simulacion] = (OSQPFloat)0.0;

    //Se calculan los terminos u de las ecuaciones (son todos 0)
    u[offset_balance_entrada_salida + punto_simulacion] = (OSQPFloat)0.0;
    u[offset_balance_entrada_salida_R + punto_simulacion] = (OSQPFloat)0.0;
    u[offset_balance_entrada_salida_S + punto_simulacion] = (OSQPFloat)0.0;
    u[offset_balance_entrada_salida_T + punto_simulacion] = (OSQPFloat)0.0;
    u[offset_balance_entrada + punto_simulacion] = (OSQPFloat)0.0;
    u[offset_balance_salida + punto_simulacion] = (OSQPFloat)0.0;
    u[offset_balance_fase_red + punto_simulacion] = (OSQPFloat)0.0;
  }
}