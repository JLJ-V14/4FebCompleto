

#ifndef CALCULAR_NUMERO_RESTRICCIONES_H
#define CALCULAR_NUMERO_RESTRICCIONES_H

#include "tipos_optimizacion.h"
#include "osqp.h"

#ifdef __cplusplus
extern "C" {
#endif
	int calcular_numero_restricciones(informacion_procesada_t* informacion_sistema, OSQPInt* numero_restricciones);


#ifdef __cplusplus
}
#endif

#endif
