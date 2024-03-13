
#include "definiciones_globales.h"
#include "funciones_calcular_a_x.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/*Este subprograma se utiliza para rellenar un array, con las potencias maximas que puede intercambiar cada terminal
  dependiendo de las especificaciones de las baterias y vehiculos conectadas al mismo */

void calcular_limite_restricciones_potencia_elementos(informacion_procesada_t* informacion_sistema,int numero_terminal,
  informacion_carga_terminales_t* elementos_carga_terminales) {

 //Se carga el numero de elementos que tienen programada la carga en los terminales
  int numero_elementos_programados_terminal = elementos_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;


  //Se carga el valor máximo que puede intercambiar cada terminal
  OSQPFloat maxima_potencia_terminal = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_estaciones_carga[numero_terminal];


  //Se itera por todos los elementos que tienen programada su carga en el terminal 
  for (int elemento_terminal = 0; elemento_terminal < numero_elementos_programados_terminal; elemento_terminal++) {

    //Cargo la potencia maxima que puede intercambiar el vehículo o batería que tiene su carga programada
    OSQPFloat potencia_maxima_elemento = elementos_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal].potencia_maxima;

    if (potencia_maxima_elemento> maxima_potencia_terminal) {
      elementos_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[elemento_terminal].potencia_maxima = maxima_potencia_terminal;
    }

  }
}


/*Este subprograma se utiliza para escribir en el vector l los limites inferiores de los valores que pueden tomar
  los estados de la bateria */
void calcular_limite_superior_restricciones_bateria(informacion_procesada_t* informacion_sistema, OSQPFloat* u) {
  //Cargo el numero de puntos de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Creo una variable para situar los terminos en las posiciones correctas del vector
  int offset_index_vector_u;
  //Como hay 12 terminales se itera por cada uno de ellos
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    offset_index_vector_u = numero_terminal * numero_puntos_simulacion;
    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
      u[offset_index_vector_u + punto_simulacion] = (OSQPFloat)1.0;
    }
  }
}

/*Este subprograma se utiliza para incluir los límites superiores de las potencias que pueden tener los terminales
  si hay una batería o un vehículo presente*/

void calcular_limite_superior_potencia_terminal(informacion_procesada_t* informacion_sistema, OSQPFloat*u,
  int numero_terminal,informacion_carga_terminales_t *elementos_programados_carga_terminales) {

  //Se crean tres arrays, el primero que sirve para guardar los puntos iniciales a partir de las cuales hay un
  //vehiculo o bateria, presente, el segundo sirve para guardar los puntos finales a partir de los cuales no hay
  //una bateria o un vehiculo, el array potencias maximas sirve para almacenar las potencias maximas que pueden
  //intercambiar los terminales.


  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Cargo la primera posicion a escribir en el vector u
  int offset_potencia_terminal = NUMERO_TERMINALES * numero_puntos_simulacion + (numero_terminal) * numero_puntos_simulacion;

  //Cargo una variable para poder acceder al array que almacena los puntos iniciales
  int index_elemento_carga = 0;


  //Se carga el numero de elementos que tienen su carga programda en el terminal
  int numero_elementos_terminal = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].numero_elementos_terminal;

  //Se definen variables para cargar el punto inicial y el punto final de los vehiculos y baterias que tienen su
  //carga programada en el terminal
  int punto_inicial;
  int punto_final;


  //Se calcula un array que analiza cuales son las potencias máximas que pueden intercambiar los diferentes
  //vehículos o baterías que tienen su carga programada en el terminal
  calcular_limite_restricciones_potencia_elementos(informacion_sistema, numero_terminal,
  elementos_programados_carga_terminales);

  

  //Se escribe en el vector u los limites superiores de potencia que pueden intercambiar los terminales

  if (index_elemento_carga < numero_elementos_terminal) {

    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {

      
      //Se carga el punto inicial y el punto final del elemento que tiene programada su carga ya sea vehículo o
      //batería

      punto_inicial = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_carga].punto_inicio;
      punto_final = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_carga].punto_final;

      if (index_elemento_carga < numero_elementos_terminal) {
        if (comprobar_rango(punto_actual,punto_inicial,punto_final) == true) {
          if (punto_actual == punto_inicial) {
            u[offset_potencia_terminal + punto_actual] = 0.0;
          }
          else {
            u[offset_potencia_terminal + punto_actual] = elementos_programados_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[index_elemento_carga].potencia_maxima;

            //Se actualiza el index adicional si es necesario
            if (punto_final == punto_actual) {
              index_elemento_carga++;
            }

          }
        }

        else {
          u[offset_potencia_terminal + punto_actual] = 0.0;
        }
      }

      //Si no hay ningun vehiculo o bateria presente en los terminales el limite superior es 0
      else {
        u[offset_potencia_terminal + punto_actual] = 0.0;
      }
    }
  }

  //Se procede a liberar memoria reservada para los arrays para gestionar la informacion
  else {
    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
      u[offset_potencia_terminal + punto_actual] = 0.0;
    }
  }
 
}

/*Este subprograma se utiliza para calcular los limites de potencia que pueden intercambian los terminales.*/
void calcular_limites_superior_potencia_terminales(informacion_procesada_t* informacion_sistema, OSQPFloat*u,
  informacion_carga_terminales_t* elementos_programados_carga_terminales) {

  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se carga el offset a partir del cual situar los términos
  int offset = NUMERO_VARIABLES * numero_puntos_simulacion;

  for (int terminal = 0; terminal < NUMERO_TERMINALES; terminal++) {
    offset = offset + terminal * numero_puntos_simulacion;
    calcular_limite_superior_potencia_terminal(informacion_sistema, u, terminal, elementos_programados_carga_terminales);
  }
}

void calcular_restricciones_superiores_borde_potencias_red(informacion_procesada_t* informacion_sistema,OSQPFloat*u) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int offset_potencia_red = (NUMERO_TERMINALES * 2) * numero_puntos_simulacion;
  int offset_potencia_entrada_red = offset_potencia_red + numero_puntos_simulacion;
  int offset_potencia_salida_red  = offset_potencia_red + numero_puntos_simulacion;

  //Cargo los limites máximos de potencia que puede intercambiar la red con el sistema
  OSQPFloat potencia_maxima_red = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red;
  OSQPFloat potencia_maxima_entrada_red = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red;
  OSQPFloat potencia_maxima_salida_red = informacion_sistema->informacion_restricciones_sistema.potencia_minima_red;

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    u[offset_potencia_red + punto_simulacion] = potencia_maxima_red;
    u[offset_potencia_entrada_red + punto_simulacion] = potencia_maxima_entrada_red;
    u[offset_potencia_salida_red + punto_simulacion]  = potencia_maxima_salida_red;
  }

}

void calcular_restricciones_superiores_potencia_red_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* u) {

  //Se carga el numero de puntos simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int offset_potencia_red_R = 27 * numero_puntos_simulacion;
  int offset_potencia_red_S = 28 * numero_puntos_simulacion;
  int offset_potencia_red_T = 29 * numero_puntos_simulacion;

  //Se carga los limites máximos de potencia que puede intercambiar la red con el sistema
  OSQPFloat potencia_maxima_red_R = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_R;
  OSQPFloat potencia_maxima_red_S = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_S;
  OSQPFloat potencia_maxima_red_T = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_T;

  for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
    u[offset_potencia_red_R + punto_actual] = potencia_maxima_red_R;
    u[offset_potencia_red_S + punto_actual] = potencia_maxima_red_S;
    u[offset_potencia_red_T + punto_actual] = potencia_maxima_red_T;
  }
}

/*Se utiliza el siguiente subprograma para calcular los terminos de restricción de borde de las potencias
  entradas a las diferentes fases*/
void calcular_restricciones_superiores_potencia_entrada_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* u) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int offset_potencia_entrada_red_R = 30 * numero_puntos_simulacion;
  int offset_potencia_entrada_red_S = 31 * numero_puntos_simulacion;
  int offset_potencia_entrada_red_T = 32 * numero_puntos_simulacion;

  OSQPFloat potencia_maxima_red_R = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_R;
  OSQPFloat potencia_maxima_red_S = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_S;
  OSQPFloat potencia_maxima_red_T = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_T;

  for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
    u[offset_potencia_entrada_red_R + punto_actual] = potencia_maxima_red_R;
    u[offset_potencia_entrada_red_S + punto_actual] = potencia_maxima_red_S;
    u[offset_potencia_entrada_red_T + punto_actual] = potencia_maxima_red_T;

   
  }
}

void calcular_restricciones_borde_superiores_potencia_salida_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* u) {

  // Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int offset_potencia_salida_red_fase_R = 33 * numero_puntos_simulacion;
  int offset_potencia_salida_red_fase_S = 34 * numero_puntos_simulacion;
  int offset_potencia_salida_red_fase_T = 35 * numero_puntos_simulacion;
  
  //Se carga los valores máximos que pueden tomar estas variable
  OSQPFloat potencia_maxima_red_R = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_R;
  OSQPFloat potencia_maxima_red_S = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_S;
  OSQPFloat potencia_maxima_red_T = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_red_T;

  for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
      u[offset_potencia_salida_red_fase_R + punto_actual] = potencia_maxima_red_R;
      u[offset_potencia_salida_red_fase_S + punto_actual] = potencia_maxima_red_S;
      u[offset_potencia_salida_red_fase_T + punto_actual] = potencia_maxima_red_T;
  }

}