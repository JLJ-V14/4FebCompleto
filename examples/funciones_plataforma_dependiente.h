#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>


#ifndef FUNCIONES_PLATAFORMA_DEPENDIENTE_H
#define FUNCIONES_PLATAFORMA_DEPENDIENTE_H


#ifdef __cplusplus
extern "C" {
#endif
	const char* obtener_tiempo_string(time_t* tiempo);
	char* strok_seguro(char* str, const char* delimitador, char** token);
	char* strdup_plataforma(const char* src);
	int      abrir_archivo(const char* nombre_archivo, const char* modo, FILE** archivo);
	void     imprimir_error(int codigo_error);
#ifdef __cplusplus
}
#endif

#endif