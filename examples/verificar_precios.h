#pragma once
#ifndef VERIFICAR_PRECIOS_H
#define VERIFICAR_PRECIOS_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>
#include "Tipos_Optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif
	int verificar_precios(datos_csv_precio_t* datos_precio_compra, datos_csv_precio_t* datos_precio_venta,
		datos_csv_algoritmo_t* datos_algoritmo);


#ifdef __cplusplus
}
#endif

#endif