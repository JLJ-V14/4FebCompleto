


#ifndef FUNCIONES_CALCULAR_A_I_H
#define FUNCIONES_CALCULAR_A_I_H

#include "osqp.h"
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif

	int  incluir_filas_baterias(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* index_actual,
		                        int* fila_actual, informacion_carga_terminales_t* elementos_programados_terminales);

	int  incluir_filas_potencias_terminales(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* index_actual,
		                                    int* fila_actual, informacion_carga_terminales_t* elementos_programados_terminales);

	void incluir_filas_potencia_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_i,int* ultima_fila_balance_bateria,
		                            int* index_actual);
	void incluir_filas_potencia_entrada_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* ultima_fila_balance_bateria,
		int* index_actual);

	void incluir_filas_potencia_salida_red(informacion_procesada_t* informacion_sistema, OSQPInt* A_i,
		int* ultima_fila_balance_bateria, int* index_actual);

	void incluir_filas_potencia_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* ultima_fila_balance_bateria,
		int* index_actual, char fase);
	void incluir_filas_potencia_entrada_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* ultima_fila_balance_bateria,
		int* index_actual, char fase);
	void incluir_filas_potencia_salida_red_fase(informacion_procesada_t* informacion_sistema, OSQPInt* A_i, int* ultima_fila_balance_bateria,
		int* index_actual, char fase);

#ifdef __cplusplus
}
#endif

#endif