


#ifndef CALCULAR_VECTOR_L_H
#define CALCULAR_VECTOR_L_H

#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif
	int calcular_vector_l_u(informacion_procesada_t* informacion_sistema, OSQPFloat** l,OSQPFloat** u,
		informacion_carga_terminales_t * elementos_programados_carga_terminal);


#ifdef __cplusplus
}
#endif

#endif