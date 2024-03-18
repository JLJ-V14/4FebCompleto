
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
     
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*index_actual)++;
      (*comienzo_columna_actual) += 3;
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
      
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*index_actual)++;
      (*comienzo_columna_actual) += 3;
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
      
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*index_actual)++;
      (*comienzo_columna_actual) += 4;
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
    
    A_p[(*index_actual)] = (*comienzo_columna_actual);
    (*index_actual)++;
    (*comienzo_columna_actual) += 3;
  }

}

//Este subprograma se utiliza para incluir las columnas del termino Pin grid,t
void incluir_columnas_potencia_entrada_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
  int* index_actual, int* comienzo_columna_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
   
    A_p[(*index_actual)] = (*comienzo_columna_actual);
    (*index_actual)++;
    (*comienzo_columna_actual) += 3;
  }
}

//Este subprograma se utiliza para incluir las columnas del termino Pout grid,t
void incluir_columnas_potencia_salida_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
     int* index_actual, int* comienzo_columna_actual) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
   
    A_p[(*index_actual)] = (*comienzo_columna_actual);
    (*index_actual)++;
    (*comienzo_columna_actual) += 3;
  }
}


/*Este subprograma se utiliza para indicar las posiciones de las columnas que representan los terminos de las potencias
  intercambiados por los terminales*/
int incluir_columnas_potencia_terminal(informacion_procesada_t* informacion_sistema, OSQPInt* A_p, int* index_actual,
  int terminal_actual, int* comienzo_columna_actual, char fase,informacion_carga_terminales_t* programacion_elementos_carga_terminales) {

  //Cargo el numero de puntos de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  

  //Se crea una variable para llevar la cuenta de a que puntos se accede del arrya de puntos iniciales y finales
  int index_adicional = 0;

  //Se almacenan cuantos elementos tienen su carga programada en el terminal
  int numero_elementos_terminales = programacion_elementos_carga_terminales->informacion_carga_terminales[terminal_actual].numero_elementos_terminal;

  //Se definen variables para cargar los puntos iniiciales y finales que tienen su carga programada terminal
  int punto_inicial;
  int punto_final;

  //Si el terminal esta conectado a la fase R,S o T puede haber vehiculos conectados
  if ((fase == 'R') || (fase == 'S') || (fase == 'T')) {

    printf("El index actual es %d\n", *index_actual);
    printf("Prueba fase \n");
    if (numero_elementos_terminales > 0) {

      //Se carga el punto inicial del elemento de terminal actual
      punto_inicial = programacion_elementos_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_adicional].punto_inicio;
      punto_final = programacion_elementos_carga_terminales->informacion_carga_terminales[terminal_actual].elementos_terminal[index_adicional].punto_final;


      for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {

        //Si hay elementos que tienen su carga programada en el terminal, hay que tener en cuenta a la hora de añadir
        //los elementos en el vector A_p

        if (numero_elementos_terminales > index_adicional) {

          if (comprobar_rango(punto_actual,punto_inicial,punto_final) == true) {

            //Si coincide con el primer punto de la simulacion en el cual la batería o vehículo está presente, solo
            //hay que añadir la ecuación del balance.

            //Se incluye el termino que aparece en el balance de fase
            if (punto_actual == punto_inicial) {
              A_p[(*index_actual)] = (*comienzo_columna_actual);
              (*index_actual)++;
              (*comienzo_columna_actual) += 2;
              printf("El index actual1 es %d\n", *index_actual);
            }

            //Se tiene en cuenta que la potencia intercambiada por el terminal aparace en la ecuacion de
            //balance de fase, restricción de borde y balance de batería.
            else {
              A_p[(*index_actual)] = (*comienzo_columna_actual);
              (*index_actual)++;
              (*comienzo_columna_actual) +=3;
              printf("El index actual2 es %d\n", *index_actual);

              if (punto_final == punto_actual) {
                index_adicional++;
              }
            }
            //Se actualiza el index de los puntos adicionales si ya el vehiculo o bateria no estaba presene
           
          }

          //Si no quedan mas ecuaciones del comportamiento del bateria, solo quedan añadir las columnas de las ecuaciones
          //del balance de la fase y la restriccion de borde
          else {
            A_p[(*index_actual)] = (*comienzo_columna_actual);
            (*index_actual)++;
            (*comienzo_columna_actual) += 2;
            printf("El index actual3 es %d\n", *index_actual);
          }
        }

        //Si no quedan más vehículos o baterías que tengan su carga programada en el terminal, se actualiza el
        //vector A_p teniendo eso en cuenta.

        else {
          A_p[(*index_actual)] = (*comienzo_columna_actual);
          (*index_actual)++;
          (*comienzo_columna_actual) += 2;
          printf("El index actual4 es %d\n", *index_actual);
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
      printf("El index actual5 es %d\n", *index_actual);
    }
  }
  return EXITO;
}

/*Este subprograma se utiliza para incluir  las columnas en las que se encuentran los terminos de la potencia de los terminale*/
int incluir_columnas_potencias_terminales(informacion_procesada_t* informacion_sistema,OSQPInt* A_p,int* index_actual,
  int* comienzo_columna_actual, informacion_carga_terminales_t* programacion_elementos_carga_terminales) {

  //Variable para saber en que fase está el terminal
  char fase;

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++){

    //Se carga la fase en la que está el terminal
    fase = informacion_sistema->informacion_terminales.fases_electricas[numero_terminal];

    if (incluir_columnas_potencia_terminal(informacion_sistema, A_p, index_actual,numero_terminal, comienzo_columna_actual,fase,
        programacion_elementos_carga_terminales) == ERROR) {
      printf("No se han podido incluir las columnas en las que se encuentran las columnas en donde se encuentran los terminos de las potencias de los terminales\n");
      registrar_error("No se han podido incluir las columnas en las que se encuentran las columnas en donde están las potencias de los terminales\n", REGISTRO_ERRORES);
      return ERROR;
    }
  
  }
  return EXITO;
}



/*Este subprograma se utiliza para indicar las posiciones de las columnas que representan los terminos estados de
  baterias en la matriz A el termino elemento_A_actual es el numero de termino de la matriz A que se ha identificado
  como ultimo valor de A en el que empezaba la ultima columna */

int incluir_columnas_bateria_terminal(informacion_procesada_t* informacion_sistema,OSQPInt* A_p, int* index_actual,
  OSQPInt terminal_actual, int* comienzo_columna_actual, informacion_carga_terminales_t *programacion_elementos_carga_terminal) {

  //Cargo el numero de puntos de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se crea una variable para llevar la cuenta 
  int index_elemento_carga = 0;

  //Creo una variable para almacenar cuantos elementos se tienen en el terminal
  int numero_elementos_terminales = programacion_elementos_carga_terminal->informacion_carga_terminales[terminal_actual].numero_elementos_terminal;





  if (numero_elementos_terminales > 0) {
    printf("Entra aquí\n");

    //Se carga el punto inicial y el punto final del primer elemento a poner en la carga
    int punto_inicial = programacion_elementos_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga].punto_inicio;
    int punto_final   = programacion_elementos_carga_terminal->informacion_carga_terminales[terminal_actual].elementos_terminal[index_elemento_carga].punto_final;

    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {

      if (numero_elementos_terminales > index_elemento_carga) {

        //el primer punto de simulacion cuando la ecuacion es SOC = SOC inicial
        if (comprobar_rango(punto_actual, punto_inicial, punto_final) == true) {

          if (punto_actual != punto_final) {
            printf("Entra aquí\n");
            A_p[(*index_actual)] = (*comienzo_columna_actual);
            (*comienzo_columna_actual) = (*comienzo_columna_actual) + 3;
            (*index_actual)++;
          }
          //si es el ultimo punto de simulacion en el que está presente la batería o vehículo 
        else {
          A_p[(*index_actual)] = (*comienzo_columna_actual);
          (*comienzo_columna_actual) = (*comienzo_columna_actual) + 2;
          (*index_actual)++;
          index_elemento_carga++;

          }
        }

        else {
          A_p[(*index_actual)] = (*comienzo_columna_actual);
          (*comienzo_columna_actual) = (*comienzo_columna_actual) + 2;
          (*index_actual)++;
        }
      }
      else {
       
        A_p[(*index_actual)] = (*comienzo_columna_actual);
        *comienzo_columna_actual = (*comienzo_columna_actual) + 2;
        (*index_actual)++;
      }
    }
  }

  //Si solo hay restricciones de borde, cada elemento en la matriz A_x, corresponde a una nueva
  //columa, 
  else {
    printf("Entru\n");
    for (int i = 0; i < numero_puntos_simulacion; i++) {
      A_p[(*index_actual)] = (*comienzo_columna_actual);
      (*comienzo_columna_actual) = (*comienzo_columna_actual) + 2;
      (*index_actual)++;
    }
  }
}

int incluir_columnas_baterias(informacion_procesada_t* informacion_sistema, OSQPInt* A_p, int* index_actual,
  int* comienzo_columna_actual,informacion_carga_terminales_t* programacion_elementos_carga_terminal) {
  //Se crea una variable booleana para controlar el bucle donde se añaden las columnas en donde se encuentran los
  //términos de los estados de batería
 

  //Se incluye el primer valor del vector A_P
  A_p[(*index_actual)] = 0;

  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++){

    if (incluir_columnas_bateria_terminal(informacion_sistema, A_p, index_actual,numero_terminal,
        comienzo_columna_actual,programacion_elementos_carga_terminal) == ERROR) {
      printf("No se ha podido incluir las columnas en las que se encuentran los terminos estados de bateria en el vector A_p\n");
      registrar_error("No se ha podido incluir las columnas en las que se encuentran los terminos estados de bateria en el vector A_p\n",REGISTRO_ERRORES);
      return ERROR;
    }

  }
  return EXITO;
}