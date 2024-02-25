

#ifndef CALCULAR_NUMERO_RESTRICCIONES_H
#define CALCULAR_NUMERO_RESTRICCIONES_H

#include "tipos_optimizacion.h"
#include "osqp.h"

#ifdef __cplusplus
extern "C" {
#endif
	void calcular_numero_restricciones(informacion_procesada_t* informacion_sistema, OSQPInt* numero_restricciones);
	void calcular_numero_ecuaciones_estado_bateria(informacion_procesada_t* informacion_sistema,
		int* numero_ecuaciones_bateria);

#ifdef __cplusplus
}
#endif

#endif
