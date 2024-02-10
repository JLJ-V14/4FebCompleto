
/*
   En este archivo.h y el correspondiente.c se encuentran los
   archivos relacionados con liberar espacio en memoria reservado
   a las diferentes variables que se usan en el programa.
*/

#ifndef LIBERAR_MEMORIA_H
#define LIBERAR_MEMORIA_H

#include "tipos_optimizacion.h"

#ifdef __cplusplus
extern "C" {
#endif


    // Libera memoria para múltiples conjuntos de datos CSV.
    void liberar_memoria_csv_individual(datos_csv_t* datos_csv);
    void liberar_memoria_csvs(informacion_entrada_t* informacion_sistema);
    void liberar_memoria_informacion_procesada(informacion_procesada_t* informacion_procesada);
    void finalizar_problema_optimizacion(problema_optimizacion_t* problema_optimizacion);
#ifdef __cplusplus
}
#endif

#endif 
