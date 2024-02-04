
#ifndef VERIFICAR_BATERIAS_H
#define VERIFICAR_BATERIAS_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif
	int verificar_baterias(const datos_csv_baterias_t* datos_csv_baterias, const datos_csv_algoritmo_t* datos_csv_algoritmo);


#ifdef __cplusplus
}
#endif

#endif