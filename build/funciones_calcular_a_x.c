#include "definiciones_globales.h"
#include "mostrar_informacion_terminales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



//Este subprograma se utiliza para actualizar la posicion a comprobar de baterias presentes en el terminal
int  actualizar_index_elemento_carga(int* index_elemento_carga,int punto_final_actual,int punto_actual) {
  if (punto_final_actual < punto_actual){
    (*index_elemento_carga) ++;
  }
}

//Este subprograma se utiliza para comprobar que se sigue teniendo una bateria presente en el terminal a la
//hora de construir la ecuacion en la matriz A para los puntos de simulacion en los que se esta presente la
//simulacion

 bool comprobar_rango(int punto_actual, int punto_inicial, int punto_final) {

  return ((punto_actual >= punto_inicial) && (punto_actual <= punto_final));
}



//Este subprograma se utiliza para comparar variables tipo ints y reordenar los array
 int comparar_ints(const void *a, const void *b) {
  return (*(int*)a - *(int*)b);
}

//Este subprograma se utiliza para crear un array con las capacidades de baterias
int crear_array_baterias(informacion_procesada_t* informacion_sistema, OSQPFloat** capacidades_baterias,
  OSQPInt* puntos_iniciales, OSQPInt terminal_actual, int numero_elementos_terminal) {

  int index_actual_puntos_iniciales = 0;
  //Se carga el numero de vehiculos
  int numero_vehiculos = informacion_sistema->informacion_vehiculos.numero_vehiculos;
  int numero_baterias = informacion_sistema->informacion_vehiculos.numero_vehiculos;

  *capacidades_baterias = malloc(numero_elementos_terminal * sizeof(OSQPFloat));
  if (*capacidades_baterias == NULL) {
    printf("Error añadiendo las capacidades de las baterias en las ecuaciones del comportamiento de la bateria\n");
    registrar_error("Error añadiendo las capacidades de las baterias en las ecuaciones del comportamiento de la bateria\n", REGISTRO_ERRORES);
    return ERROR;
  }

  for (int numero_elemento = 0; numero_elemento < numero_elementos_terminal; numero_elemento++) {
    //Se carga una serie de variables para poder obtener la capacidad de la bateria que corresponde a anyadir

    bool bateria_anyadida = false;
    int  index_vehiculos = 0;
    int  index_baterias = 0;
    int  punto_inicial = puntos_iniciales[index_actual_puntos_iniciales];
    int  index_capacidades = 0;

    //Se itera por todas las baterias que existen en el sistema (ya sean de vehiculos o no) y se añade
    //al array de capacidades de bateria si el punto inicial coinicide con el array de puntos iniciales
    while (!bateria_anyadida) {
      bateria_anyadida = false;
      index_vehiculos = 0;
      index_baterias  = 0;

      while ((!bateria_anyadida) && (index_vehiculos < numero_vehiculos)) {
        
        if (informacion_sistema->informacion_vehiculos.vehiculos[index_vehiculos].punto_inicio == punto_inicial) {
          (*capacidades_baterias)[index_baterias] = informacion_sistema->informacion_vehiculos.vehiculos[index_vehiculos].capacidad_bateria;
          bateria_anyadida = true;
          index_actual_puntos_iniciales += 1;
        }
        index_vehiculos++;
      }
      while((!bateria_anyadida) && (index_baterias < numero_baterias)){
        if (informacion_sistema->informacion_baterias.baterias[index_baterias].punto_inicio == punto_inicial) {
          (*capacidades_baterias)[index_baterias] = informacion_sistema->informacion_baterias.baterias[index_baterias].capacidad_bateria;
          bateria_anyadida = true;
          index_actual_puntos_iniciales += 1;
        }
        index_baterias++;
      }

    }
    //Se carga el numero de vehiculos

    //Se carga el numero de baterias
  }
  return EXITO;
}




/*Este subprograma se utiliza para incluir en el vector A_x, el vector de los elementos diferentes de 0
  los terminos SOC, (state of charge) estos terminos aparecen en las restricciones de borde, y las ecuaciones
  del balance de batería*/

int incluir_terminos_baterias_terminal_A_x(informacion_procesada_t* informacion_sistema,int terminal_actual,
    int*index_actual,OSQPFloat* A_x,informacion_carga_terminales_t* elementos_programados_carga_terminal) {

  //Se carga el numero de puntos de simulacion 
  int  numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;


  //Se carga el numero de elementos que se tienen en el terminal
  int numero_elementos_terminales = elementos_programados_carga_terminal->informacion_carga_terminales[terminal_actual].numero_elementos_terminal;

  //Se crea una variable para llevar la cuenta de que vehiculo o bateria tiene programada su carga en el terminal
  int index_elemento_terminal = 0;

  //Si hay vehiculos o baterias que tengan su carga programada en el terminal es necesario tener en cuenta que
  //ademas de las restricciones de borde, se tienen las ecuaciones de comportamiento de la batería

  if (numero_elementos_terminales > 0) {

    //Se itera por todos los puntos de simualacion.
    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {

      //Se carga el punto inicial del vehiculo o bateria cuyo termino SOC se está considerando
      int punto_inicial = elementos_programados_carga_terminal->informacion_carga_terminales->elementos_terminal[index_elemento_terminal].punto_inicio;

      //Cargo el punto final del vehiculo o bateria cuyo termino SOC, se está considerando
      int punto_final = elementos_programados_carga_terminal->informacion_carga_terminales->elementos_terminal[index_elemento_terminal].punto_final;

      //Se comprueba si es necesario actualizar el index del elemento que tiene su carga programada en el terminal 
      actualizar_index_elemento_carga(&index_elemento_terminal, punto_final, punto_actual);

      if (numero_elementos_terminales > index_elemento_terminal) {


        //Se pasa a comprobar si para el punto de simulacion, el vehiculo o bateria que se esta considerando
        //esta presente en el terminal, si lo está es necesario añadir los terminos correspondientes a la ecuacion
        //de balance de batería.

        if (comprobar_rango(punto_actual, punto_inicial, punto_final) == true) {

          //Se comprueba si el punto actual y el punto inicial coinciden.
          //Se añade la restricción de borde y el primer termino de la ecuacion del balance de bateria

          if (punto_actual == punto_inicial) {
            A_x[*index_actual] = 1;
            (*index_actual)++;
            A_x[*index_actual] = 1;
            (*index_actual)++;
          }

          //Si no se esta en el punto inicial se añaden la restricción de borde, el termino de la bateria
          //anterior y el actual, el termino inicial de la bateria es negativa.

          else {
            A_x[*index_actual] = -1;
            (*index_actual)++;
            A_x[*index_actual] = +1;
            (*index_actual)++;
            A_x[*index_actual] = +1;
            (*index_actual)++;
          }
        }
        else {
          A_x[*index_actual] = 1;
          (*index_actual)++;
        }
      }

      //Si no hay ningún vehículo o batería conectada al terminal, simplemente hay que considerar la restricción
      //de borde.
      else {
        A_x[*index_actual] = 1;
        (*index_actual)++;
      }
    }
  }

  //Si no se tienen vehiculos o baterías que tienen su carga programada en el terminal, igualmente se incluye
  //el termino SOC en la matriz ya que las restricciones de borde deben estar igualmente

  else {
    for (int i = 0; i < numero_puntos_simulacion; i++) {
      A_x[*index_actual] = 1;
      (*index_actual)++;
    }
  }
  return EXITO;
}




  //Este subprograma se utiliza para incluir en el vector A_x los terminos de los estados de las baterías
  //Los terminos se escriben por orden de columna, los primeros términos que pueden aparecer son los estados de
  //baterías, en las restricciones pueden aparecer en dos tipos de restricciones las de borde y las de ecuación
  //del comportamiento de la batería

int incluir_terminos_baterias_A_x(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x,int* index_actual,
  informacion_carga_terminales_t* programacion_carga_terminales) {

  //Se crea una variable booleana para controlar el bucle donde se añaden los terminos de la variable del estado
  //de bateria, y una variable de tipo int para controlar por que terminal se va escribiendo, y una variable
  //tipo int para ver por que punto de simulacion se va.

  
 for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++){

    if (incluir_terminos_baterias_terminal_A_x(informacion_sistema,numero_terminal, index_actual, A_x,
        programacion_carga_terminales) == ERROR) {
      printf("No se ha podido incluir los terminos de la batería en las ecuaciones de la matriz A_x\n");
      registrar_error("No se ha podido incluir los términos de la batería en las ecuaciones de la matriz A_x", REGISTRO_ERRORES);
      return ERROR;
    }
  
    
  }
  return EXITO;
}


/*Este subprograma se utiliza para incluir en el vector A_x los terminos de las potencias que intercambian
  los diferentes terminales */

int incluir_terminos_potencias_terminal_A_x(informacion_procesada_t* informacion_sistema, OSQPInt terminal_actual,
  int* index_actual, OSQPFloat* A_x, char fase_terminal, informacion_carga_terminales_t* elementos_programados_terminal) {

  //Este subprograma se utiliza para escribir en el vector A_x los términos correspondientes a las potencias
  //intercambiada por el terminal


  //Se carga el numero de elementos que se tiene en el terminal
  int numero_elementos_terminales = elementos_programados_terminal->informacion_carga_terminales[terminal_actual].numero_elementos_terminal;

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se carga el index_carga es decir al vehiculo o bateria que se esta accediendo de los diferentes elementos que
  //tienen su carga programada
  int index_adicional = 0;

  //Se define una variable para almacenar la capacidad de la bateria
  OSQPFloat capacidad_bateria;


  //Si hay algún vehículo o batería que tenga la carga programada en el terminal, es necesario considerar las
  //ecuaciones del balance de batería, ademas el terminal ha de estar conectado a fase R, S o T

  if ((numero_elementos_terminales > 0) && ((fase_terminal != 'N') && (fase_terminal != '0'))) {


    int punto_inicial = elementos_programados_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_adicional].punto_inicio;
    int punto_final = elementos_programados_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_adicional].punto_final;

    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
      if (numero_elementos_terminales > index_adicional) {

        //Se carga el punto inicial y final del siguiente vehiculo o bateria cuya carga está programada en el terminal
        //y que hay que considerar



          //Se comprueba si para el punto de simulacion que se esta calculando para el vector A_x hay algún vehículo o
          //batería que tenga la carga programada.

          if (comprobar_rango(punto_actual, punto_inicial, punto_final) == true) {


            if (punto_actual == punto_inicial) {

              //Se incluye la restricción del borde y el termino que aparece en la ecuacion del balance de potencia en la fase, ya que
              //está en fase R,S o T
              //Restricción de borde
              A_x[*index_actual] = 1;
              (*index_actual)++;
              //Término del balance de fase.
              A_x[*index_actual] = -1;
              (*index_actual)++;

              //Cargo la capacidad de la bateria
              capacidad_bateria = elementos_programados_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_adicional].capacidad_bateria;
            }
            else {

              //Se carga el delta de la simulacion
              OSQPInt   delta_simulacion = informacion_sistema->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta;
              //Restricción de borde
              A_x[*index_actual] = 1;
              (*index_actual)++;
              //Termino del balance de fase
              A_x[*index_actual] = -1;
              (*index_actual)++;
              //Termino del balance de batería
              A_x[*index_actual] = -delta_simulacion / (capacidad_bateria);
              (*index_actual)++;

            }

            //Se comprueba si el vehiculo o bateria sigue presente.
            if (punto_final== punto_actual) {
              index_adicional++;

              //Si hay más elementos que tienen su carga programada en el terminal es neccesario actualizar el punto
              //inicial y final
              if (numero_elementos_terminales > index_adicional) {
                punto_inicial = elementos_programados_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_adicional].punto_inicio;
                punto_final = elementos_programados_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_adicional].punto_final;
              }
              }
          }
      }

      //Si no queda ninguna bateria o vehículo que tenga su carga programada en el terminal 
      else {
        //Termino de la restricción de borde
        A_x[*index_actual] = 1;
        (*index_actual)++;
        //Termino del balance potencia en la fase.
        A_x[*index_actual] = -1;
        (*index_actual)++;
      }
    }
  }


  else {

    for (int i = 0; i < numero_puntos_simulacion; i++) {
      //Se incluye la restricción de borde
      A_x[*index_actual] = 1;
      (*index_actual)++;
      //Si el terminal está en fase Neutro o nada, no se incluye en el balance
      if ((fase_terminal != 'N') && (fase_terminal != '0')) {
        A_x[*index_actual] = -1;
        (*index_actual)++;
      }
    }
  }

 

  return EXITO;
}




// Este subprograma se utiliza para incluir los terminos de las potencias en las estaciones de carga y en la
// restricciones de borde en el vector A_x que es el vector que contiene los elementos diferentes de 0.

int incluir_terminos_potencias_A_x(informacion_procesada_t* informacion_sistema,OSQPFloat* A_x, int* index_actual,
  informacion_carga_terminales_t* elementos_programados_terminal) {

  for (OSQPInt terminal_actual = 0; terminal_actual < NUMERO_TERMINALES; terminal_actual++){

    char fase_terminal = informacion_sistema->informacion_terminales.fases_electricas[terminal_actual];

    if (incluir_terminos_potencias_terminal_A_x(informacion_sistema, terminal_actual, index_actual, A_x,fase_terminal,
      elementos_programados_terminal) == ERROR) {
      printf("No se han podido incluir los terminos de potencia  de los terminales en la matriz A_x\n");
      registrar_error("No se han podido incluir los terminos de potencia de los terminales en la matriz A_x\n", REGISTRO_ERRORES);
      return ERROR;
    }
  
  }
  return EXITO;
}


/*Este subprograma se utiliza para intercambiar el término de la variable de la potencia que intercambia la red
  con el sistema*/

void incluir_termino_potencia_red(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual) {
  //Primero se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  for (int i = 0; i < numero_puntos_simulacion; i++) {
    A_x[(*index_actual)] = 1;
    (*index_actual)++;
    A_x[(*index_actual)] = 1;
    (*index_actual)++;
    A_x[(*index_actual)] = 1;
    (*index_actual)++;
  }
}

/*Se utiliza este subprograma para incluir a la matriz A_x el término Pin grid */

void incluir_termino_potencia_entrada_sistema(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual) {
  //Se carga el numero de puntos de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  for (int i = 0; i < numero_puntos_simulacion; i++) {
    A_x[(*index_actual)] = 1;
    (*index_actual)++;
    A_x[(*index_actual)] = -1;
    (*index_actual)++;
    A_x[(*index_actual)] = 1;
    (*index_actual)++;
  }
}

// Este subprograma se utiliza para incluir en el vector A_x Pout grid
void incluir_termino_potencia_salida_sistema(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual) {
  //Se carga el numero de puntos de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  for (int i = 0; i < numero_puntos_simulacion; i++) {
    A_x[(*index_actual)] = 1;
    (*index_actual)++;
    A_x[(*index_actual)] = 1;
    (*index_actual)++;
    A_x[(*index_actual)] = 1;
    (*index_actual)++;
  }
}

//Este subprograma se utiliza para incluir los términos Pgrid,p,t
void incluir_terminos_potencia_red_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual) {
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  for (int i = 0; i < NUMERO_FASES; i++) {
    for (int j = 0; j < numero_puntos_simulacion; j++) {
      A_x[(*index_actual)] = 1;
      (*index_actual)++;
      A_x[(*index_actual)] = 1;
      (*index_actual)++;
      A_x[(*index_actual)] = 1;
      (*index_actual)++;
      A_x[(*index_actual)] = -1;
      (*index_actual)++;
    }
  }
}

/*Este subprograma se utiliza para incluir los términos Pingrid p,t */
void incluir_terminos_potencia_entrada_fase(informacion_procesada_t* informacion_sistema,OSQPFloat* A_x,int* index_actual) {
  //Se carga el número de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  for (int i = 0; i < NUMERO_FASES; i++) {
    for (int j = 0; j < numero_puntos_simulacion; j++) {
      A_x[(*index_actual)] = 1;
      (*index_actual)++;
      A_x[(*index_actual)] = -1;
      (*index_actual)++;
      A_x[(*index_actual)] = -1;
      (*index_actual)++;
    }
  }
}

/*Este subprograma se incluye los términos Pout grid p,t */
void incluir_terminos_potencia_salida_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual) {
 //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Se escriben los valores en la matriz A_x
  for (int i = 0; i < NUMERO_FASES; i++) {
    for (int j = 0; j < numero_puntos_simulacion; j++) {
      A_x[(*index_actual)] = 1;
      (*index_actual)++;
      A_x[(*index_actual)] = 1;
      (*index_actual)++;
      A_x[(*index_actual)] = -1;
      (*index_actual)++;
    }
  }
}
