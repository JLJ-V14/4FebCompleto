#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



//Este subprograma se utiliza para actualizar la posicion a comprobar de baterias presentes en el terminal
int  actualizar_index_adicional(int* index_adicional,int*puntos_finales,int punto_actual) {
  if (puntos_finales[*index_adicional] < punto_actual){
    (*index_adicional)++;
  }
}

//Este subprograma se utiliza para comprobar que se sigue teniendo una bateria presente en el terminal a la
//hora de construir la ecuacion en la matriz A para los puntos de simulacion en los que se esta presente la
//simulacion

 bool comprobar_rango(int punto_actual, int punto_inicial, int punto_final) {

  return ((punto_actual > punto_inicial) && (punto_actual <= punto_final));
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


//Este subprograma se utiliza para rellenar los puntos iniciales y finales de las baterias y vehiculos conectadas
//a los diferentes terminales

int cargar_puntos_iniciales_finales(informacion_procesada_t* informacion_sistema,OSQPInt terminal_actual,
                                    int *numero_elementos_terminales,int **puntos_iniciales,
                                    int** puntos_finales) {

  //Se van añadiendo los puntos iniciales y finales al array de puntos iniciales y finales de los
  //diferentes vehículos.
  for (int i = 0; i < informacion_sistema->informacion_vehiculos.numero_vehiculos; i++) {
    if (informacion_sistema->informacion_vehiculos.vehiculos[i].numero_terminal == terminal_actual) {
      (*numero_elementos_terminales)++;
      int* temp = realloc(puntos_iniciales, *numero_elementos_terminales * sizeof(int));
      int* temp2 = realloc(puntos_finales,  *numero_elementos_terminales * sizeof(int));

      if ((temp == NULL) || (temp2 == NULL)) {
        printf("No se ha podido reservar memoria para el array de los puntos iniciales o finales al crear la matriz A_x\n");
        registrar_error("No se No se ha podido reservar memoria para el array de los puntos iniciales o finales al crear la matriz A_x\n", REGISTRO_ERRORES);
        return ERROR;
      }

      *puntos_iniciales = temp;
      (*puntos_iniciales)[*numero_elementos_terminales] = informacion_sistema->informacion_vehiculos.vehiculos[i].punto_inicio;
      *puntos_finales   = temp2;
      (*puntos_finales)[*numero_elementos_terminales] = informacion_sistema->informacion_vehiculos.vehiculos[i].punto_final;
    }
  }

  //Se van a añadir los puntos iniciales y finales de las baterias al array de puntos iniciales y final
  //correspondiente a las baterías
  for (int i = 0; i < informacion_sistema->informacion_baterias.numero_baterias; i++) {
    if (informacion_sistema->informacion_baterias.baterias[i].numero_terminal == terminal_actual) {
      (*numero_elementos_terminales)++;
      int* temp = realloc(puntos_iniciales, *numero_elementos_terminales * sizeof(int));
      int* temp2 = realloc(puntos_finales, *numero_elementos_terminales * sizeof(int));

      if ((temp == NULL) || (temp2 == NULL)) {
        printf("No se ha podido reservar memoria para el array de los puntos iniciales o finales al crear la matriz A_x\n");
        registrar_error("No se No se ha podido reservar memoria para el array de los puntos iniciales o finales al crear la matriz A_x\n", REGISTRO_ERRORES);
        return ERROR;
      }

      *puntos_iniciales = temp;
      (*puntos_iniciales)[*numero_elementos_terminales] = informacion_sistema->informacion_baterias.baterias[i].punto_inicio;
      *puntos_finales = temp2;
      (*puntos_finales)[*numero_elementos_terminales] = informacion_sistema->informacion_baterias.baterias[i].punto_final;
    }
  }
  return EXITO;
}



int incluir_terminos_baterias_terminal(informacion_procesada_t* informacion_sistema,OSQPInt terminal_actual,
    int*index_actual,OSQPFloat* A_x,informacion_carga_terminales_t* elementos_programados_carga_terminal) {

  //Se procede a crear dos arrays un array que contiene los puntos iniciales y finales
  //de vehiculos y baterias conectadas a los terminales 
  //También se crea una variable que sirve para indicar el numero de vehiculos o baterias que se tienen conectados
  //a un determinado terminal.
  //Se carga tambien el numero de puntos de simulacion


  int* puntos_iniciales = NULL;
  int* puntos_finales   = NULL;
  int  numero_elementos_terminales = 0;
  int  numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int  index_adicional = 0;

  if (cargar_puntos_iniciales_finales(informacion_sistema, terminal_actual, &numero_elementos_terminales,
    &puntos_iniciales, &puntos_finales) == ERROR) {
    printf("No se ha podido cargar los puntos iniciales y finales de las baterias en la realizacion de la matriz a_x\n");
    registrar_error("No se ha podido cargar los puntos iniciales y finales de las baterias en la realizacion de la matriz a_x", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se reordenan los arrays de puntos iniciales y finales de menor a mayor
  qsort(puntos_iniciales, numero_elementos_terminales, sizeof(int), comparar_ints);
  qsort(puntos_finales, numero_elementos_terminales, sizeof(int), comparar_ints);

  if (numero_elementos_terminales > 0) {

    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {

      if (numero_elementos_terminales > index_adicional) {
        actualizar_index_adicional(&index_adicional, puntos_finales, punto_actual);

        if (comprobar_rango(punto_actual, puntos_iniciales[index_adicional], puntos_finales[index_adicional]) == true) {

          if (punto_actual == puntos_iniciales[index_adicional]) {
            A_x[*index_actual] = 1;
            (*index_actual)++;
            A_x[*index_actual] = 1;
            (*index_actual)++;
          }
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
      else {
        A_x[*index_actual] = 1;
        (*index_actual)++;
      }
    }
  }

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

int incluir_terminos_baterias(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x,int* index_actual,
  informacion_carga_terminales_t* programacion_carga_terminales) {

  //Se crea una variable booleana para controlar el bucle donde se añaden los terminos de la variable del estado
  //de bateria, y una variable de tipo int para controlar por que terminal se va escribiendo, y una variable
  //tipo int para ver por que punto de simulacion se va.

  bool fin_bucle = false;
  OSQPInt  terminal_actual = 1;
  
  while (!fin_bucle) {
    if (incluir_terminos_baterias_terminal(informacion_sistema, terminal_actual, index_actual, A_x,
        programacion_carga_terminales) == ERROR) {
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


int incluir_terminos_potencias_terminal(informacion_procesada_t* informacion_sistema, OSQPInt terminal_actual,
                                        int* index_actual, OSQPFloat* A_x, char fase_terminal) {
  // Este subprograma se utiliza para escribir en el vector A_x los términos correspondientes a las potencias
  //  intercambiada por el terminal

  int* puntos_iniciales = NULL;
  int* puntos_finales = NULL;
  OSQPFloat* baterias = NULL;
  int  numero_elementos_terminales = 0;
  int  numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  int  index_adicional = 0;
  OSQPFloat capacidad_bateria_actual = 0;
  

  if (cargar_puntos_iniciales_finales(informacion_sistema, terminal_actual, &numero_elementos_terminales,
    &puntos_iniciales, &puntos_finales) == ERROR) {
    printf("No se ha podido cargar los puntos iniciales y finales de las baterias en la realizacion de la matriz a_x\n");
    registrar_error("No se ha podido cargar los puntos iniciales y finales de las baterias en la realizacion de la matriz a_x", REGISTRO_ERRORES);
    if (puntos_iniciales)free(puntos_iniciales);
    if (puntos_finales)free(puntos_finales);

    return ERROR;
  }

  //Se reordenan los arrays de puntos iniciales y finales de menor a mayor
  qsort(puntos_iniciales, numero_elementos_terminales, sizeof(int), comparar_ints);
  qsort(puntos_finales, numero_elementos_terminales, sizeof(int), comparar_ints);

  if (crear_array_baterias(informacion_sistema, &baterias, puntos_iniciales, terminal_actual, numero_elementos_terminales) == ERROR) {
    printf("No se ha podido cargar el array con capacidades de baterias\n");
    registrar_error("No se ha podido cargar el array con capacidades de baterias", REGISTRO_ERRORES);
    if (puntos_iniciales)free(puntos_iniciales);
    if (puntos_finales)free(puntos_finales);
    if (baterias)free(baterias);
    return ERROR;
  }

  if (numero_elementos_terminales > 0) {
    for (int punto_actual = 0; punto_actual < numero_puntos_simulacion; punto_actual++) {

      if (numero_elementos_terminales > index_adicional) {

       

        if (comprobar_rango(punto_actual, puntos_iniciales[index_adicional], puntos_finales[index_adicional]) == true) {

          if (punto_actual == puntos_iniciales[index_adicional]) {
            
            A_x[*index_actual] = 1;
            (*index_actual)++;
            A_x[*index_actual] = -1;
            (*index_actual)++;

            //Cargo la capacidad de la bateria
            capacidad_bateria_actual = baterias[index_adicional];
          }
          else {
            //Se carga el delta de la simulacion
            OSQPInt   delta_simulacion   = informacion_sistema->informacion_puntos_simulacion.puntos_simulacion[punto_actual].delta;
            A_x[*index_actual] = 1;
            (*index_actual)++;
            A_x[*index_actual] = -1;
            (*index_actual)++;
            A_x[*index_actual] = -delta_simulacion / (capacidad_bateria_actual);
            (*index_actual)++;
   
          }

          if (puntos_finales[index_adicional] == punto_actual) {
            index_adicional++;
          }
        }
      }
      else {
        A_x[*index_actual] = 1;
        (*index_actual)++;
        A_x[*index_actual] = -1;
        (*index_actual)++;
      }
    }
  }

  else {
    for (int i = 0; i < numero_puntos_simulacion; i++) {
      A_x[*index_actual] = 1;
      (*index_actual)++;
      if ((fase_terminal != 'N') && (fase_terminal != '0')) {
        A_x[*index_actual] = -1;
        (*index_actual)++;
      }
    }
  }

  //Se libera la memoria reservada a los arrays creados durante este subprograma 
  if(baterias) free(baterias);
  if (puntos_iniciales) free(puntos_iniciales);
  if (puntos_finales) free(puntos_finales);

  return EXITO;
}




// Este subprograma se utiliza para incluir los terminos de las potencias en las estaciones de carga y en la
//  restricciones de borde

int incluir_terminos_potencias(informacion_procesada_t* informacion_sistema,OSQPFloat* A_x, int* index_actual ) {
  bool fin_bucle = false;
  OSQPInt terminal_actual = 1;
  while (!fin_bucle) {
    char fase_terminal = informacion_sistema->informacion_terminales.fases_electricas[terminal_actual - 1];
    if (incluir_terminos_potencias_terminal(informacion_sistema, terminal_actual, index_actual, A_x,fase_terminal) == ERROR) {
      printf("No se han podido incluir los terminos de potencia  de los terminales en la matriz A_x\n");
      registrar_error("No se han podido incluir los terminos de potencia de los terminales en la matriz A_x\n", REGISTRO_ERRORES);
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
