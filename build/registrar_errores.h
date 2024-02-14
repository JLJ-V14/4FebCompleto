
#ifndef REGISTRAR_ERRORES_H
#define REGISTRAR_ERRORES_H

/* Include files */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
	void registrar_error(const char* mensaje, const char* nombre_achivo);
	void borrar_contenido_log(const char* nombre_archivo);

#ifdef __cplusplus
}
#endif

#endif
