
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

int calcular_limite_restricciones_potencia_elementos(informacion_procesada_t* informacion_sistema, OSQPFloat** potencias_maximas,
  OSQPInt numero_terminal,int* puntos_iniciales,int* numero_elementos_terminales) {

  //Se reserva memoria para el array que almacena las potencias maximas que pueden intercambiar los terminales
  //teniendo en cuenta si hay un vehiculo o bateria presente en el terminal

  *potencias_maximas = (OSQPFloat*)malloc(*numero_elementos_terminales * sizeof(OSQPFloat));

  if (*potencias_maximas == NULL) {
    printf("No se ha podido reservar memoria para el array que almacena las potencias máximas que pueden intercambiar los terminales\n");
    registrar_error("No se han podido reservar memoria para el array que almacena las potencias máximas que pueden intercambiar los terminales\n", REGISTRO_ERRORES);
    return ERROR;
  }

  int index_potencias_anyadidas = 0;
  //Cargo el numero de vehiculos que se tienen en el sistema
  int numero_vehiculos = informacion_sistema->informacion_vehiculos.numero_vehiculos;
  //Cargo el numero de baterias  que se tienen en el sistema
  int numero_baterias = informacion_sistema->informacion_baterias.numero_baterias;

  //Cargo la potencia maxima que puede tener el terminal

  int potencia_maxima_terminal = informacion_sistema->informacion_restricciones_sistema.potencia_maxima_estaciones_carga[numero_terminal - 1];

  while (index_potencias_anyadidas < *numero_elementos_terminales) {
    //Variables para llevar la cuenta de que vehiculo o bateria se está examinando.
    int vehiculo_actual = 0;
    int bateria_actual  = 0;
    //Se crean variables booleanas para saber cuando añadir los vehiculos y baterias
    bool vehiculo_anyadido = false;
    bool bateria_anyadida = false;

    while ((vehiculo_actual < numero_vehiculos) ||(vehiculo_anyadido == false)) {

      //Se comprueba si el vehiculo que se esta comprobando esta conectado al mismo terminal que a el terminal
      //que se esta fijando el limite superior de la potencia del terminal.
      if (numero_terminal == informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].numero_terminal) {
          if (puntos_iniciales[index_potencias_anyadidas] == informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].numero_terminal) {

            //Si el punto inicial y el terminal coinciden con el vehiculos que se está inspeccionando se procede a añadir
            //la potencia maxima en el array.

            if (potencia_maxima_terminal < informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].potencia_maxima) {
              (*potencias_maximas)[index_potencias_anyadidas] = (OSQPFloat)potencia_maxima_terminal;
              vehiculo_anyadido = true;
              index_potencias_anyadidas++;
            }
            else {
              (*potencias_maximas)[index_potencias_anyadidas] = (OSQPFloat)informacion_sistema->informacion_vehiculos.vehiculos[vehiculo_actual].potencia_maxima;
              vehiculo_anyadido = true;
              index_potencias_anyadidas++;
            }
          }
        }
      vehiculo_actual++;
      }
    
    while ((bateria_actual<numero_baterias) || (bateria_anyadida == false)) {
      if (numero_terminal == informacion_sistema->informacion_baterias.baterias[bateria_actual].numero_terminal) {
        if (puntos_iniciales[index_potencias_anyadidas] == informacion_sistema->informacion_baterias.baterias[bateria_actual].numero_terminal) {

          //Si la potencia maxima que puede intercambiar el terminal es inferior a lo que puede aceptar la bateria ese
          //es el limite
          if (potencia_maxima_terminal < informacion_sistema->informacion_baterias.baterias[bateria_actual].maxima_potencia) {
            (*potencias_maximas)[index_potencias_anyadidas] = (OSQPFloat)potencia_maxima_terminal;
            bateria_anyadida = true;
            index_potencias_anyadidas++;
          }
          else {
            (*potencias_maximas)[index_potencias_anyadidas] = (OSQPFloat)potencia_maxima_terminal;
            bateria_anyadida = true;
            index_potencias_anyadidas++;
          }
        }
      }
      bateria_actual++;
    }

  }
  return EXITO;
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

int calcular_limite_superior_potencia_terminal(informacion_procesada_t* informacion_sistema, OSQPFloat*u,
                                               OSQPInt numero_terminal) {

  //Se crean tres arrays, el primero que sirve para guardar los puntos iniciales a partir de las cuales hay un
  //vehiculo o bateria, presente, el segundo sirve para guardar los puntos finales a partir de los cuales no hay
  //una bateria o un vehiculo, el array potencias maximas sirve para almacenar las potencias maximas que pueden
  //intercambiar los terminales.


  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Cargo la primera posicion a escribir en el vector u
  int offset_potencia_terminal = NUMERO_VARIABLES * numero_puntos_simulacion + (numero_terminal - 1) * numero_puntos_simulacion;

  //Cargo una variable para poder acceder al array que almacena los puntos iniciales
  int index_adicional = 0;

  int* puntos_iniciales = NULL;
  int* puntos_finales   = NULL;
  OSQPFloat* potencias_maximas = NULL;
  int numero_elementos_terminal = 0;

  if (cargar_puntos_iniciales_finales(informacion_sistema, numero_terminal, &numero_elementos_terminal, &puntos_iniciales,
    &puntos_finales) == ERROR) {
    printf("No se ha podido cargar los puntos iniciales y finales de baterias y vehiculos en la elaboracion del vector u\n");
    registrar_error("No se ha podido cargar los puntos iniciales y finales de bateria y vehiculos en la elaboracion del vector u\n", REGISTRO_ERRORES);
    return ERROR;
  }
  qsort(puntos_iniciales, numero_elementos_terminal, sizeof(int), comparar_ints);
  qsort(puntos_finales, numero_elementos_terminal, sizeof(int), comparar_ints);

  //Se calcula un array que almacena las potencias maximas que pueden intercambiar los terminales en los rangos de
  //los puntos iniciales a los puntos finales que hay en los arrays.

  if (calcular_limite_restricciones_potencia_elementos(informacion_sistema,&potencias_maximas,numero_terminal,
      puntos_iniciales,&numero_elementos_terminal) == ERROR) {
    printf("No se ha podido cargar las potencias maximas de los terminales adecuadamente error reservando memoria\n");
    registrar_error("No se ha podido cargar las potencias maximas de los terminales adecuadamente error reservando memoria\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se escribe en el vector u los limites superiores de potencia que pueden intercambiar los terminales 
  for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual) {
    if (index_adicional < numero_elementos_terminal) {

      if (comprobar_rango(punto_actual,puntos_iniciales[index_adicional], puntos_finales[index_adicional]) == true) {
        if (punto_actual == puntos_iniciales[index_adicional]) {
          u[offset_potencia_terminal + punto_actual] = 0.0;
        }
        else {
          u[offset_potencia_terminal + punto_actual] = (OSQPFloat)potencias_maximas[index_adicional];

          //Se actualiza el index adicional si es necesario
          if (puntos_finales[index_adicional] == punto_actual) {
            index_adicional++;
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

  //Se procede a liberar memoria reservada para los arrays para gestionar la informacion
  if (puntos_iniciales)free(puntos_iniciales);
  if (puntos_finales)free(puntos_finales);
  if (potencias_maximas)free(potencias_maximas);
  return EXITO;
}

/*Este subprograma se utiliza para calcular los limites de potencia que pueden intercambian los terminales.*/
int calcular_limites_superior_potencia_terminales(informacion_procesada_t* informacion_sistema, OSQPFloat*u) {

  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se carga el offset a partir del cual situar los términos
  int offset = NUMERO_VARIABLES * numero_puntos_simulacion;

  for (int terminal = 0; terminal < NUMERO_TERMINALES; terminal++) {
    offset = offset + terminal * numero_puntos_simulacion;
    if (calcular_limite_superior_potencia_terminal(informacion_sistema, u, terminal + 1) == ERROR) {
      printf("No se ha podido cargar los limites superiores de potencia que pueden intercambiar los terminales\n");
      registrar_error("No se ha podido cargar los limites superiores de potencia que pueden intercambiar los terminales\n", REGISTRO_ERRORES);
      return ERROR;
    }
  }
  return EXITO;
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