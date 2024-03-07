#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"
#include <stdlib.h>

/*Este subprograma se utiliza para configurar los ajustes*/
int configurar_ajustes_problema_optimizacion(OSQPSettings ** ajustes) {
  *ajustes = (OSQPSettings*)malloc(sizeof(OSQPSettings));

  //El parámetro alfa sirve para que la solución sea más o menos y precisa y obtener así resultados más o menos
  //precisos y más o menos rápidos
  if (ajustes) {
    osqp_set_default_settings(*ajustes);
    (*ajustes)->alpha = 1.0;
  }
}