#include "definiciones_globales.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*Se calcula la cantidad máxima de puntos de simulacion que se va a tener */

int calcular_numero_puntos_provisional(time_t* tiempo_actual, time_t *tiempo_final, int delta_simulacion_segundos,
                                       struct tm** fechas_adicionales) {
  // Se calcula la diferencia en segundos entre los dos tiempo
  double diferencia_segundos = difftime(*tiempo_final, *tiempo_actual);

  //Se calcula el numero de puntos de simulacion.
  int numero_puntos = (int)(diferencia_segundos / delta_simulacion_segundos);

  //Si hay un resto se añade un punto adicional 
  if ((int)diferencia_segundos % delta_simulacion_segundos > 0) {
    numero_puntos++;
  }

  // Se calcula el numero de fechas adicionales 
  int numero_fechas_adicionales = 0;
  while (fechas_adicionales[numero_fechas_adicionales] != NULL) {
  numero_fechas_adicionales++;
  }

  //Se incluye el numero de fechas adicionales al numero de puntos de simulacion
  numero_puntos += numero_fechas_adicionales;

  //Se incluye el numero de puntos correspondiente a las horas en punto
  struct tm temp_fecha_actual = *localtime(tiempo_actual);
  int numero_puntos_en_punto = 0;
  for (time_t t = *tiempo_actual; t <= *tiempo_final; t += delta_simulacion_segundos) {
    temp_fecha_actual = *localtime(&t);
    if (temp_fecha_actual.tm_min == 0 && temp_fecha_actual.tm_sec == 0) {
      numero_puntos_en_punto++;
    }
  }

  return numero_puntos += numero_puntos_en_punto;

}

/*Este subprograma se encarga de calcular los puntos de simulacion y la fecha asociada a cada uno de ellos*/
int cacular_puntos_simulacion(informacion_entrada_t* informacion_entrada, struct tm** fechas_adicionales,
  informacion_procesada_t* informacion_procesada, struct tm* fecha_inicial_algoritmo,
  struct tm* fecha_final_algoritmo, int delta_simulacion) {

  struct tm* fecha_actual = fecha_inicial_algoritmo;
  
  time_t tiempo_actual = mktime(fecha_inicial_algoritmo);
  time_t tiempo_final = mktime(fecha_final_algoritmo);

  //Se convierte el delta de simulacion de minutos a segundos
  int delta_simulacion_segundos = delta_simulacion * 60;

  //Se obtiene el tamaño provisional del numero de puntos de simulacion
  int numero_puntos_provisionales = calcular_numero_puntos_provisional(tiempo_actual,tiempo_final,delta_simulacion_segundos,
                                                         fechas_adicionales);

  //Se declara un puntero a las posiciones en memoria donde se encuentra la informacion de los puntos de simulacion
  punto_simulacion_t* informacion_puntos_simulacion = informacion_procesada->informacion_puntos_simulacion.puntos_simulacion;
  informacion_puntos_simulacion = (punto_simulacion_t*)malloc(numero_puntos_provisionales * sizeof(punto_simulacion_t));


  while (fecha_actual != fecha_final_algoritmo) {

    if (delta_simulacion_segundos == 0) {
      printf("No se puede tener un delta de 0 minutos\n");
      registrar_error("Fallo no se puede tener un delta de 0 minutos", REGISTRO_ERRORES);
      return ERROR;
    }

  }
}