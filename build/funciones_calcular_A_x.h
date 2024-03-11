

#ifndef FUNCIONES_CALCULAR_A_X_H
#define FUNCIONES_CALCULAR_A_X_H

#include "osqp.h"
#include "tipos_optimizacion.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

	int incluir_terminos_baterias_A_x(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual,
		informacion_carga_terminales_t* programacion_carga_terminales);
	


	int incluir_terminos_baterias_terminal_A_x(informacion_procesada_t* informacion_sistema, int terminal_actual,
		int* index_actual, OSQPFloat* A_x, informacion_carga_terminales_t* elementos_programados_carga_terminal);

	int incluir_terminos_potencias_A_x(informacion_procesada_t* informacion_sistema, OSQPFloat* A_x, int* index_actual,
		informacion_carga_terminales_t* elementos_programados_terminal);


	int incluir_terminos_potencias_terminal_A_x(informacion_procesada_t* informacion_sistema, OSQPInt terminal_actual,
		int* index_actual, OSQPFloat* A_x, char fase_terminal, informacion_carga_terminales_t* elementos_programados_terminal);
	
	
	int comparar_ints(const void* a, const void* b);
	bool comprobar_rango(int punto_actual, int punto_inicial, int punto_final);
	int  actualizar_index_elemento_carga(int* index_elemento_carga, int punto_final_actual, int punto_actual);
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