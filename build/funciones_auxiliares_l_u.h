
#ifndef FUNCIONES_AUXILIARES_L_U_H
#define FUNCIONES_AUXILIARES_L_U_H

#include "osqp.h"
#include "tipos_optimizacion.h"


#ifdef __cplusplus
extern "C" {
#endif


	void calcular_minima_potencia_baterias(informacion_procesada_t* informacion_sistema, int terminal_actual,
		 informacion_carga_terminales_t* elementos_carga_terminales);

#ifdef __cplusplus
}
#endif

#endif