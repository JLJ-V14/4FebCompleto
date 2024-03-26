
#include "calcular_matrices.h"
#include "calcular_numero_variables.h"
#include "calcular_numero_restricciones.h"
#include "configurar_ajustes_optimizacion.h"
#include "comprobar_informacion_matrices.h"
#include "definiciones_globales.h"
#include "mostrar_informacion_terminales.h"
#include "preparar_solver_optimizacion.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>


//Este subprograma se utiliza para 
int preparar_problema_optimizacion(informacion_procesada_t* informacion_sistema,problema_optimizacion_t* problema_optimizacion,
  informacion_carga_terminales_t* programacion_carga_terminales) {

  //Se procede a calcular el numero de variables que tiene el problema de optimizacion
  calcular_numero_variables(informacion_sistema, &(problema_optimizacion->numero_variables));
  
  
  
  //Se procede a calcular el numero de restricciones que tiene el problema de optimizacion
  calcular_numero_restricciones(informacion_sistema,&(problema_optimizacion->numero_restricciones));

  
  
  //Se procede a calcular las diferentes matrices del problema de optimizacion
  if ((calcular_matrices(informacion_sistema,problema_optimizacion,programacion_carga_terminales)) == ERROR) {
    printf("No se han podido calcular las matrices del problema de optimizacion\n");
    registrar_error("No se han podido calcular las matrices del problema de optimizacion\n", REGISTRO_ERRORES);
    return ERROR;
  }
  
  
  
  //Se procede a configurar los ajustes del problema de optimizacion
  if ((configurar_ajustes_problema_optimizacion(&(problema_optimizacion->ajustes))) == ERROR) {
    printf("No se ha podido configurar los ajustes del problema de optimizacion\n");
    registrar_error("No se ha podido configurar los ajustes del problema de optimizacion\n",REGISTRO_ERRORES);
    return ERROR;
  }
 
 
  
  
  
  //Se muestra la informacion de las matrices para comprobar si se han escrito correctamente
  if ((imprimir_matrices_problema_optimizacion(problema_optimizacion,informacion_sistema)) == ERROR) {
    printf("No se ha podido escribir las matrices del problema de optimización en el csv\n");
    registrar_error("No se han podido escribir las matrices del problema de optimización en el csv\n", REGISTRO_ERRORES);
    return ERROR;
  }
  /* */
  
 
  
  
  //Se procede a preparar el solver del problema de optimizacion.
  if (preparar_solver_optimizacion(problema_optimizacion,informacion_sistema) == ERROR) {
    printf("No se ha podido preparar el solver para el problema optimizacion\n");
    registrar_error("No se ha podido preparar el solver para el problema de optimizacion\n", REGISTRO_ERRORES);
    return ERROR;
  }
  /*
  
  */
  return EXITO;
}
