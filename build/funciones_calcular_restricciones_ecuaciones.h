
#ifndef FUNCIONES_CALCULAR_RESTRICCIONES_ECUACIONES_H
#define FUNCIONES_CALCULAR_RESTRICCIONES_ECUACIONES_H

#include "osqp.h"
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	void calcular_resultado_balance_fase(informacion_procesada_t* informacion_sistema,OSQPFloat*l, OSQPFloat*u);
	void calcular_resto_ecuaciones(informacion_procesada_t* informacion_sistema, OSQPFloat* l, OSQPFloat* u);
	int calcular_ecuaciones_balance_bateria(informacion_procesada_t* informacion_sistema, OSQPFloat* l, OSQPFloat* u);
#ifdef __cplusplus
}
#endif

#endif