#include "calcular_numero_restricciones.h"
#include "definiciones_globales.h"
#include "funciones_calcular_a_i.h"
#include "funciones_calcular_a_x.h"
#include "funciones_calcular_a_p.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>






// Se calcula el vector A_x contienen los valores diferentes de cero
int calcular_vector_A_x(informacion_procesada_t* informacion_sistema, OSQPFloat** A_x,OSQPInt A_nnz) {

  //Primeramente hay que dimensionar el tamaño del vector A_x este vector sera tan grande como sea el numero
  //de términos diferentes de 0

  *A_x = (OSQPFloat*)malloc(A_nnz * sizeof(OSQPFloat));

  if (*A_x == NULL) {
    printf("No se ha podido reservar memoria para el vector A_x\n");
    registrar_error("No se ha podido reservar memoria para el vector A_x\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se llama a los subprogramas para incluir los diferentes términos en el vector A_x
  //Incluyo una variable para saber donde situar los valores del vector A_x
  int index_actual = 0;
  if (incluir_terminos_baterias(informacion_sistema, *A_x, &index_actual) == ERROR) {
    printf("No se han podido incluir los términos de las baterias en el vector A_x\n");
    registrar_error("No se han podido incluir los términos de las baterías en el vector A_x\n", REGISTRO_ERRORES);
    return ERROR;
  }

  if (incluir_terminos_potencias(informacion_sistema, *A_x, &index_actual) == ERROR) {
    printf("No se han podido incluir los términos de potencia en el vector A_x\n");
    registrar_error("No se han podido incluir los términos de potencia en el vector A_x\n", REGISTRO_ERRORES);
    return ERROR;
  }
  incluir_termino_potencia_red(informacion_sistema, *A_x, &index_actual);
  incluir_termino_potencia_entrada_sistema(informacion_sistema, *A_x, &index_actual);
  incluir_termino_potencia_salida_sistema(informacion_sistema, *A_x, &index_actual);
  incluir_terminos_potencia_red_fase(informacion_sistema, *A_x, &index_actual);
  incluir_terminos_potencia_entrada_fase(informacion_sistema, *A_x, &index_actual);
  incluir_terminos_potencia_salida_fase(informacion_sistema,*A_x,&index_actual);
  return EXITO;
}

// Se calcula el vector A_i contiene los índices de las filas donde se encuentre los valores
//  diferentes de 0
int calcular_vector_A_i(informacion_procesada_t* informacion_sistema, OSQPFloat** A_i,OSQPInt A_nnz) {

  //Primeramente hay que dimensionar el tamaño del vector A_i este vector será tan grande como
  //A_x y tan grande como número de elementos diferentes de 0 haya.
  *A_i = (OSQPFloat*)malloc(A_nnz * sizeof(OSQPFloat));
  if (*A_i == NULL) {
    printf("No se ha podido reservar memoria para el vector A_i\n");
    registrar_error("No se ha podido reservar memoria para el vector A_i", REGISTRO_ERRORES);
    return ERROR;
  }
  int index_actual = 0;
  int fila_actual  = 0;

  if (incluir_localizacion_baterias(informacion_sistema, *A_i, &index_actual, &fila_actual) == ERROR) {
    printf("No se ha podido indicar las filas del termino estado de bateria en la matriz A_i\n");
    registrar_error("No se ha podido indicar las filas del termino del estado de bateria en la matriz A_i", REGISTRO_ERRORES);
    return ERROR;
  }

  if (incluir_filas_potencias_terminales(informacion_sistema, *A_i, &index_actual, &fila_actual) == ERROR) {
    printf("No se ha podido indicar las filas del termino de la potencia del terminal en la matriz A_i\n");
    registrar_error("No se ha podido indicar las filas del termino de la potencia del terminal en la matriz A_i", REGISTRO_ERRORES);
    return ERROR;
  }
  int ultima_fila_balance_bateria = fila_actual;

  incluir_filas_potencia_red(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual);
  incluir_filas_potencia_entrada_red(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual);
  incluir_filas_potencia_salida_red(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual);
  incluir_filas_potencia_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'R');
  incluir_filas_potencia_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'S');
  incluir_filas_potencia_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'T');
  incluir_filas_potencia_entrada_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'R');
  incluir_filas_potencia_entrada_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'S');
  incluir_filas_potencia_entrada_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'T');
  incluir_filas_potencia_salida_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'R');
  incluir_filas_potencia_salida_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'S');
  incluir_filas_potencia_salida_red_fase(informacion_sistema, *A_i, &ultima_fila_balance_bateria, &index_actual, 'T');
  return EXITO;
}

//Se calcula el vector A_p contiene los índices de las columnas donde se encuentran los
//  valores diferentes de 0 

int calcular_vector_A_p(informacion_procesada_t* informacion_sistema, OSQPInt** A_p) {
  //cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Calculo el numero de columnas que se tienen en la matriz
  int numero_columnas = NUMERO_VARIABLES * numero_puntos_simulacion;
  int index_actual = 0;
  int columna_a_actual = 0;
  *A_p = (OSQPInt*)malloc((numero_columnas + 1) * sizeof(OSQPInt));
  //Se comprueba que la reserva de memoria ha sido exitosa
  if (*A_p == NULL) {
    printf("No se ha podido reservar memoria para el vector A_p\n");
    registrar_error("No se ha podido reservar memoria para el vector A_p",REGISTRO_ERRORES);
    return ERROR;
  }

  //Se incluyen las columnas en las que se encuentran los terminos de los estados de bateria
  if (incluir_columnas_baterias(informacion_sistema, *A_p, &index_actual, &columna_a_actual) == ERROR) {
    printf("No se ha podido incluir las columnas en las que se encuentran los terminos de estados de bateria\n");
    registrar_error("No se ha podido incluir las columnas en las que se encuentran los terminos de estados de bateria\n",REGISTRO_ERRORES);
    return ERROR;
 }
  //Se incluyen las columnas en la que se encuentran los terminos de las potencias que intercambian los terminales
  if (incluir_columnas_potencias_terminales(informacion_sistema, *A_p, &index_actual, &columna_a_actual) == ERROR) {
    printf("No se ha podido incluir las columnas en las que se encuentran los terminos de potencia de los terminales\n");
    registrar_error("No se ha podido incluir las columnas en las que se encuentran los terminos de potencia de los terminales\n", REGISTRO_ERRORES);
    return ERROR;
  }
  incluir_columnas_potencia_red(informacion_sistema,*A_p,&index_actual,&columna_a_actual);
  incluir_columnas_potencia_entrada_red(informacion_sistema, *A_p, &index_actual, &columna_a_actual);
  incluir_columnas_potencia_salida_red(informacion_sistema, *A_p, &index_actual, &columna_a_actual);
  incluir_columnas_potencias_red_fase(informacion_sistema, *A_p, &index_actual, &columna_a_actual);
  incluir_columnas_potencia_entrada_red_fase(informacion_sistema, *A_p, &index_actual, &columna_a_actual);
  incluir_columnas_potencia_salida_red_fase(informacion_sistema, *A_p, &index_actual, &columna_a_actual);
  return EXITO;
}

// Este subprograma se utiliza para calcular el numero de términos diferentes de 0
//  en la matriz A 

void calcular_numero_terminos(informacion_procesada_t* informacion_sistema,OSQPInt* A_nnz) {
  //El numero   de términos  diferentes de 0 en la matriz A es la siguiente
  //Restriccion de valores del estado de baterías (umbral inferior y superior ) 12  * puntos_simulacion
  //Restriccion de valores de la potencia que pueden intercambiar los terminales 12 * puntos_simulacion
  //Restrcicion de valores que puede intercambiar la red con el sistema      1 * puntos_simulacion
  //Restriccion de valores de potencia que puede salir del sistema a la red  1 * puntos_simulacion
  //Restriccion de valores de potencia que puede entrar al sistema de la red 1 * puntos_simulacion
  //Restriccion de valores de potencia que puede intercambiar la red con el sistema por cada fase 3 * puntos_simulacion
  //Restriccion de valores de potencia que puede entrar al sistema de la red 3 * puntos_simulacion
  //Restriccion de valores de potencia que puede salir  al sistema de la red 3 * puntos_simulacion
  //Término de la potencia que intercambia la red con el sistema por cada fase 3 * puntos_simulacion (balance
  //potencia un balance por cada fase
  //12 * puntos_simulacion (numero terminales) (Ecuación de balance de potencia)
  
 
  //Ecuacion del balance de potencia entrada/salida con la red 3 * puntos_simulacion
  //Ecuación del balance de potencia entrada/salida con la red 3 * puntos_simulacion * 3 fases = 9 *puntos_simulacion
  //Ecuacion de la potencia de entrada de la red = balance de entrada por cada fase 4 * puntos_simulacion
  //Ecuación de que la potencia de salida de la red = 4 * puntos_simulacion
  //Ecuación de que la potencia intercambiada por la red = sumatorio de lo que intercambia por cada fase 4 * puntos_simulacion
  //Parte fija = 75 * puntos_simulacion

  OSQPInt numero_terminos_restricciones_fijas = 75 * informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Batería inicial en la ecuación de balance de batería depende de cuantas conexiones y desconexiones hay
  //12 * puntos_simulacion (estado_baterias) tantas baterias como terminales.
  //12* puntos_simulacion  (Potencia intercambiada por el terminal) en las ecuaciones de balance de bateria
  OSQPInt numero_ecuaciones_baterias = 0;
  calcular_numero_ecuaciones_estado_bateria(informacion_sistema,&numero_ecuaciones_baterias);
  *A_nnz = numero_terminos_restricciones_fijas + numero_ecuaciones_baterias;
}

// Se calcula la matriz A que sirve para indicar las restricciones del sistema

int calcular_matriz_a(informacion_procesada_t * informacion_sistema, problema_optimizacion_t* problema_optimizacion) {
  matriz_a_t* matriz_a = &(problema_optimizacion->matriz_a);
  //Se procede a cargar el numero de variables
  OSQPInt n     = problema_optimizacion->numero_variables;
  OSQPInt m     = problema_optimizacion->numero_restricciones;
  OSQPInt A_nnz = 0;
  calcular_numero_terminos(informacion_sistema, &A_nnz);

  //Se procede a calcular el vector A_x
  if (calcular_vector_A_x(informacion_sistema, &(matriz_a->A_x),A_nnz,n) == ERROR) {
    printf("No se ha podido calcular el vector A_x\n");
    registrar_error("No se ha podido calcular el vector A_x\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se procede a calcular el vector A_i
  if (calcular_vector_A_i(informacion_sistema, &(matriz_a->A_i),A_nnz) == ERROR) {
    printf("No se ha podido calcular el vector A_i\n");
    registrar_error("No se ha podido calcular el vector A_i\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se procede a calcular el vector A_p
  if (calcular_vector_A_p(informacion_sistema, &(matriz_a->A_p)) == ERROR) {
    printf("No se ha podido calcular el vector A_p\n");
    registrar_error("No se ha podido calcular el vector A_p\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se reserva memoria para la matriz A
  matriz_a->A = malloc(sizeof(OSQPCscMatrix));
  if (matriz_a->A == NULL) {
    printf("No se ha podido reservar memoria para la matriz A\n");
    registrar_error("No se ha podido reservar memoria para la matriz A\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se rellenan los datos de las matrices
  csc_set_data(matriz_a->A, m, n, A_nnz, matriz_a->A_x, matriz_a->A_i, matriz_a->A_p);
  return EXITO;
}
