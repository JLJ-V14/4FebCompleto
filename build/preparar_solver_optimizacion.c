
#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"
#include <stdio.h>

int preparar_solver_optimizacion(problema_optimizacion_t* problema_optimizacion) {
    problema_optimizacion->bandera_salida = osqp_setup(problema_optimizacion->solver, problema_optimizacion->matriz_p.P,
    problema_optimizacion->vector_q, problema_optimizacion->matriz_a.A, problema_optimizacion->vector_l, problema_optimizacion->vector_u,
    problema_optimizacion->numero_restricciones, problema_optimizacion->numero_variables, problema_optimizacion->ajustes);


    printf("El valor de la bandera de salida\n");
    printf("%d\n",problema_optimizacion->bandera_salida);

    //Si no se ha podido configurar el problema de optimización se devuelve un error.
    if (problema_optimizacion->bandera_salida == 0) {
      printf("Retorna con exito\n");
      return EXITO;
    }
    printf("Retorna\n");
    return ERROR;

}