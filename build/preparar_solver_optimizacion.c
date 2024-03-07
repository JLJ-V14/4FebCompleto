
#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"

int preparar_solver_optimizacion(problema_optimizacion_t* problema_optimizacion) {
    problema_optimizacion->bandera_salida = osqp_setup(problema_optimizacion->solver, problema_optimizacion->matriz_p.P,
    problema_optimizacion->vector_q, problema_optimizacion->matriz_a.A, problema_optimizacion->vector_l, problema_optimizacion->vector_u,
    problema_optimizacion->numero_restricciones, problema_optimizacion->numero_variables, problema_optimizacion->ajustes);
}