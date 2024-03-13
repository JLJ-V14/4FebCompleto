
#include "definiciones_globales.h"
#include "funciones_auxiliares_l_u.h"
#include "funciones_calcular_a_x.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>

void calcular_limite_inferior_restricciones_bateria(informacion_procesada_t* informacion_sistema,OSQPFloat *l) {
  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Creo una variable para situar los terminos en el punto correcto del vector
  int offset_index_vector_l ;
  //Como hay 12 terminales hay 12 variables de estados de bateria
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    offset_index_vector_l = numero_terminal * numero_puntos_simulacion;

    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
      l[offset_index_vector_l + punto_simulacion] =(OSQPFloat) 0.0;
    }
  }
}




/*Este subprograma se utiliza para indicar los límites de las potencias de los terminales, si se tiene un vehiculo o
  no se tiene nada el límite inferior es 0, si se tiene una batería puede ser bidireccional*/

void calcular_limite_inferior_restricciones_potencia_terminales(informacion_procesada_t* informacion_sistema,
  OSQPFloat* l, informacion_carga_terminales_t* elementos_programados_carga_terminales) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Se crea una variable para situar los terminos en el punto correcto del vector
  int offset_index_vector_l;


  // Se crea un array para almacenar la minima potencia que puede intercambiar el terminal dependiendo de si
  //hay una bateria conectada o no.
  OSQPFloat * potencias_maxima_terminal = NULL;

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    //Calculo el offset para las potencias de los terminales están después de las restricciones de borde
    int offset_potencia_terminal = NUMERO_TERMINALES * numero_puntos_simulacion;

    int  numero_baterias_terminal = 0;
    int  index_elemento_terminal = 0;

    int punto_inicial;
    int punto_final;

    //Se carga el numero de elementos que tienen su carga programada en el terminal actual
    int numero_elementos_terminal = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;



    //Se llama a este subprograma para calcular la mínima potencia que pueden intercambiar las baterías, ya que hay
    //que tener en cuenta que las baterias pueden donar energía a otros elementos en el sistema.

    calcular_minima_potencia_baterias(informacion_sistema, numero_terminal, elementos_programados_carga_terminales);


    //Se carga los puntos iniciales y finales del primer elemento que tiene su carga programada en el terminal
    if (numero_elementos_terminal>index_elemento_terminal ) {
      punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].punto_inicio;
      punto_final = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].punto_final;
    }




    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {

      if (numero_elementos_terminal > index_elemento_terminal) {

        //Si el siguiente elemento del terminal es un vehículo se salta.

          if (comprobar_rango(punto_simulacion, punto_inicial, punto_final) == true) {

            //Dependiendo de si la carga programada en el terminal, es un vehículo o una batería el valor a escribir
            //en el vector l es diferente.
            bool bateria = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].bateria;

            if (bateria == false) {
              l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] = (OSQPFloat)0.0;

              //Si el punto de simulacion coincide con el punto final del elmento que tiene su carga programada en el terminal
              if (punto_simulacion == punto_final) {
                index_elemento_terminal++;

                if (numero_elementos_terminal > index_elemento_terminal) {
                  //Si hay mas elementos que tengan su carga programada en el terminal se actualiza las variables punto inicial y punto final
                  punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].punto_inicio;
                  punto_final = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].punto_final;
                }

              }
            }

            //Si es el primer punto SOC = SOC inicial y el terminal no puede haber intercambiado potencia
            else if (punto_simulacion == punto_inicial){
              l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] =(OSQPFloat) 0.0;
            }

            else if (punto_simulacion == punto_final) {
              l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].potencia_minima;
              index_elemento_terminal++;

              if (numero_elementos_terminal > index_elemento_terminal) {
                //Se actualiza el punto inicial y el punto final 
                punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].punto_inicio;
                punto_final = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].punto_final;
              }

            }


            else {
              l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_terminal].potencia_minima;
            }

          }

          else {
            l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] = (OSQPFloat)0.0;
          }
        }
      else {
        //Si no hay una bateria presente el limite inferior es 0
        l[offset_potencia_terminal + numero_terminal * numero_puntos_simulacion + punto_simulacion] = (OSQPFloat)0.0;
      }
      }
    }
  
 
  
}

/*Este subprograma se utiliza para calcular las restricciones de borde inferiores de las potencias que intercambia
  la red con el sistema Pgrid,t Pgrid in,t Pgrid out,t*/

void calcular_restricciones_borde_inferiores_potencia_red(informacion_procesada_t* informacion_sistema, OSQPFloat* l) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int offset_potencia_red = (NUMERO_TERMINALES * 2) * numero_puntos_simulacion;
  int offset_potencia_entrada_red = offset_potencia_red + numero_puntos_simulacion;
  int offset_potencia_salida_red = offset_potencia_entrada_red + numero_puntos_simulacion;

  //Se carga los valores máximos de potencia
  OSQPFloat potencia_minima_red = informacion_sistema->informacion_restricciones_sistema.potencia_minima_red;
  OSQPFloat potencia_entrada_red = 0.0;
  OSQPFloat potencia_salida_red = 0.0;

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    l[offset_potencia_red + punto_simulacion] = potencia_minima_red;
    l[offset_potencia_entrada_red + punto_simulacion] = potencia_entrada_red;
    l[offset_potencia_salida_red + punto_simulacion] = potencia_salida_red;
  }
}

/*Este subprograma se utiliza para calcular los límites inferiores de los terminos P grid p,t */
void calcular_limite_inferior_borde_potencia_red_fases(informacion_procesada_t* informacion_sistema, OSQPFloat* l) {
  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  int offset_potencia_red_fase_R = 27 * numero_puntos_simulacion;
  int offset_potencia_red_fase_S = 28 * numero_puntos_simulacion;
  int offset_potencia_red_fase_T = 29 * numero_puntos_simulacion;

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    l[offset_potencia_red_fase_R + punto_simulacion] = informacion_sistema->informacion_restricciones_sistema.potencia_minima_red_R;
    l[offset_potencia_red_fase_S + punto_simulacion] = informacion_sistema->informacion_restricciones_sistema.potencia_minima_red_S;
    l[offset_potencia_red_fase_T + punto_simulacion] = informacion_sistema->informacion_restricciones_sistema.potencia_minima_red_T;
  }
}

/*Este subprograma se utiliza para calcular las restricciones inferiores de borde del termino P in grid p,t*/
void calcular_limite_inferior_borde_potencia_red_entrada_fases(informacion_procesada_t* informacion_sistema,OSQPFloat* l) {
  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Cargo los offset a partir de los cuales están situados las diferentes variables
  int offset_potencia_red_fase_R = 30 * numero_puntos_simulacion;
  int offset_potencia_red_fase_S = 31 * numero_puntos_simulacion;
  int offset_potencia_red_fase_T = 32 * numero_puntos_simulacion;

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    l[offset_potencia_red_fase_R + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_potencia_red_fase_S + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_potencia_red_fase_T + punto_simulacion] = (OSQPFloat)0.0;
  }
}

/*Este subprograma se utiliza para calcualr los términos inferiores de las restricciones de borde Pout grid p,t*/
void calcular_limite_inferior_borde_potencia_red_salida_fase(informacion_procesada_t* informacion_sistema, OSQPFloat*l) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  int offset_potencia_red_fase_R = 33 * numero_puntos_simulacion;
  int offset_potencia_red_fase_S = 34 * numero_puntos_simulacion;
  int offset_potencia_red_fase_T = 35 * numero_puntos_simulacion;

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    l[offset_potencia_red_fase_R + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_potencia_red_fase_S + punto_simulacion] = (OSQPFloat)0.0;
    l[offset_potencia_red_fase_T + punto_simulacion] = (OSQPFloat)0.0;
  }

}