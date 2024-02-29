
#ifndef FUNCIONES_CALCULAR_VECTOR_U_H
#define FUNCIONES_CALCULAR_VECTOR_U_H

#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	void calcular_limite_superior_restricciones_bateria(informacion_procesada_t* informacion_sistema, OSQPFloat* u);
	void calcular_restricciones_superiores_borde_potencias_red(informacion_procesada_t* informacion_sistema, OSQPFloat* u);
	int  calcular_limites_superior_potencia_terminales(informacion_procesada_t* informacion_sistema, OSQPFloat* u);
	void calcular_restricciones_superiores_potencia_red_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* u);
	void calcular_restricciones_superiores_potencia_entrada_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* u);
	void calcular_restricciones_borde_superiores_potencia_salida_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* u);
#ifdef __cplusplus
}
#endif

#endif