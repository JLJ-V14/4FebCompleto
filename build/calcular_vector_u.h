
#ifndef CALCULAR_VECTOR_U_H
#define CALCULAR_VECTOR_U_H

#include "osqp.h"
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif
	int calcular_vector_u(informacion_procesada_t* informacion_sistema, OSQPFloat** vector_u);


#ifdef __cplusplus
}
#endif

#endif