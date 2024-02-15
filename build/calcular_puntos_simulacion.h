

#ifndef CALCULAR_PUNTOS_SIMULACION_H
#define CALCULAR_PUNTOS_SIMULACION_H

#include "osqp.h"
#include "tipos_optimizacion.h"


#ifdef __cplusplus
extern "C" {
#endif
	int cacular_puntos_simulacion(informacion_entrada_t* informacion_entrada, struct tm** fechas_adicionales,
		informacion_procesada_t* informacion_procesada, struct tm* fecha_inicial_algoritmo,
		struct tm* fecha_final_algoritmo, int delta_simulacion, const int *numero_fechas_adicionales);
	int calcular_numero_puntos_provisional(time_t* tiempo_actual, time_t* tiempo_final, int delta_simulacion_segundos,
		struct tm** fechas_adicionales);
	int leer_fechas_adicionales(datos_csv_vehiculos_t* datos_vehiculos, struct tm** fechas_adicionales,
		datos_csv_baterias_t* datos_baterias, int* numero_fechas_adicionales, struct tm* fecha_inicial_algoritmo,
		struct tm* fecha_final_algoritmo);
	void obtener_punto_simulacion(OSQPInt* punto_simulacion, struct tm* fecha, puntos_adicionales_t* puntos_adicionales);

#ifdef __cplusplus
}
#endif

#endif