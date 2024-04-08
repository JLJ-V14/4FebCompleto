

#ifndef FUNCIONAR_CALCULAR_A_P_H
#define FUNCIONES_CALCULAR_A_P_H

#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	void incluir_columnas_baterias(informacion_procesada_t* informacion_sistema, OSQPInt* A_p, int* index_actual,
		int* columna_a_actual, informacion_carga_terminales_t* programacion_elementos_carga_terminal);

	void incluir_columnas_potencias_terminales(informacion_procesada_t* informacion_sistema, OSQPInt* A_p, int* index_actual,
		int* comienzo_columna_actual, informacion_carga_terminales_t* programacion_elementos_carga_terminal);

	void incluir_columnas_potencia_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_p, int* index_actual,
		int* comienzo_columna_actual);
	void incluir_columnas_potencia_entrada_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_p, int* index_actual,
		int* comienzo_columna_actual);
	void incluir_columnas_potencia_salida_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
		int* index_actual, int* comienzo_columna_actual);
	void incluir_columnas_potencias_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
		int* index_actual, int* comienzo_columna_actual);
	void incluir_columnas_potencia_entrada_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
		int* index_actual, int* comienzo_columna_actual);
	void incluir_columnas_potencia_salida_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_p,
		int* index_actual, int* comienzo_columna_actual);

#ifdef __cplusplus
}
#endif

#endif