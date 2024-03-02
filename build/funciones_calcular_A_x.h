

#ifndef FUNCIONES_CALCULAR_A_X_H
#define FUNCIONES_CALCULAR_A_X_H

#include "osqp.h"
#include "tipos_optimizacion.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
	int incluir_terminos_baterias(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual,
		informacion_carga_terminales_t* programacion_carga_elementos_terminal);

	int incluir_terminos_potencias(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual);
	int cargar_puntos_iniciales_finales(informacion_procesada_t* informacion_sistema, OSQPInt terminal_actual,
		int* numero_elementos_terminales, int** puntos_iniciales,
		int** puntos_finales);
	int comparar_ints(const void* a, const void* b);
	bool comprobar_rango(int punto_actual, int punto_inicial, int punto_final);
	int  actualizar_index_adicional(int* index_adicional, int* puntos_finales, int punto_actual);
	void incluir_termino_potencia_red(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual);
	void incluir_termino_potencia_entrada_sistema(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual);
	void incluir_termino_potencia_salida_sistema(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual);
	void incluir_terminos_potencia_red_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual);
	void incluir_terminos_potencia_entrada_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual);
	void incluir_terminos_potencia_salida_fase(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual);
#ifdef __cplusplus
}
#endif

#endif