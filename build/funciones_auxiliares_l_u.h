
#ifndef FUNCIONES_AUXILIARES_L_U_H
#define FUNCIONES_AUXILIARES_L_U_H

#include "osqp.h"
#include "tipos_optimizacion.h"


#ifdef __cplusplus
extern "C" {
#endif

	int calcular_puntos_iniciales_finales_baterias(informacion_procesada_t* informacion_sistema, OSQPInt terminal_actual,
		int* numero_baterias_terminales, int** puntos_iniciales, int** puntos_finales);
	int calcular_maxima_potencia_baterias(informacion_procesada_t* informacion_sistema, OSQPInt terminal_actual,
		int* numero_baterias_terminales, int* puntos_iniciales, int* puntos_finales, OSQPFloat** potencias_maximas);

#ifdef __cplusplus
}
#endif

#endif