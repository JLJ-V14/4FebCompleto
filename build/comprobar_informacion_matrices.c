
#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include  <stdio.h>
#include  <stdlib.h>


/*Se utiliza el siguiente subprograma para imprimir los vectores que componen la matriz P */
int imprimir_vectores_p(problema_optimizacion_t* informacion_problema_optimizacion, informacion_procesada_t* informacion_simulacion) {
  FILE* archivo_comprobacion_P_x = fopen("archivo_comprobacion_p_x.csv", "w");
  FILE* archivo_comprobacion_P_p = fopen("archivo_comprobacion_p_p.csv", "w");
  FILE* archivo_comprobacion_P_i = fopen("archivo_comprobacion_p_i.csv", "w");

  if ((archivo_comprobacion_P_x == NULL) || (archivo_comprobacion_P_i == NULL) || (archivo_comprobacion_P_p == NULL)) {
    printf("No se han podido abrir los csvs de comprobacion de los vectores de la matriz P\n");
    registrar_error("No se han podido abrir los csvs de comprobacion de los vectores de la matriz P\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se cargan el numero de elementos que se tienen diferentes de 0
  OSQPInt* P_nnz = informacion_problema_optimizacion->matriz_p.P_nnz;

  //Se cargan los diferentes vectorees P
  OSQPFloat* P_x = informacion_problema_optimizacion->matriz_p.P_x;
  OSQPInt* P_i = informacion_problema_optimizacion->matriz_p.P_i;
  OSQPInt* P_p = informacion_problema_optimizacion->matriz_p.P_p;

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_simulacion->informacion_puntos_simulacion.numero_puntos_simulacion;

  //Se carga el numero de columnas
  int numero_columnas = (NUMERO_VARIABLES)*numero_puntos_simulacion + 1;

  for (int i = 0; i < P_nnz; i++) {
    fprintf(archivo_comprobacion_P_x, "%f\n", P_x[i]);
  }

  for (int i = 0; i < P_nnz; i++) {
    fprintf(archivo_comprobacion_P_i, "%d\n", P_i[i]);
  }

  for (int i = 0; i < numero_columnas; i++) {
    fprintf(archivo_comprobacion_P_p, "%d\n", P_p[i]);
  }

  fclose(archivo_comprobacion_P_i);
  fclose(archivo_comprobacion_P_x);
  fclose(archivo_comprobacion_P_p);
  return EXITO;
}

int imprimir_vectores_a(problema_optimizacion_t* informacion_problema_optimizacion,
  informacion_procesada_t* informacion_simulacion) {
  FILE* archivo_comprobacion_A_x = fopen ("archivo_comprobacion_a_x.csv", "w");
  FILE* archivo_comprobacion_A_p = fopen("archivo_comprobacion_a_p.csv", "w");
  FILE* archivo_comprobacion_A_i = fopen("archivo_comprobacion_a_i.csv", "w");
  

  if (archivo_comprobacion_A_x == NULL || (archivo_comprobacion_A_i == NULL) || (archivo_comprobacion_A_p == NULL)) {
    printf("No se han podido abrir los csvs de comprobación de los vectores de la matriz A\n");
    registrar_error("No se han podido abrir los csvs de comprobación de los vectores de la matriz A\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se carga los vectores A_x, A_i y A_p
  OSQPFloat* A_x = informacion_problema_optimizacion->matriz_a.A_x;
  OSQPInt* A_i = informacion_problema_optimizacion->matriz_a.A_i;
  OSQPInt* A_p = informacion_problema_optimizacion->matriz_a.A_p;

  //Se carga el numero de términos diferentes de 0
  OSQPInt A_nnz = informacion_problema_optimizacion->matriz_a.A_nnz;
  

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_simulacion->informacion_puntos_simulacion.numero_puntos_simulacion;

    //Se carga el numero de columnas
  int numero_columnas = (NUMERO_VARIABLES)*numero_puntos_simulacion + 1;

  //Se pasa a escribir los elementos de los vectores A_x, A_i y A_p

  for (int i = 0; i<A_nnz; i++) {
    fprintf(archivo_comprobacion_A_x, "%f\n", A_x[i]);
  }

  for (int i = 0; i < A_nnz; i++) {
    fprintf(archivo_comprobacion_A_i, "%d\n", A_i[i]);
  }

  for (int i = 0; i <numero_columnas; i++) {
    fprintf(archivo_comprobacion_A_p, "%d\n", A_p[i]);
  }

  fclose(archivo_comprobacion_A_x);
  fclose(archivo_comprobacion_A_i);
  fclose(archivo_comprobacion_A_p);
  return EXITO;
}



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
  }

  fclose(archivo);
  return EXITO;
}

int imprimir_vector_l(problema_optimizacion_t* informacion_problema_optimizacion) {
  FILE* archivo = fopen("comprobacion_vector_l.csv", "w");

  if (archivo == NULL) {
    printf("Error abriendo el csv de comprobación del vector l\n");
    registrar_error("Error abriendo el csv de comprobación del vector l\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se calcula el numero de elementos del vector l
  int numero_elementos_vector_l = (informacion_problema_optimizacion->numero_restricciones);

 

  for (int elemento_actual = 0; elemento_actual < numero_elementos_vector_l; elemento_actual++) {
    fprintf(archivo, "%f\n", informacion_problema_optimizacion->vector_l[elemento_actual]);
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
  int numero_elementos_vector_u = (informacion_problema_optimizacion->numero_restricciones);



  for (int elemento_actual = 0; elemento_actual < numero_elementos_vector_u; elemento_actual++) {
    fprintf(archivo, "%f\n", informacion_problema_optimizacion->vector_u[elemento_actual]);
  }

  fclose(archivo);
  return EXITO;
}



/*Este subprograma se utiliza para imprimir en el csv la información de la matriz P*/
int imprimir_matriz_P(problema_optimizacion_t* informacion_problema_optimizacion) {
  FILE* archivo = fopen("comprobacion_matriz_p.csv", "w");

  if (archivo == NULL) {
    printf("No se ha podido abrir el archivo de comprobación de la matriz A\n");
    registrar_error("No se ha podido abrir el archivo de comprobación de la matriz A\n", REGISTRO_ERRORES);
    return ERROR;
  }

  // Se carga la matriz A (se asume que ya está rellenada de datos)
  OSQPCscMatrix* P = informacion_problema_optimizacion->matriz_p.P;

  // Se escribe las dimensiones de las matrices en la primera fila
  fprintf(archivo, "%d,%d\n", P->n, P->n);



  printf("n es %d\n", P->n);


  // Se escribe los elementos de la matriz en el CSV
  for (int i = 0; i < P->n; i++) {

    for (int j = 0; j < P->n; j++) {
      // Buscar el índice de la matriz A correspondiente a la fila y columna actuales
      int idx = -1;
      for (int k = P->p[j]; k < P->p[j + 1]; k++) {
        if (P->i[k] == i) {
          idx = k;
          break;
        }
      }
      // Si el índice es encontrado, escribir el valor correspondiente; de lo contrario, escribir 0
      if (idx != -1) {
        fprintf(archivo, "%f,", P->x[idx]);
      }
      else {
        fprintf(archivo, "%f,", 0.0);
      }
    }
    fprintf(archivo, "\n"); // Nueva línea después de cada fila
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
  
  // Se carga la matriz A (se asume que ya está rellenada de datos)
  OSQPCscMatrix* A = informacion_problema_optimizacion->matriz_a.A;

  // Se escribe las dimensiones de las matrices en la primera fila
  fprintf(archivo, "%d,%d\n", A->m, A->n);



  printf("m es %d\n", A->m);
  printf("n es %d\n", A->n);


  // Se escribe los elementos de la matriz en el CSV
  for (int i = 0; i < A->m; i++) {
   
    for (int j = 0; j < A->n; j++) {
      // Buscar el índice de la matriz A correspondiente a la fila y columna actuales
      int idx = -1;
      for (int k = A->p[j]; k < A->p[j + 1]; k++) {
        if (A->i[k] == i) {
          idx = k;
          break;
        }
      }
      // Si el índice es encontrado, escribir el valor correspondiente; de lo contrario, escribir 0
      if (idx != -1) {
        fprintf(archivo, "%f,", A->x[idx]);
      }
      else {
        fprintf(archivo, "%f,", 0.0);
      }
    }
    fprintf(archivo, "\n"); // Nueva línea después de cada fila
  }
  
  fclose(archivo);
  return EXITO;
}



/*Este subprograma se utiliza para imprimir las diferentes matrices del problema de optimizacion*/
int imprimir_matrices_problema_optimizacion(problema_optimizacion_t* informacion_problema_optimizacion,
  informacion_procesada_t* informacion_sistema) {

  

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

  
  if (imprimir_vectores_a(informacion_problema_optimizacion, informacion_sistema) == ERROR) {
    printf("No se ha podido imprimir los datos de los vectores A_x, A_i y A_p\n");
    registrar_error("No se ha podido imprimir los datos de los vectores A_x, A_i y A_p\n", REGISTRO_ERRORES);
    return ERROR;
  }

  if (imprimir_vectores_p(informacion_problema_optimizacion, informacion_sistema) == ERROR) {
    printf("No se ha podido imprimir los datos de los vectores P_x, P_i y P_p\n");
    registrar_error("No se ha podido imprimir los datos de los vectores P_x, P_i y P_p\n", REGISTRO_ERRORES);
    return ERROR;
  }

  return EXITO;
}