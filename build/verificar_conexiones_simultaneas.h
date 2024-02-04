

#ifndef VERIFICAR_CONEXIONES_SIMULTANEAS_H
#define VERIFICAR_CONEXIONES_SIMULTANEAS_H

#include "tipos_optimizacion.h"
#ifdef __cplusplus
extern "C" {
#endif

	int verificar_simultaneidad_vehiculos_baterias(datos_csv_vehiculos_t* datos_vehiculos, datos_csv_baterias_t* datos_baterias);

#ifdef __cplusplus
}
#endif

#endif