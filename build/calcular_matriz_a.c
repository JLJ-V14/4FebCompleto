#include "definiciones_globales.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>

/*Se calcula el vector A_x contienen los valores diferentes de cero*/
int calcular_vector_A_x(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x) {

}

int calcular_vector_A_i(informacion_procesada_t* informacion_sistema, OSQPFloat* A_i) {

}

int calcular_vector_A_p(informacion_procesada_t* informacion_sistema, OSQPFloat* A_p) {

}

int calcular_matriz_a(informacion_procesada_t * informacion_sistema, matriz_a_t * matriz_a) {
  if (calcular_vector_A_x(informacion_sistema, matriz_a->A_x) == ERROR) {
    printf("No se ha podido calcular el vector A_x\n");
    registrar_error("No se ha podido calcular el vector A_x\n", REGISTRO_ERRORES);
    return ERROR;
  }
}