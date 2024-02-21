#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>

/*Este subprograma se utiliza para definir el vector p_x con el que se define la matriz p*/

int calcular_vector_px(informacion_procesada_t* informacion_sistema, OSQPFloat** P_x) {

}

/*Este subprograma se utiliza para definir el vector p_i con el que se define la matriz p*/

int calcular_vector_pi(informacion_procesada_t* informacion_sistema, OSQPFloat** P_i) {

}

/*Este subprograma se utiliza para definir el vector p_p con el que se define la matriz p*/

int calcular_vector_pp(informacion_procesada_t* informacion_sistema, OSQPFloat** P_p) {

}

/*Este subprograma se utiliza para calcular el número de términos diferentes de 0 */
int calcular_numero_terminos(informacion_procesada_t* informacion_sistema) {

}

/*Este subprograma se utiliza para calcular la matriz p del problema de optimización */

int calcular_matriz_p(informacion_procesada_t* informacion_sistema, matriz_p_t *matriz_p) {
  //Se calcular el vector px
  if ((calcular_vector_px(informacion_sistema, matriz_p->P_x)) == ERROR) {
    printf("No se ha podido calcular el vector px\n");
    registrar_error("No se ha podido calcular el vector px\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se calcula el vector pi
  if ((calcular_vector_pi(informacion_sistema, matriz_p->P_i)) == ERROR) {
    printf("No se ha podido calcular el vector pi\n");
    registrar_error("No se ha podido calcular el vector pi\n", REGISTRO_ERRORES);
    return ERROR;
  }
  if ((calcular_vector_pp(informacion_sistema, matriz_p->P_p)) == ERROR) {
    printf("No se ha podido calcular el vector pp\n");
    registrar_error("No se ha podido calcular el vector pp\n", REGISTRO_ERRORES);
    return ERROR;
  }

  //Se reserva memoria para la matriz P
  matriz_p->P = malloc(sizeof(OSQPCscMatrix));

  if (matriz_p->P == NULL) {
    printf("No se ha podido reservar memoria para la matriz P\n");
    registrar_error("No se ha podido reservar memoria para la matriz P\n",REGISTRO_ERRORES);
    return ERROR;
  }
  
  return EXITO;
}