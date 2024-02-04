

#ifndef validaciones_h
#define validaciones_h

/* include files */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
	bool es_un_numero(const char* str, int permitir_decimal);
	bool es_negativo(double numero);
	bool comprobar_porcentaje_bateria(const double porcentaje_bateria);
	int verificar_numero_terminal(const char* numero_terminal_string, const int numero_fila);
#ifdef __cplusplus
}
#endif

#endif