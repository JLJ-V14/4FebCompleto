
#ifndef VERIFICAR_BATERIAS_H
#define VERIFICAR_BATERIAS_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif
	int verificar_baterias( datos_csv_baterias_t* datos_csv_baterias,  datos_csv_algoritmo_t* datos_csv_algoritmo,
		 datos_csv_terminales_t* datos_csv_terminales);


#ifdef __cplusplus
}
#endif

#endif