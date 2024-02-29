

#ifndef MAIN_H
#define MAIN_H

#include "definiciones_globales.h"
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	void calcular_limite_inferior_restricciones_bateria(informacion_procesada_t* informacion_sistema,
		OSQPFloat* l);
	void calcular_restricciones_borde_inferiores_potencia_red(informacion_procesada_t* informacion_sistema,OSQPFloat*l);
	void calcular_limite_inferior_borde_potencia_red_fases(informacion_procesada_t* informacion_sistema, OSQPFloat* l);
	void calcular_limite_inferior_borde_potencia_red_entrada_fases(informacion_procesada_t* informacion_sistema, OSQPFloat* l);
	void calcular_limite_inferior_borde_potencia_red_salida_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* l);
	int  calcular_limite_inferior_restricciones_potencia_terminales(informacion_procesada_t* informacion_sistema,
		OSQPFloat* l);
	

#ifdef __cplusplus
}
#endif

#endif