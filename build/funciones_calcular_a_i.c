#include "definiciones_globales.h"
#include "funciones_calcular_a_x.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



void incluir_filas_potencia_salida_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_i,
  int* ultima_fila_balance_bateria, int* index_actual, char fase) {
  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int fila_restriccion_borde = 0;
  int fila_ecuacion_35 = 0;
  int fila_ecuacion_37 = 44 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  if (fase == 'R') {
    fila_restriccion_borde = 33 * numero_puntos_simulacion;
    fila_ecuacion_35 = 40 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  else if (fase == 'S') {
    fila_restriccion_borde = 34 * numero_puntos_simulacion;
    fila_ecuacion_35 = 41 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  else if (fase == 'T') {
    fila_restriccion_borde = 35 * numero_puntos_simulacion;
    fila_ecuacion_35 = 42 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  for (int i = 0; i < numero_puntos_simulacion; i++) {
    A_i[(*index_actual)] = fila_restriccion_borde;
    (*index_actual)++;
    (fila_restriccion_borde)++;
    A_i[(*index_actual)] = fila_ecuacion_35;
    (*index_actual)++;
    (fila_ecuacion_35)++;
    A_i[(*index_actual)] = fila_ecuacion_37;
    (*index_actual)++;
    (fila_ecuacion_37)++;
  }
}

//Este subprograma se utiliza para incluir la filas en las que se encuentra el término Pin grid p,t
void incluir_filas_potencia_entrada_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_i,
                                             int* ultima_fila_balance_bateria, int* index_actual, char fase) {
  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int fila_restriccion_borde ;
  int fila_ecuacion_35 ;
  int fila_ecuacion_36 = 43 * numero_puntos_simulacion +(*ultima_fila_balance_bateria);
  //Dependiendo de la fase el valor de fila a añadir en el vector A_i es diferente.
  if (fase == 'R') {
    fila_restriccion_borde = 30 * numero_puntos_simulacion;
    fila_ecuacion_35 = 40 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  else if (fase == 'S') {
    fila_restriccion_borde = 31 * numero_puntos_simulacion;
    fila_ecuacion_35 = 41 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  else if (fase == 'T') {
    fila_restriccion_borde = 32 * numero_puntos_simulacion;
    fila_ecuacion_35 = 42 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  for (int i = 0; i < numero_puntos_simulacion; i++) {
    A_i[(*index_actual)] = fila_restriccion_borde;
    (*index_actual)++;
    (fila_restriccion_borde)++;
    A_i[(*index_actual)] = fila_ecuacion_35;
    (*index_actual)++;
    (fila_ecuacion_35)++;
    A_i[(*index_actual)] = fila_ecuacion_36;
    (*index_actual)++;
    (fila_ecuacion_36)++;
  }
}



//Este subprograma se utiliza para incluir los terminos P grid,p,t, las filas depende de la fase que se estén
//añadiendo

void incluir_filas_potencia_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_i,
                                     int* ultima_fila_balance_bateria, int* index_actual, char fase) {
  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Defino una serie de variables para indicar las filas en donde se encuentran los términos de restricción
  int fila_restriccion_borde;
  int fila_restriccion_balance;
  int fila_ecuacion_35;
  int fila_ecuacion_38 = 45 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);

  if (fase == 'R') {
    fila_restriccion_borde = 27 * numero_puntos_simulacion;
    fila_restriccion_balance = NUMERO_VARIABLES * numero_puntos_simulacion;
    fila_ecuacion_35 = 40 * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  else if (fase == 'S') {
    fila_restriccion_borde = (27 +1) * numero_puntos_simulacion;
    fila_restriccion_balance = (NUMERO_VARIABLES + 1) * numero_puntos_simulacion;
    fila_ecuacion_35 = (40 + 1) * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  else if (fase == 'T') {
    fila_restriccion_borde = (27 + 2) * numero_puntos_simulacion;
    fila_restriccion_balance = (NUMERO_VARIABLES + 2) * numero_puntos_simulacion;
    fila_ecuacion_35 = (40 + 2) * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  }
  //Una vez situada las filas a añadir dependiendo de la fase se pasa a añadir las filas en el vector A_i
  for (int i = 0; i < numero_puntos_simulacion; i++) {
    A_i[(*index_actual)] = fila_restriccion_borde;
    (*index_actual)++;
    (fila_restriccion_borde)++;
    A_i[(*index_actual)] = fila_restriccion_balance;
    (*index_actual)++;
    (fila_restriccion_balance)++;
    A_i[(*index_actual)] = fila_ecuacion_35;
    (*index_actual)++;
    (fila_ecuacion_35)++;
    A_i[(*index_actual)] = fila_ecuacion_38;
    (*index_actual)++;
    (fila_ecuacion_38)++;
  }
}




//Se utiliza el siguiente siguiente subprograma para incluir las filas en donde se encuentra el termino Pgrid,out,t
void incluir_filas_potencia_salida_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_i,
  int* ultima_fila_balance_bateria, int* index_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Se carga la fila en donde se encuentra la restricion de borde
  int fila_restriccion_borde = (2 * NUMERO_TERMINALES + 2) * numero_puntos_simulacion;
  //Se carga la primera fila de la ecuacion 34
  int fila_balance_ec_34 = NUMERO_VARIABLES * numero_puntos_simulacion + NUMERO_FASES * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  //Se carga la primera fila de la ecuacion 37
  int fila_balance_ec_37 = fila_balance_ec_34 + 5 * numero_puntos_simulacion;

  for (int i = 0; i < numero_puntos_simulacion; i++) {

    A_i[(*index_actual)] = fila_restriccion_borde;
    (*index_actual)++;
    (fila_restriccion_borde)++;
    A_i[(*index_actual)] = fila_balance_ec_34;
    (*index_actual)++;
    (fila_balance_ec_34)++;
    A_i[(*index_actual)] = fila_balance_ec_37;
    (*index_actual)++;
    (fila_balance_ec_37)++;
  }
}


//Se utiliza el siguiente subprograma para incluir las filas en donde se encuentra el termino Pgrid,in,t
void incluir_filas_potencia_entrada_red(informacion_procesada_t* informacion_sistema,OSQPInt* A_i,
                                        int* ultima_fila_balance_bateria,int*index_actual) {
  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Se carga la fila en donde se encuentra la restriccion de borde
  int fila_restriccion_borde = (2 * NUMERO_TERMINALES + 1) * numero_puntos_simulacion;
  //Se carga la primera fila de la ecuacion 34
  int fila_balance_ec_34 = NUMERO_VARIABLES * numero_puntos_simulacion + NUMERO_FASES * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  //Se carga la primera fila de la ecuacion 36
  int fila_balance_ec_36 = fila_balance_ec_34 + 4 * numero_puntos_simulacion;
  for (int i = 0; i < numero_puntos_simulacion; i++) {
    A_i[(*index_actual)] = fila_restriccion_borde;
    (*index_actual)++;
    (fila_restriccion_borde)++;
    A_i[(*index_actual)] = fila_balance_ec_34;
    (*index_actual)++;
    (fila_balance_ec_34)++;
    A_i[(*index_actual)] = fila_balance_ec_36;
    (*index_actual)++;
    (fila_balance_ec_36)++;
  }
}




//Se utiliza el siguiente subprograma para incluir las filas en donde se encuentran los terminos Pgrid,t
void incluir_filas_potencia_red(informacion_procesada_t* informacion_sistema,OSQPInt* A_i,
                               int* ultima_fila_balance_bateria,int*index_actual) {
  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Se carga la primera fila de la restricción de borde del término Pgrid,t
  int fila_restriccion_borde = (2 * NUMERO_TERMINALES) * numero_puntos_simulacion;
  //Se carga la primera fila del balance en la fase de red depende de la fase por supuesto
  int fila_balance_ec_34 = NUMERO_VARIABLES * numero_puntos_simulacion + NUMERO_FASES * numero_puntos_simulacion + (*ultima_fila_balance_bateria);
  //Sitúo la primera fila de la ecuación 38 
  int fila_balance_ec_38 = fila_balance_ec_34 + 6 * numero_puntos_simulacion;
  for (int i = 0; i < numero_puntos_simulacion; i++) {
    //Se indica la fila en la que está la restricción de borde Pgrid,t
    A_i[(*index_actual)] = fila_restriccion_borde;
    (fila_restriccion_borde)++;
    (*index_actual)++;
    A_i[(*index_actual)] = fila_balance_ec_34;
    (fila_balance_ec_34)++;
    (*index_actual)++;
    A_i[(*index_actual)] = fila_balance_ec_38;
    (fila_balance_ec_38)++;
    (*index_actual)++;
  }
}





//Se utiliza el siguiente subprograma para indicar las filas en la que se encuentra la potencia intercambiada
//por un terminal en concreto

int incluir_filas_potencias_terminal(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* fila_actual,
  int* numero_ecuaciones_bateria, char fase, OSQPInt numero_terminal, int* index_actual,
  informacion_carga_terminales_t* programacion_elementos_carga_terminales) {

 
  //Cargo el numero de puntos de simulacion en la optimizacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;


  //Cargo el offset para las ecuaciones del comportamiento de la bateria
  int offset_bateria = NUMERO_VARIABLES * numero_puntos_simulacion + NUMERO_FASES * numero_puntos_simulacion;

  //Se carga cual seria el valor de la fila de la primera ecuacion del balance de bateria teniendo en cuenta
  //todas las demas baterías que han sido consideradas
  int fila_ecuacion_bateria = offset_bateria + (*numero_ecuaciones_bateria);

  //Cargo la fila inicial de la restriccion de borde
  int fila_actual_restriccion_borde = (numero_terminal ) * numero_puntos_simulacion;
  int fila_ecuacion_balance = 0;

  if (fase == 'R') {
    fila_ecuacion_balance = NUMERO_VARIABLES * numero_puntos_simulacion;
  }
  else if (fase == 'S') {
    fila_ecuacion_balance = NUMERO_VARIABLES * numero_puntos_simulacion + numero_puntos_simulacion;
  }

  else if (fase == 'T') {
    fila_ecuacion_balance = NUMERO_VARIABLES * numero_puntos_simulacion + 2 * numero_puntos_simulacion;
  }


  //Se carga el numero de elementos que tienen su carga prgoramada en el terminal
  int numero_elementos_terminal = programacion_elementos_carga_terminales->informacion_carga_terminales->numero_elementos_terminal;
  int index_elemento_carga_terminal = 0;

  


  if ((numero_elementos_terminal > 0) && ((fase == 'R') || (fase == 'S') || (fase == 'T'))) {

    //Se carga el punto inicial y el final del primer vehiculo o batería que tiene su carga programada en el terminal
    int punto_inicial = programacion_elementos_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[0].punto_inicio;
    int punto_final = programacion_elementos_carga_terminales->informacion_carga_terminales[numero_terminal].elementos_terminal[0].punto_final;


    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {

      if (numero_elementos_terminal > index_elemento_carga_terminal) {

        //Se comprueba si para el punto actual en el que se están calculando las diferentes filas en las que hay
        //un termino que es la potencia intercambiada un terminal

        if (comprobar_rango(punto_actual, punto_inicial, punto_final) == true) {

          //El primer punto que hay una bateria conectada ya sea de una bateria o de un vehiculo propiamente dicho
          //no hay ecuacion de balance de bateria ya que la bateria = bateria inicial

          if (punto_actual == punto_inicial) {

            //Se incluye la fila de la restricción de borde
            A_i[(*index_actual)] = fila_actual_restriccion_borde;
            (fila_actual_restriccion_borde)++;
            (*index_actual)++;

            //Se incluye la fila de la ecuacion del balance bateria.
            A_i[(*index_actual)] = fila_ecuacion_balance;
            (fila_ecuacion_balance)++;
            (*index_actual)++;

            //Es necesario tambien actualizar el numero de ecuaciones que modelan el comportamiento de una bateria
            (*numero_ecuaciones_bateria)++;
          }
          else {

            //Se incluye la restricción de borde
            A_i[(*index_actual)] = fila_actual_restriccion_borde;
            (fila_actual_restriccion_borde)++;
            (*index_actual)++;

            //Se incluye la ecuación del balance de potencia
            A_i[(*index_actual)] = fila_ecuacion_balance;
            (*index_actual)++;
            (fila_ecuacion_balance)++;

            //Se incluye la ecuación del balance de batería
            A_i[(*index_actual)] = fila_ecuacion_bateria;
            (*index_actual)++;
            (fila_ecuacion_bateria)++;

            //Es necesario tambien actualizar el numero de ecuaciones que modelan el comportamiento de una bateria
            (*numero_ecuaciones_bateria)++;

          }

          //Se actualiza el index_adicional
          if (punto_final == punto_actual) {
            (*index_actual)++;
          }

        }
      }
      else {
        //Si no hay que añadir más filas correspondientes a las ecuaciones que modelan el comportamiento de las
        //baterias.
        A_i[(*index_actual)] = fila_actual_restriccion_borde;
        (fila_actual_restriccion_borde)++;
        (*index_actual)++;
        //Se añade la ecuacion del balance
        A_i[(*index_actual)] = fila_ecuacion_balance;
        (fila_ecuacion_balance)++;
        (*index_actual)++;
      }


    }

  }

  //Si no se tiene nada programado para conectar en el terminal rellenar las matrices es más simple


  else {

    for (int i = 0; i < numero_puntos_simulacion; i++) {
      A_i[(*index_actual)] = fila_actual_restriccion_borde;
      (*index_actual)++;
      (fila_actual_restriccion_borde)++;
      if ((fase == 'R')||(fase== 'S')||(fase == 'T')) {
        A_i[(*index_actual)] = fila_ecuacion_balance;
        (*index_actual)++;
        (fila_ecuacion_balance)++;
      }
    }
  }
}




// Se utiliza el siguiente subprograma para indicar las filas en las que se encuentran las potencias
int incluir_filas_potencias_terminales(informacion_procesada_t* informacion_sistema,OSQPInt* A_i,
    int* index_actual, int* fila_actual,informacion_carga_terminales_t* elementos_programados_terminales) {

  //Variable booleana para saber cuando terminar el bucle while
  bool fin_bucle = false;
 
  //variable char para saber en que fase está el terminal
  char fase;

  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Cargo la primera fila donde se encuentra un termino de potencia intercambiado por un terminal (restricciones de borde)
  *fila_actual = NUMERO_TERMINALES * (numero_puntos_simulacion);

  //Se carga el numero de ecuaciones de modelado de bateria que se han anyadido hasta el momento
  int numero_ecuaciones_modelado_bateria = 0;

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {

    fase = informacion_sistema->informacion_terminales.fases_electricas[numero_terminal];

    if (incluir_filas_potencias_terminal(informacion_sistema, A_i, fila_actual, &numero_ecuaciones_modelado_bateria,
      fase, numero_terminal, index_actual, elementos_programados_terminales) == ERROR) {
      printf("No se han podido incluir las filas de las potencias intercambiadas por los terminales en el vector A_i\n");
      registrar_error("No se han podido incluir las filas de las potencias intercambiadas por los terminales en el vector A_i\n", REGISTRO_ERRORES);
      return ERROR;
    }

  }

  return EXITO;
}





/*Se incluye las filas en las que se encuentran los terminos de los estados de las baterías SOC*/
int incluir_filas_terminos_baterias_terminal(informacion_procesada_t* informacion_sistema,OSQPInt terminal_actual,
     int*index_actual,OSQPInt*A_i,int* fila_actual_balance,informacion_carga_terminales_t* elementos_programados_terminales) {

  //Se carga el numero de elementos que se tienen en el terminal
  int numero_elementos_terminales = elementos_programados_terminales->informacion_carga_terminales->numero_elementos_terminal;

  //Variable que sirve para acceder a los diferentes elementos que tienen su carga programada en el terminal.
  int index_elemento_terminal = 0;

  //  Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Se carga las filas iniciales en las que se encuentra la informacion

  //Se define la primera fila en la que se encuentra la restriccion de borde de la batería de este terminal
  int fila_termino_bateria_borde = terminal_actual * numero_puntos_simulacion;

  //Se define la primera fila en la que se encuentra el balance de bateria 
  int fila_termino_bateria_balance = (*fila_actual_balance);


  //Se crea una variable para indicar el numero de fila de ecuacion que se está rellenando en la matriz A_i

  //Se crean dos variables para poder guardar el punto inicial y final del elemento que tienen su carga
  //programada en el terminal:
  int punto_inicial;
  int punto_final;


  //Se pasa a escribir las filas en donde se encuentran los terminos SOC 
  if (numero_elementos_terminales > 0) {

    punto_inicial = elementos_programados_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_terminal].punto_inicio;
    punto_final = elementos_programados_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_terminal].punto_final;

    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {

      if (numero_elementos_terminales > index_elemento_terminal) {
        if (comprobar_rango(punto_actual,punto_inicial,punto_final) == true) {

          //Primer termino SOC de las ecuaciones del comportamiento de bateria (1ºpunto simulacion en el
          // que está presente la batería)

          if (punto_actual == punto_inicial) {
            //Se escribe en el vector A_i la fila en la que está la batería SOC

            A_i[*index_actual] = fila_termino_bateria_borde;
            //Se incrementa el index del vector A_i y la siguiente fila de la batería SOC de la restricción de borde a añadir
            (fila_termino_bateria_borde)++;
            (*index_actual)++;
            //Se indica donde en que fila está el término SOC de la ecuacion del comportamiento de la batería
            A_i[*index_actual] = fila_termino_bateria_balance;
            (fila_termino_bateria_balance)++;
            (*index_actual)++;
          }
          else {

            //Se indica en que fila están los términos de SOC en un punto de simulacion que hay una batería
            //presente ya bien sea de un vehículo o una batería

            //Se añade el termino del estado de bateria del punto anterior 
            A_i[*index_actual] = fila_termino_bateria_balance;
            (*index_actual)++;
            //Se añade el termino de las restriccion de la bateria de borde
            A_i[*index_actual] = fila_termino_bateria_borde;
            (fila_termino_bateria_borde)++;
            (*index_actual)++;
            //Se añade el termino de la ecuacion del estado de bateria del punto actual
            A_i[*index_actual] = fila_termino_bateria_balance;
            (*index_actual)++;
            (fila_termino_bateria_balance)++;
          }
          //Si el punto actual coincide con el punto final en el siguiente punto de simulacion no es necesario
          //incluir más terminos.
          if (punto_final== punto_actual) {
            index_elemento_terminal++;
          //Se actualiza el index que sirve para acceder a los elementos que tienen su carga programada
            if (numero_elementos_terminales >index_elemento_terminal) {
              punto_inicial = elementos_programados_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_terminal].punto_inicio;
              punto_final = elementos_programados_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_terminal].punto_final;
            }
          }


        }

        //Si no hay elemento que tenga la carga programada en el terminal, cada termino SOC  está en una nueva fila
        else {
          A_i[*index_actual] = fila_termino_bateria_borde;
          (*index_actual)++;
          (fila_termino_bateria_borde)++;
        }
      }

      //Si no hay elemento que tenga la carga programada, simplemente hay que indicar donde están las restricciones
      //de borde
      else {
        A_i[*index_actual] = fila_termino_bateria_borde;
        (*index_actual)++;
        (fila_termino_bateria_borde)++;
      }
    }
  }
  //Si no hay elemento que tenga la carga programada, simplemente hay que indicar donde están las restricciones
   //de borde
  else {
    for (int i = 0; i < numero_puntos_simulacion; i++) {
      A_i[(*index_actual)] = (fila_termino_bateria_borde);
      (*index_actual)++;
      (fila_termino_bateria_borde)++;

    }
  }

  (*fila_actual_balance) = fila_termino_bateria_balance;

  return EXITO;
}

/*Este subprograma se utiliza para indicar en que filas de la matriz A se encuentran los terminos del problema
  de optimizacion SOC */

int incluir_filas_baterias(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* index_actual,
                                  int* fila_actual, informacion_carga_terminales_t* elementos_programados_terminal) {

  
  /*Se va iterando por todos los terminos SOC, es necesario iterar por cada terminal, para incluir las filas
    en las que se ecuentran estos terminos.*/

  for (int numero_terminal = 0; numero_terminal<NUMERO_TERMINALES; numero_terminal++) {

    if (incluir_filas_terminos_baterias_terminal(informacion_sistema,numero_terminal, index_actual, A_i,fila_actual,
      elementos_programados_terminal) == ERROR) {
      printf("No se ha podido incluir los terminos de la batería en las ecuaciones de la matriz A_x\n");
      registrar_error("No se ha podido incluir los términos de la batería en las ecuaciones de la matriz A_x", REGISTRO_ERRORES);
      return ERROR;
    }
    
    }
  
  return EXITO;
}

