
#include "definiciones_globales.h"
#include "funciones_calcular_a_x.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>

/*Este subprograma se utiliza para indicar las columnas en las que se encuentra el termino Pout grid p,t*/
void incluir_columnas_potencia_salida_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
  int* index_actual,int* comienzo_columna_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  for (int fase = 0; fase < NUMERO_FASES; fase++) {
    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
      (*comienzo_columna_actual) += 2;
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*index_actual)++;
      (*comienzo_columna_actual)++;
    }
  }
  //Hay que indicar la ultima columna cual es su ultimo elemento 
  A_p[(*index_actual)] = (*comienzo_columna_actual);
}

/*Este subprograma se utiliza para indicar las columnas en las que se encuentra el termino Pin grid p,t*/
void incluir_columnas_potencia_entrada_red_fase(informacion_procesada_t* informacion_sistema,OSQPInt* A_p,
     int* index_actual, int* comienzo_columna_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  for (int fase = 0; fase < NUMERO_FASES; fase++) {
    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
      (*comienzo_columna_actual) += 2;
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*index_actual)++;
      (*comienzo_columna_actual)++;
    }
  }
  
}


/*Este subprograma se utiliza para indicar las columnas en las que se encuentra el termino Pgridp,t*/
/*Hay 3 (1 por cada fase luego es necesario tener eso en cuenta*/

void incluir_columnas_potencias_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
  int* index_actual, int* comienzo_columna_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  for (int fase = 0; fase < NUMERO_FASES; fase++) {
    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion;punto_simulacion++) {
      (*comienzo_columna_actual) += 3;
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*index_actual)++;
      (*comienzo_columna_actual)++;
  }
}
}




/*Este subprograma se utiliza se utiliza para indicar las columnas en las que se encuentra la variable
 de la potencia red, basicamente se utiliza para indicar donde empiezan y terminas las columnas que identifican
 a la variable Pgrid,t*/

void incluir_columnas_potencia_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,int* index_actual,
                                   int* comienzo_columna_actual) {
  //cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Aparece en 3 ecuaciones distintas asi que por cada punto de simulacion hay 3 variables
  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    (*comienzo_columna_actual) += 2;
    A_p[(*index_actual)] = (*comienzo_columna_actual);
    (*index_actual)++;
    (*comienzo_columna_actual)++;
  }
}

//Este subprograma se utiliza para incluir las columnas del termino Pin grid,t
void incluir_columnas_potencia_entrada_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
  int* index_actual, int* comienzo_columna_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    (*comienzo_columna_actual) += 2;
    A_p[(*index_actual)] = (*comienzo_columna_actual);
    (*index_actual)++;
    (*comienzo_columna_actual)++;
  }
}

//Este subprograma se utiliza para incluir las columnas del termino Pout grid,t
void incluir_columnas_potencia_salida_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
     int* index_actual, int* comienzo_columna_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
    (*comienzo_columna_actual) += 2;
    A_p[(*index_actual)] = (*comienzo_columna_actual);
    (*index_actual)++;
    (*comienzo_columna_actual)++;
  }
}


/*Este subprograma se utiliza para indicar las posiciones de las columnas que representan los terminos de las potencias
  intercambiados por los terminales*/
int incluir_columnas_potencia_terminal(informacion_procesada_t* informacion_sistema, OSQPInt* A_p, int* index_actual,
  OSQPInt terminal_actual, int* comienzo_columna_actual, char fase) {

  //Cargo el numero de puntos de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Creo dos arrays para almacenar los puntos iniciales y finales de las baterias y vehiculos conectados a los
  //terminales
  int* puntos_iniciales = NULL;
  int* puntos_finales = NULL;

  //Se crea una variable para llevar la cuenta de a que puntos se accede del arrya de puntos iniciales y finales
  int index_adicional = 0;

  //Se crea una variable para almacenar cuantos elementos se tienen el terminal
  int numero_elementos_terminales = 0;

  //Si el terminal esta conectado a la fase R,S o T puede haber vehiculos conectados
  if ((fase == 'R') || (fase == 'S') || (fase == 'T')) {
    if (cargar_puntos_iniciales_finales(informacion_sistema, terminal_actual, &numero_elementos_terminales,
      &puntos_iniciales, &puntos_finales) == ERROR) {
      printf("No se han podido cargar los puntos iniciales y finales de las baterías y vehículos en la elaboración de la matriz A_p\n");
      registrar_error("No se han podido cargar los puntos iniciales y finales de las baterías y vehículos en la elaboración de la matriz A_p\n", REGISTRO_ERRORES);
      return ERROR;
    }
    qsort(puntos_iniciales, numero_elementos_terminales, sizeof(int), comparar_ints);
    qsort(puntos_iniciales, numero_elementos_terminales, sizeof(int), comparar_ints);

    if (numero_elementos_terminales > 0) {
      for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
        if (numero_elementos_terminales > index_adicional) {
          if (comprobar_rango(punto_actual, puntos_iniciales[index_adicional], puntos_finales[index_adicional]) == true) {
            //Si coincide con el primer punto de la simulacion en el cual la batería o vehículo está presente, solo
            //hay que añadir la ecuación del balance
            if (punto_actual == puntos_iniciales[index_adicional]) {
              (*comienzo_columna_actual)++;
              A_p[(*index_actual)] = (*comienzo_columna_actual);
              (*index_actual)++;
              (*comienzo_columna_actual)++;
            }

            else {
              (*comienzo_columna_actual) += 2;
              A_p[(*index_actual)] = (*comienzo_columna_actual);
              (*index_actual)++;
              (*comienzo_columna_actual)++;
            }
            //Se actualiza el index de los puntos adicionales si ya el vehiculo o bateria no estaba presene
            if (puntos_finales[index_adicional] == punto_actual) {
              index_adicional++;
            }
          }

          //Si no quedan mas ecuaciones del comportamiento del bateria, solo quedan añadir las columnas de las ecuaciones
          //del balance de la fase de la restriccion de borde
          else {
            (*comienzo_columna_actual)++;
            A_p[(*index_actual)] = (*comienzo_columna_actual);
            (*index_actual)++;
            (*comienzo_columna_actual)++;
          }
        }
        else {
          (*comienzo_columna_actual)++;
          A_p[(*index_actual)] = (*comienzo_columna_actual);
          (*index_actual)++;
          (*comienzo_columna_actual)++;
        }
      }
    }
  }
  //Si no está conectado a ninguna de las tres fases el termino no está presente ni en la ecuación del balance
  //de la fase, ni en la ecuación del modelado de la batería 
  else {
    for (int i = 0; i < numero_puntos_simulacion; i++) {
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*index_actual)++;
      (*comienzo_columna_actual)++;
    }
  }
  return EXITO;
}

/*Este subprograma se utiliza para incluir  las columnas en las que se encuentran los terminos de la potencia de los terminale*/
int incluir_columnas_potencias_terminales(informacion_procesada_t* informacion_sistema,OSQPInt* A_p,int* index_actual,
  int* comienzo_columna_actual) {
  //Variable booleana para saber cuando terminar el bucle while
  bool fin_bucle = false;
  //Variable tipo int para llevar la cuenta del terminal actual 
  OSQPInt terminal_actual = 1;
  //Variable para saber en que fase está el terminal
  char fase;

  while (!fin_bucle) {
    //Se carga la fase en la que está el terminal
    fase = informacion_sistema->informacion_terminales.fases_electricas[terminal_actual - 1];

    if (incluir_columnas_potencia_terminal(informacion_sistema, A_p, index_actual, terminal_actual, comienzo_columna_actual,fase) == ERROR) {
      printf("No se han podido incluir las columnas en las que se encuentran las columnas en donde se encuentran los terminos de las potencias de los terminales\n");
      registrar_error("No se han podido incluir las columnas en las que se encuentran las columnas en donde están las potencias de los terminales\n", REGISTRO_ERRORES);
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



/*Este subprograma se utiliza para indicar las posiciones de las columnas que representan los terminos estados de
  baterias en la matriz A el termino elemento_A_actual es el numero de termino de la matriz A que se ha identificado
  como ultimo valor de A en el que empezaba la ultima columna */

int incluir_columnas_bateria_terminal(informacion_procesada_t* informacion_sistema,OSQPInt* A_p, int* index_actual,
                                      OSQPInt terminal_actual, int* comienzo_columna_actual) {
  //Cargo el numero de puntos de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Creo dos arrays para almacenar los puntos iniciales y finales de las baterias y vehiculos conectados a los terminales
  int* puntos_iniciales = NULL;
  int* puntos_finales   = NULL;

  //Se crea una variable para llevar la cuenta 
  int index_adicional = 0;

  //Creo una variable para almacenar cuantos elementos se tienen en el terminal
  int numero_elementos_terminales = 0;


  //Se cargan los arrays de puntos iniciales y finales de los vehiculos y baterias conectados al terminal 
  if (cargar_puntos_iniciales_finales(informacion_sistema, terminal_actual, &numero_elementos_terminales,
    &puntos_iniciales, &puntos_finales) == ERROR) {
    printf("No se ha podido cargar los puntos iniciales y finales de vehiculos y baterías en la realización del vector A_p\n");
    registrar_error("No se han podido cargar los puntos iniciales y finales de los vehiculo y baterías en la realización del vector A_p\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se reordenan los arrays de los puntos iniciales y los finales de menor a mayor
  qsort(puntos_iniciales, numero_elementos_terminales, sizeof(int), comparar_ints);
  qsort(puntos_finales, numero_elementos_terminales, sizeof(int), comparar_ints);

  if (numero_elementos_terminales > 0) {
    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {
      if (numero_elementos_terminales > index_adicional) {
        //el primer punto de simulacion cuando la ecuacion es SOC = SOC inicial
        if (comprobar_rango(punto_actual, puntos_iniciales[index_adicional], puntos_finales[index_adicional]) == true) {
          (*comienzo_columna_actual) = (*comienzo_columna_actual) + 3;
          A_p[(*index_actual)] = (*comienzo_columna_actual);
          (*index_actual)++;
        }
        //si es el ultimo punto de simulacion en el que está presente la batería o vehículo 
        else if (punto_actual == puntos_finales[index_adicional]) {
          (*comienzo_columna_actual) = (*comienzo_columna_actual) + 2;
          A_p[(*index_actual)] = (*comienzo_columna_actual);
          (*index_actual)++;
        }
        else {
          A_p[(*index_actual)] = (*comienzo_columna_actual);
          (*comienzo_columna_actual)++;
          (*index_actual)++;
        }
      }
      else {
       
        A_p[(*index_actual)] = (*comienzo_columna_actual);
        (*comienzo_columna_actual)++;
        (*index_actual)++;
      }
    }
  }

  //Si solo hay restricciones de borde, cada elemento en la matriz A_x, corresponde a una nueva
  //columa, 
  else {
    for (int i = 0; i < numero_puntos_simulacion; i++) {
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*comienzo_columna_actual)++;
      (*index_actual)++;
    }
  }
}

int incluir_columnas_baterias(informacion_procesada_t* informacion_sistema, OSQPInt* A_p, int* index_actual,
  int* comienzo_columna_actual) {
  //Se crea una variable booleana para controlar el bucle donde se añaden las columnas en donde se encuentran los
  //términos de los estados de batería
  bool fin_bucle = false;
  OSQPInt terminal_actual = 1;
 

  while (!fin_bucle) {
    if (incluir_columnas_bateria_terminal(informacion_sistema, A_p, index_actual,terminal_actual,
                                          comienzo_columna_actual) == ERROR) {
      printf("No se ha podido incluir las columnas en las que se encuentran los terminos estados de bateria en el vector A_p\n");
      registrar_error("No se ha podido incluir las columnas en las que se encuentran los terminos estados de bateria en el vector A_p\n",REGISTRO_ERRORES);
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