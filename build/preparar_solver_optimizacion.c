
#include "comprobar_informacion_matrices.h"
#include "definiciones_globales.h"
#include "liberar_memoria.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>

int preparar_solver_optimizacion(problema_optimizacion_t* problema_optimizacion, informacion_procesada_t* informacion_sistema) {

 

  /*
  
 
  
  OSQPSolver* solver;
  OSQPSettings* Settings;

  Settings = (OSQPSettings*)malloc(sizeof(OSQPSettings));
  if (Settings) {
    osqp_set_default_settings(Settings);
    Settings->alpha = 1.0;  //Change alpha parameter 
  }

  OSQPCscMatrix* P = (OSQPCscMatrix*)malloc(sizeof(OSQPCscMatrix));
  OSQPInt P_nnz = 0;  // No nonzero elements
  OSQPInt P_i[1] = { 0 };  // Dummy array (unused for zero matrix)
  OSQPFloat P_x[1] = { 0 };
  OSQPInt P_p[973] = {0 };  // Dummy array (unused for zero matrix)

  csc_set_data(P, 972, 972, P_nnz, P_x, P_i, P_p);

  OSQPFloat l[233] = { 0 };
  OSQPFloat u[233] = { 0 };
  OSQPFloat q[233] = { 0 };
   

  OSQPCscMatrix* A = (OSQPCscMatrix*)malloc(sizeof(OSQPCscMatrix));
  OSQPInt A_nnz = 0;
  OSQPInt A_i[1] = { 0 };
  OSQPFloat A_x[1] = { 0 };
  OSQPInt A_p[973] = { 0 };
  csc_set_data(A, 1566, 972, A_nnz, A_x, A_i, A_p);
  */

  


    problema_optimizacion->bandera_salida = osqp_setup(&(problema_optimizacion->solver),problema_optimizacion->matriz_p.P,
    problema_optimizacion->vector_q, problema_optimizacion->matriz_a.A, problema_optimizacion->vector_l, problema_optimizacion->vector_u,
    problema_optimizacion->numero_restricciones,problema_optimizacion->numero_variables, problema_optimizacion->ajustes);

  /*
    problema_optimizacion->bandera_salida = osqp_setup(&(problema_optimizacion->solver), problema_optimizacion->matriz_p.P,
    problema_optimizacion->vector_q, problema_optimizacion->matriz_a.A, problema_optimizacion->vector_l, problema_optimizacion->vector_u,
    problema_optimizacion->numero_restricciones, problema_optimizacion->numero_variables, problema_optimizacion->ajustes);

    */
    
    
 




    
  
  
    printf("El valor de la bandera de salida\n");
    printf("%lld\n",problema_optimizacion->bandera_salida);

    //Si no se ha podido configurar el problema de optimización se devuelve un error.
    if (problema_optimizacion->bandera_salida == 0) {
      printf("Retorna con exito\n");
      return EXITO;
    }
    
    
    printf("Retorna\n");
    return ERROR;

}