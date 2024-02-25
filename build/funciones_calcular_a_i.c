#include "definiciones_globales.h"
#include "funciones_calcular_a_x.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//Se utiliza el siguiente subprograma para indicar las filas en la que se encuentra la potencia intercambiada
//por un terminal en concreto

int incluir_filas_potencias_terminal(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* fila_actual,
  int* numero_ecuaciones_bateria, char fase, OSQPInt numero_terminal, int* index_actual) {

 
  //Cargo el numero de puntos de simulacion en la optimizacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;


  //Cargo el offset para las ecuaciones del comportamiento de la bateria
  int offset_bateria = NUMERO_VARIABLES * numero_puntos_simulacion + NUMERO_FASES * numero_puntos_simulacion;
  int fila_ecuacion_bateria = offset_bateria + (*numero_ecuaciones_bateria);

  //Cargo la fila inicial de la restriccion de borde
  int fila_actual_restriccion_borde = (numero_terminal - 1) * numero_puntos_simulacion;
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


  //Creo dos arrays para almacenar los puntos iniciales y finales en el que hay algún vehículo
  int* puntos_iniciales = NULL;
  int* puntos_finales = NULL;
  //Se crea una variable para indicar cuantos vehiculos o baterias se tienen en el terminal
  int numero_elementos_terminales = 0;
  int index_adicional = 0;


  if (cargar_puntos_iniciales_finales(informacion_sistema, numero_terminal, &numero_elementos_terminales,
    &puntos_iniciales, &puntos_finales) == ERROR) {
    printf("No se han podido cargar los puntos iniciales y finales de las baterías y vehículos en la elaboración del vector A_i\n");
    registrar_error("No se han podido cargar los puntos iniciales y finales de las baterías y vehículos en la elaboración del vector A_i\n", REGISTRO_ERRORES);
    return ERROR;
  }
  qsort(puntos_iniciales, numero_elementos_terminales, sizeof(int), comparar_ints);
  qsort(puntos_iniciales, numero_elementos_terminales, sizeof(int), comparar_ints);

  if (numero_elementos_terminales > 0) {
    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
      if (numero_elementos_terminales > index_adicional) {
        if (comprobar_rango(punto_actual, puntos_iniciales[index_adicional], puntos_finales[index_adicional]) == true) {
          //El primer punto que hay una bateria conectada ya sea de una bateria o de un vehiculo propiamente dicho
          if (punto_actual == puntos_iniciales[index_adicional]) {
            A_i[(*index_actual)] = fila_actual_restriccion_borde;
            (fila_actual_restriccion_borde)++;
            (*index_actual)++;
            A_i[(*index_actual)] = fila_ecuacion_balance;
            (fila_ecuacion_balance)++;
            (*index_actual)++;
            //Es necesario tambien actualizar el numero de ecuaciones que modelan el comportamiento de una bateria
            (*numero_ecuaciones_bateria)++;
          }
          else {
            A_i[(*index_actual)] = fila_actual_restriccion_borde;
            (fila_actual_restriccion_borde)++;
            (*index_actual)++;
            A_i[(*index_actual)] = fila_ecuacion_balance;
            (*index_actual)++;
            (fila_ecuacion_balance)++;
            A_i[(*index_actual)] = fila_ecuacion_bateria;
            (*index_actual)++;
            (fila_ecuacion_bateria)++;
            //Es necesario tambien actualizar el numero de ecuaciones que modelan el comportamiento de una bateria
            (*numero_ecuaciones_bateria)++;

          }
          //Se actualiza el index_adicional
          if (puntos_finales[index_adicional] == punto_actual) {
            index_adicional++;
          }
        }
      }
      else {
        //Si no hay que añadir más filas correspondientes a las ecuaciones que modelan el comportamiento de las
        //baterias.
        A_i[(*index_actual)] = fila_actual_restriccion_borde;
        (fila_actual_restriccion_borde)++;
        (*index_actual)++;
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
    int* index_actual, int* fila_actual) {
  //Variable booleana para saber cuando terminar el bucle while
  bool fin_bucle = false;
  //Variable tipo int para llevar la cuenta del terminal actual
  OSQPInt terminal_actual = 1;
  //variable char para saber en que fase está el terminal
  char fase;

  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Cargo la primera fila donde se encuentra un termino de potencia intercambiado por un terminal (restricciones de borde)
  *fila_actual = NUMERO_TERMINALES * (numero_puntos_simulacion);
  //Se carga el numero de ecuaciones de modelado de bateria que se han anyadido hasta el momento
  int numero_ecuaciones_modelado_bateria = 0;

  while (fin_bucle) {
    fase = informacion_sistema->informacion_terminales.fases_electricas[terminal_actual - 1];
    if (incluir_filas_potencias_terminal(informacion_sistema,A_i,fila_actual,&numero_ecuaciones_modelado_bateria,
                                         fase,terminal_actual,index_actual) == ERROR) {
      printf("No se han podido incluir las filas de las potencias intercambiadas por los terminales en el vector A_i\n");
      registrar_error("No se han podido incluir las filas de las potencias intercambiadas por los terminales en el vector A_i\n", REGISTRO_ERRORES);
      return ERROR;
    }

    if (terminal_actual >= 12) {
      fin_bucle = true;
    }

    else {
      terminal_actual += 1;
    }

  }
  return EXITO;
}





/*Se incluye las localizaciones en las que se encuentran los terminos de los estados de las baterías SOC*/
int incluir_filas_terminos_baterias_terminal(informacion_procesada_t* informacion_sistema,OSQPInt terminal_actual,
     int*index_actual,OSQPInt*A_i,int* fila_actual) {

  //Se crean dos arrays para almacenar los puntos iniciales y finales de los vehículos y baterías conectadas a los
  //terminales los puntos iniciales y finales.

  int* puntos_iniciales = NULL;
  int* puntos_finales = NULL;

  int numero_elementos_terminales = 0;
  //Variable para acceder al array de los puntos iniciales y finales
  int index_adicional = 0;
  //  Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int fila_termino_bateria_borde   = (*fila_actual);
  int fila_termino_bateria_balance = (NUMERO_TERMINALES * numero_puntos_simulacion) + *fila_actual;
  //Se crea una variable para indicar el numero de fila de ecuacion que se está rellenando en la matriz A_i
  

  if (cargar_puntos_iniciales_finales(informacion_sistema, terminal_actual, &numero_elementos_terminales,
    &puntos_iniciales, &puntos_finales) == ERROR) {
    printf("No se han podido cargar los puntos iniciales y finales de las baterias en la realización del vector A_i");
    registrar_error("No se han podido cargar los puntos iniciales y finales de las baterias en la realización del vector A_i", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se reordenan los arrays de puntos iniciales y finales de menor a mayor
  qsort(puntos_iniciales, numero_elementos_terminales, sizeof(int), comparar_ints);
  qsort(puntos_finales, numero_elementos_terminales, sizeof(int), comparar_ints);

  //Se pasa a escribir las filas en donde se encuentran los terminos SOC 
  if (numero_elementos_terminales > 0) {
    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
      if (comprobar_rango(punto_actual, puntos_iniciales[index_adicional], puntos_finales[index_adicional]) == true) {

        //Primer termino SOC de las ecuaciones del comportamiento de bateria (1ºpunto simulacion en el
        // que está presente la batería)

        if (punto_actual == puntos_iniciales[index_adicional]) {
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
        if (puntos_finales[index_adicional] == punto_actual) {
          index_adicional++;
        }
     }

      else {
        A_i[*index_actual] = fila_termino_bateria_borde;
        (*index_actual)++;
        (fila_termino_bateria_borde)++;
      }
    }
  }
  
  else {
    for (int i = 0; i < numero_puntos_simulacion; i++) {
      A_i[(*index_actual)] = (fila_termino_bateria_borde);
      (*index_actual)++;
      (fila_termino_bateria_borde)++;

    }
  }

  (*fila_actual) = fila_termino_bateria_borde;

  return EXITO;
}

/*Este subprograma se utiliza para indicar en que filas de la matriz A se encuentran los terminos del problema
  de optimizacion SOC */

int incluir_localizacion_baterias(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* index_actual,
                                  int* fila_actual) {

  //Se crea una variable booleana para controlar el bucle donde se añaden los terminos de la variable del estado
  //de bateria, y una variable de tipo int para controlar por que terminal se va escribiendo, y una variable
  //tipo int para ver por que punto de simulacion se va.
  bool fin_bucle = false;
  OSQPInt terminal_actual = 1;

  while (!fin_bucle) {
    if (incluir_filas_terminos_baterias_terminal(informacion_sistema, terminal_actual, index_actual, A_i,fila_actual) == ERROR) {
      printf("No se ha podido incluir los terminos de la batería en las ecuaciones de la matriz A_x\n");
      registrar_error("No se ha podido incluir los términos de la batería en las ecuaciones de la matriz A_x", REGISTRO_ERRORES);
      return ERROR;
    }
    if (terminal_actual >= 12) {
      fin_bucle = true;
    }
    else {
      terminal_actual += 1;
    }
  }
  return EXITO;
}

