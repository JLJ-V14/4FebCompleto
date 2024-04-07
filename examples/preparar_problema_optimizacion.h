

#ifndef PREPARAR_PROBLEMA_OPTIMIZACION_H
#define PRERPAR_PROBLEMA_OPTIMIZACION_H

#include "tipos_optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif

	int preparar_problema_optimizacion(informacion_procesada_t* informacion_sistema,problema_optimizacion_t* problema_optimizacion,
		informacion_carga_terminales_t* programacion_carga_terminales);

#ifdef __cplusplus
}
#endif

#endif