#include "calcular_matriz_a.h"
#include "calcular_matriz_p.h"
#include "calcular_matriz_q.h"
#include "calcular_vector_l_u.h"
#include "definiciones_globales.h"
#include "mostrar_informacion_terminales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>





int calcular_matrices(informacion_procesada_t *informacion_sistema, problema_optimizacion_t* problema_optimizacion,
 informacion_carga_terminales_t* elementos_programados_terminales) {


  
	//Esta función se encarga de rellenar las matrices P,A,l,u,q con los datos del problema de optimizacion 
  if (calcular_matriz_a(informacion_sistema,problema_optimizacion,elementos_programados_terminales) == ERROR) {
    printf("No se ha podido calcular la matriz A en el problema de optimizacion\n");
    registrar_error("No se ha podido calcular la matriz A en el problema de optimizacion", REGISTRO_ERRORES);
    return ERROR;
  }


  

  if (calcular_matriz_p(informacion_sistema,problema_optimizacion,elementos_programados_terminales) == ERROR) {
    printf("No se ha podido calcular la matriz P en el problema de optimizacion\n");
    registrar_error("No se ha podido calcular la matriz P en el problema de optimizacion", REGISTRO_ERRORES);
    return ERROR;
  }


  
  if (calcular_vector_q(informacion_sistema, &(problema_optimizacion->vector_q),elementos_programados_terminales) == ERROR) {
    printf("No se ha podido calcular la matriz Q en el problema de optimizacion\n");
    registrar_error("No se ha podido calcular la matriz Q en el problema de optimizacion", REGISTRO_ERRORES);
    return ERROR;
  }

  
  
  if (calcular_vector_l_u(informacion_sistema, &(problema_optimizacion->vector_l), &(problema_optimizacion->vector_u),
      elementos_programados_terminales) == ERROR) {
    printf("No se ha podido calcular el vector l en el problema de optimizacion\n");
    registrar_error("No se ha podido calcular el vector l en el problema de optimizacion\n", REGISTRO_ERRORES);
    return ERROR;
  }
  /* */
  
  return EXITO;
}
