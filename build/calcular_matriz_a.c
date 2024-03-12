#include "calcular_numero_restricciones.h"
#include "definiciones_globales.h"
#include "funciones_calcular_a_i.h"
#include "funciones_calcular_a_x.h"
#include "funciones_calcular_a_p.h"
#include "mostrar_informacion_terminales.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>






// Se calcula el vector A_x contienen los valores diferentes de cero, en las restricciones, esto incluye tanto
// las restricciones de borde como los terminos que aparecen en las diferentes ecuaciones

int calcular_vector_A_x(informacion_procesada_t* informacion_sistema, OSQPFloat** A_x,OSQPInt A_nnz,
  informacion_carga_terminales_t* elementos_programados_terminales) {

  //Primeramente hay que dimensionar el tamaño del vector A_x este vector sera tan grande como sea el numero
  //de términos diferentes de 0

  


  *A_x = (OSQPFloat*) malloc(A_nnz * sizeof(OSQPFloat));

  if (*A_x == NULL) {
    printf("No se ha podido reservar memoria para el vector A_x\n");
    registrar_error("No se ha podido reservar memoria para el vector A_x\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se llama a los subprogramas para incluir los diferentes términos en el vector A_x
  //Incluyo una variable para saber donde situar los valores del vector A_x, es la posicion del vector por el cual
  //se están añadiendo términos.
  int index_actual = 0;


 
  
  //Se llama al siguiente subprograma para incluir en la matriz de restricciones los terminos de la variable SOC 
  incluir_terminos_baterias_A_x(informacion_sistema, *A_x, &index_actual, elementos_programados_terminales);
  printf("El index actual despues de añadir las baterias es %d\n", index_actual);
  incluir_terminos_potencias_A_x(informacion_sistema, *A_x, &index_actual, elementos_programados_terminales);
  printf("El index actual despues de añadir los terminas de potencias intercambiados por los terminales es %d\n", index_actual);
  incluir_termino_potencia_red(informacion_sistema, *A_x, &index_actual);
  incluir_termino_potencia_entrada_sistema(informacion_sistema, *A_x, &index_actual);
  incluir_termino_potencia_salida_sistema(informacion_sistema, *A_x, &index_actual);
  incluir_terminos_potencia_red_fase(informacion_sistema, *A_x, &index_actual);
  incluir_terminos_potencia_entrada_fase(informacion_sistema, *A_x, &index_actual);
  incluir_terminos_potencia_salida_fase(informacion_sistema,*A_x,&index_actual);
  printf("el index actual es %d \n", index_actual);
  return EXITO;
}

// Se calcula el vector A_i contiene los índices de las filas donde se encuentre los valores
//  diferentes de 0
int calcular_vector_A_i(informacion_procesada_t* informacion_sistema, OSQPFloat** A_i,OSQPInt A_nnz,
  informacion_carga_terminales_t* elementos_programados_terminales) {

  //Primeramente hay que dimensionar el tamaño del vector A_i este vector será tan grande como
  //A_x y tan grande como número de elementos diferentes de 0 haya.
  *A_i = (OSQPFloat*)malloc(A_nnz * sizeof(OSQPFloat));
  if (*A_i == NULL) {
    printf("No se ha podido reservar memoria para el vector A_i\n");
    registrar_error("No se ha podido reservar memoria para el vector A_i", REGISTRO_ERRORES);
    return ERROR;
  }
  int index_actual = 0;

  //Cargo el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se define la primera fila en la cual se situa la ecuacion del balance de bateria
  int fila_actual_balance_bateria  = (NUMERO_VARIABLES +3) * numero_puntos_simulacion;

  if (incluir_filas_baterias(informacion_sistema, *A_i, &index_actual, &fila_actual_balance_bateria,elementos_programados_terminales) == ERROR) {
    printf("No se ha podido indicar las filas del termino estado de bateria en la matriz A_i\n");
    registrar_error("No se ha podido indicar las filas del termino del estado de bateria en la matriz A_i", REGISTRO_ERRORES);
    return ERROR;
  }

  if (incluir_filas_potencias_terminales(informacion_sistema, *A_i, &index_actual, &fila_actual_balance_bateria,elementos_programados_terminales) == ERROR) {
    printf("No se ha podido indicar las filas del termino de la potencia del terminal en la matriz A_i\n");
    registrar_error("No se ha podido indicar las filas del termino de la potencia del terminal en la matriz A_i", REGISTRO_ERRORES);
    return ERROR;
  }

  int ultima_fila_balance_bateria = fila_actual_balance_bateria;

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

int calcular_vector_A_p(informacion_procesada_t* informacion_sistema, OSQPInt** A_p,
  informacion_carga_terminales_t* informacion_carga_terminales) {
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
  if (incluir_columnas_baterias(informacion_sistema, *A_p, &index_actual, &columna_a_actual,informacion_carga_terminales) == ERROR) {
    printf("No se ha podido incluir las columnas en las que se encuentran los terminos de estados de bateria\n");
    registrar_error("No se ha podido incluir las columnas en las que se encuentran los terminos de estados de bateria\n",REGISTRO_ERRORES);
    return ERROR;
 }
  //Se incluyen las columnas en la que se encuentran los terminos de las potencias que intercambian los terminales
  if (incluir_columnas_potencias_terminales(informacion_sistema, *A_p, &index_actual, &columna_a_actual,informacion_carga_terminales) == ERROR) {
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

  //Terminos de Borde 36 * puntos de simulacion (si o si)

  //En las ecuaciones de balance aparecen si o si 3 * puntos de simulacion balance de fase
  //En las ecuaciones de balance de bateria aparecen si o si 12 * puntos de simulacion terminos de los estados de batería
  //Ecuacion 34 -> 3 terminos * puntos de simulacion
  //Ecuacion 35 -> 9 terminos * puntos de simulacion
  //Ecuacion 36 -> 4 terminos * puntos de simulacion
  //Ecuacion 37 -> 4 terminos * puntos de simulacion
  //Ecuacion 38 -> 4 terminos * puntos de simulacion 

  //Terminos fijos = 
  OSQPInt numero_terminos_restricciones_fijas = 75 * informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Batería inicial en la ecuación de balance de batería depende de cuantas conexiones y desconexiones hay
  //12 * puntos_simulacion (estado_baterias) tantas baterias como terminales.
  //12* puntos_simulacion  (Potencia intercambiada por el terminal) en las ecuaciones de balance de bateria

  OSQPInt numero_ecuaciones_baterias = 0;

  //Se pasa a calcular los terminos adicionales que deben ser incluidos en las ecuaciones de balance de baterías
  //y las ecuaciones del balance de fase
  OSQPInt terminos_adicionales_balance_fase = 0;
  OSQPInt terminos_adicionales_balance_bateria = 0;

  calcular_numero_terminos_ecuaciones_baterias(informacion_sistema,&terminos_adicionales_balance_bateria);
  calcular_numero_terminos_ecuacion_balance_fase(informacion_sistema,&terminos_adicionales_balance_fase);


  printf("El numero de terminos en las ecuaciones de baterias es %d\n", terminos_adicionales_balance_fase);
  printf("El numero de terminos en las ecuaciones de balance de bateria es %d\n", terminos_adicionales_balance_bateria);
  printf("El numero de terminos fijos  es %d\n", numero_terminos_restricciones_fijas);

  //calcular_numero_ecuaciones_estado_bateria(informacion_sistema,&numero_ecuaciones_baterias);

  *A_nnz = numero_terminos_restricciones_fijas +terminos_adicionales_balance_fase + terminos_adicionales_balance_bateria;
}

// Se calcula la matriz A que sirve para indicar las restricciones del sistema

int calcular_matriz_a(informacion_procesada_t * informacion_sistema, problema_optimizacion_t* problema_optimizacion,
  informacion_carga_terminales_t* programacion_carga_terminales) {
  matriz_a_t* matriz_a = &(problema_optimizacion->matriz_a);
  //Se procede a cargar el numero de variables
  OSQPInt n     = problema_optimizacion->numero_variables;
  OSQPInt m     = problema_optimizacion->numero_restricciones;
  OSQPInt A_nnz = 0;

  calcular_numero_terminos(informacion_sistema, &A_nnz);

  printf("El numero de términos diferentes de cero en la matriz A es %d\n", A_nnz);

  //Se procede a calcular el vector A_x
  if (calcular_vector_A_x(informacion_sistema, &(matriz_a->A_x),A_nnz,programacion_carga_terminales) == ERROR) {
    printf("No se ha podido calcular el vector A_x\n");
    registrar_error("No se ha podido calcular el vector A_x\n", REGISTRO_ERRORES);
    return ERROR;
  }

  printf("El numero de términos diferentes de cero en la matriz A es %d\n", A_nnz);

  //Se procede a calcular el vector A_i
  if (calcular_vector_A_i(informacion_sistema, &(matriz_a->A_i),A_nnz,programacion_carga_terminales) == ERROR) {
    printf("No se ha podido calcular el vector A_i\n");
    registrar_error("No se ha podido calcular el vector A_i\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se procede a calcular el vector A_p
  if (calcular_vector_A_p(informacion_sistema, &(matriz_a->A_p),programacion_carga_terminales) == ERROR) {
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
