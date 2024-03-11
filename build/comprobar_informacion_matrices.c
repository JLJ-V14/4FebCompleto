
#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include  <stdio.h>
#include  <stdlib.h>

int imprimir_vector_q(problema_optimizacion_t* informacion_problema_optimizacion) {
  FILE* archivo = fopen("comprobacion_vector_q.csv", "w");
  if (archivo == NULL) {
    printf("Error abriendo el archivo a comprobar el vector q\n");
    return ERROR;
  }

  //Se calcula el numero de elementos del vector q
  int numero_elementos_vector_q = informacion_problema_optimizacion->numero_variables;

  for (int elemento_actual = 0; elemento_actual < numero_elementos_vector_q; elemento_actual++) {
    fprintf(archivo, "%f\n", informacion_problema_optimizacion->vector_q[elemento_actual]);
    fprintf(archivo,"\n");
  }

  fclose(archivo);
  return EXITO;
}

int imprimir_vector_l(problema_optimizacion_t* informacion_problema_optimizacion) {
  FILE* archivo = fopen("comprobacion_vector_l.csv", "w");

  if (archivo == NULL) {
    printf("Error abriendo el csv de comprobación del vector l\n");
    return ERROR;
  }

  //Se calcula el numero de elementos del vector l
  int numero_elementos_vector_l = (informacion_problema_optimizacion->numero_variables) + (informacion_problema_optimizacion->numero_restricciones);

  for (int elemento_actual = 0; elemento_actual < numero_elementos_vector_l; elemento_actual++) {
    fprintf(archivo, "%f\n", informacion_problema_optimizacion->vector_l[elemento_actual]);
    fprintf(archivo, "\n");
  }

  fclose(archivo);
  return EXITO;
}

int imprimir_vector_u(problema_optimizacion_t* informacion_problema_optimizacion) {
  FILE* archivo = fopen("comprobacion_vector_u.csv", "w");

  if (archivo == NULL) {
    printf("Error abriendo el archivo de comprobacion del vector u\n");
    return ERROR;
  }

  //Se calcula el numero de elementos del vector u
  int numero_elementos_vector_u = (informacion_problema_optimizacion->numero_variables + informacion_problema_optimizacion->numero_restricciones);

  for (int elemento_actual = 0; elemento_actual < numero_elementos_vector_u; elemento_actual++) {
    fprintf(archivo, "%f\n", informacion_problema_optimizacion->vector_u[elemento_actual]);
    fprintf(archivo, "\n");
  }

  fclose(archivo);
  return EXITO;
}



/*Este subprograma se utiliza para imprimir en el csv la información de la matriz P*/
int imprimir_matriz_P(problema_optimizacion_t* informacion_problema_optimizacion) {
  FILE* archivo = fopen("comprobacion_matriz_p.csv", "w");

  if (archivo == NULL) {
    printf("No se ha podido abrir el archivo de comprobación de la matriz P\n");
    registrar_error("No se ha podido abrir el archivo de comprobación de la matriz P\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se carga la matriz P (se asume que ya está rellenada de datos
  OSQPCscMatrix* P = informacion_problema_optimizacion->matriz_p.P;
  //Se escribe las dimensiones de las matrices en la primera fila
  fprintf(archivo, "%d,%d\n", P->m, P->n);

  //Se escribe los elementos de la matriz en el csv
  for (int i = 0; i < P->n; i++) {
    OSQPInt fila = (i < P->m) ? P->i[i] : P->m - 1;      //  If i < P->m, use row index from P->i[i], else use P->m - 1
    OSQPInt col = (i < P->m) ? P->p[i] : P->p[P->n - 1]; // If i < P->m, use col index from P->p[i], else use P->p[P->n - 1]
    OSQPFloat valor = (i < P->p[P->n]) ? P->x[i] : 0.0; // If i < number of non-zero elements, use value from P->x[i], else use 0
    fprintf(archivo, "%d,%d,%f\n", fila, col, valor);
  }

  fclose(archivo);
  return EXITO;
}

/*Este subprograma se utiliza para imprimir en el csv la información de la matriz A*/
int imprimir_matriz_A(problema_optimizacion_t* informacion_problema_optimizacion) {
  FILE* archivo = fopen("comprobacion_matriz_A.csv", "w");

  if (archivo == NULL) {
    printf("No se ha podido abrir el archivo de comprobación de la matriz A\n");
    registrar_error("No se ha podido abrir el archivo de comprobación de la matriz A\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se carga la matriz A (se asume que ya está rellenada de datos)
  OSQPCscMatrix* A = informacion_problema_optimizacion->matriz_a.A;

  //Se escribe las dimensiones de las matrices en la primera fila
  fprintf(archivo, "%d,%d\n", A->m, A->n);

  //Se escribe los elementos de la matriz en el csv
  for (int i = 0; i < A->n; i++) {
    OSQPInt fila = (i < A->m) ? A->i[i] : A->m - 1; //If i< A->m, use row index from A->i[i], else use A->m - 1
    OSQPInt col = (i < A->m) ? A->p[i] : A->p[A->n - 1]; // If i < A->m, use col index from A->p[i], else use A->p[P->n - 1]
    OSQPFloat valor = (i < A->p[A->n]) ? A->x[i] : 0.0; // If i < number of non-zero elements, use value from A->x[i], else use 0
    fprintf(archivo, "%d,%d,%f\n", fila, col, valor);
  }

  fclose(archivo);
  return EXITO;
}



/*Este subprograma se utiliza para imprimir las diferentes matrices del problema de optimizacion*/
int imprimir_matrices_problema_optimizacion(problema_optimizacion_t* informacion_problema_optimizacion) {

  if (imprimir_vector_l(informacion_problema_optimizacion) == ERROR) {
    printf("No se han podido imprimir los datos del vector l en el csv de comprobación\n");
    registrar_error("No se han podido imprimir los datos del vector l en el csv de comprobación\n", REGISTRO_ERRORES);
    return ERROR;
  }

  if (imprimir_vector_u(informacion_problema_optimizacion) == ERROR) {
    printf("No se han podido imprimir los datos del vector u en el csv de comprobación\n");
    registrar_error("No se han podido imprimir los datos del vector u en el csv de comprobación\n", REGISTRO_ERRORES);
    return ERROR;
  }

  if (imprimir_matriz_A(informacion_problema_optimizacion) == ERROR){
    printf("No se ha podido imprimir los datos de la matriz A en el csv de comprobación \n");
    registrar_error("No se han podido imprimir los datos de la matriz A en el csv de comprobación\n", REGISTRO_ERRORES);
    return ERROR;
  }


  if (imprimir_matriz_P(informacion_problema_optimizacion) == ERROR) {
    printf("No se ha podido imprimir los datos del vector P en el csv de comprobación\n");
    registrar_error("No se han podido imprimir los datos del vector P en el csv de comprobación\n", REGISTRO_ERRORES);
    return ERROR;
  }

  if (imprimir_vector_q(informacion_problema_optimizacion) == ERROR) {
    printf("No se ha podido imprimir los datos del vector q en el csv de comprobación\n");
    registrar_error("No se ha podido imprimir los datos del vector q en el csv de comprobación\n", REGISTRO_ERRORES);
    return ERROR;
  }

  return EXITO;
}