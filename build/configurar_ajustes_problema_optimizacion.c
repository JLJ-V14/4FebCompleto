#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <stdbool.h>

/*Este subprograma se utiliza para configurar los ajustes*/
int configurar_ajustes_problema_optimizacion(OSQPSettings ** ajustes) {
  *ajustes = (OSQPSettings*)malloc(sizeof(OSQPSettings));

  //El parámetro alfa sirve para que la solución sea más o menos y precisa y obtener así resultados más o menos
  //precisos y más o menos rápidos
  if (ajustes) {

    osqp_set_default_settings(*ajustes);
    //Se mejora la precisión de la optimización
    (*ajustes)->alpha = 1.0;
    (*ajustes)->eps_rel = 1e-5;
    (*ajustes)->eps_abs = 1e-5;
    (*ajustes)->max_iter = 100000;
    (*ajustes)->adaptive_rho = true;
    //(*ajustes)->polishing = true;
    //(*ajustes)->polish_refine_iter = true;
  }
}