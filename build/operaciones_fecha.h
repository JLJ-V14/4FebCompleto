#ifndef OPERACIONES_FECHA_H
#define OPERACIONES_FECHA_H

/* Include files */

#include "tipos_optimizacion.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif
	int cargar_fecha_algoritmo(datos_csv_algoritmo_t* datos_algoritmo, struct tm* fecha_inicial_algoritmo,
		struct tm* fecha_final_algoritmo);
	void cargar_fecha_carga(datos_csv_t* datos_carga, struct tm* fecha_inicial_carga,
		struct tm* fecha_final_carga, const int numero_fila);
	int comprobar_fecha_carga(struct tm fecha_inicial_algoritmo, struct tm fecha_final_algoritmo,
		struct tm fecha_inicial_carga, struct tm fecha_final_carga);
	int verificar_fecha_carga(datos_csv_t* datos_carga, datos_csv_algoritmo_t* datos_algoritmo,
		const int numero_fila);

	void cargar_fecha(const datos_csv_t* datos_entrada, struct tm* fecha, const int columna_csv_anyo,
		const int columna_csv_mes, const int columna_csv_dia, const int columna_csv_hora,
		const int columna_csv_minuto, const int numero_fila, const int incluir_minuto);

	int  verificar_orden_fechas(const struct tm fecha_anterior, const struct tm fecha_posterior,
		const int incluir_igual);

	int comparar_fechas(const void* a, const void* b);
	int obtener_diferencia_minutos(const struct tm* fecha_1, const struct tm* fecha_2);

#ifdef __cplusplus
}
#endif

#endif

