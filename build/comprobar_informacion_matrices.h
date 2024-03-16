#ifndef COMPROBAR_INFORMACION_MATRICES_H
#define COMPROBAR_INFORMACION_MATRICES_H

#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif
	int imprimir_matrices_problema_optimizacion(problema_optimizacion_t* informacion_problema_optimizacion,
		informacion_procesada_t* informacion_sistema);
	int imprimir_vectores_a(problema_optimizacion_t* informacion_problema_optimizacion, informacion_procesada_t* informacion_simulacion);

#ifdef __cplusplus
}
#endif

#endif