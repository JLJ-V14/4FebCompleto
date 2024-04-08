
#ifndef CALCULAR_MATRICES_H
#define CALCULAR_MATRICES_H

#include "tipos_optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif

	int calcular_matrices(informacion_procesada_t* informacion_sistema, problema_optimizacion_t* problema_optimizacion,
		informacion_carga_terminales_t* programacion_carga_terminales);

#ifdef __cplusplus
}
#endif

#endif