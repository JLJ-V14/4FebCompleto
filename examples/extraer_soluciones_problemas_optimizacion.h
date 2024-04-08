
#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"

#ifndef EXTRAER_SOLUCIONES_PROBLEMA_OPTIMIZACION_H
#define EXTRAER_SOLUCIONES_PROBLEMA_OPTIMIZACION_H

#ifdef __cplusplus
extern "C" {
#endif

	int extraer_soluciones_problema_optimizacion(informacion_procesada_t* informacion_sistema,
		problema_optimizacion_t* problema_optimizacion);

#ifdef __cplusplus
}
#endif

#endif