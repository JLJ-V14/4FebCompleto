


#ifndef CALCULAR_MATRIZ_A_H
#define CALCULAR_MATRIZ_A_H
#include "tipos_optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif


	int calcular_matriz_a(informacion_procesada_t* informacion_sistema, problema_optimizacion_t* problema_optimizacion,
		informacion_carga_terminales_t* elementos_programados_terminales);

#ifdef __cplusplus
}
#endif

#endif