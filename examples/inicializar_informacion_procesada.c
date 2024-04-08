#include "tipos_optimizacion.h"
#include <stdlib.h>
#include <string.h>


void inicializar_informacion_procesada(informacion_procesada_t* informacion_procesada) {
  if (informacion_procesada == NULL) return;

  /*Se inicializa la informacion de los vehiculos*/
  informacion_procesada->informacion_vehiculos.numero_vehiculos = 0;
  informacion_procesada->informacion_vehiculos.vehiculos = NULL; //Se inicializa el puntero a NULL

  /*Se inicializa la informacion de las baterias*/
  informacion_procesada->informacion_baterias.numero_baterias = 0; //Se inicializa el puntero a NULL
  informacion_procesada->informacion_baterias.baterias = NULL;

  /*Se inicializa la informacion de los puntos de simulacion*/
  informacion_procesada->informacion_puntos_simulacion.numero_puntos_simulacion = 0;
  informacion_procesada->informacion_puntos_simulacion.delta_minutos = 0;
  informacion_procesada->informacion_puntos_simulacion.puntos_simulacion = NULL;

  /*Se inicializa la informacion de las restricciones del sistema*/
  memset(&informacion_procesada->informacion_restricciones_sistema, 0, sizeof(informacion_procesada->informacion_restricciones_sistema));

  /*Se inicializa la informacion de los precios */
  informacion_procesada->informacion_precio_compra.numero_horas = 0;
  informacion_procesada->informacion_precio_compra.precios = NULL;
  informacion_procesada->informacion_precio_venta.numero_horas = 0;
  informacion_procesada->informacion_precio_venta.precios = NULL;

  /*Se inicializa la informacion de los puntos adicionales*/
  informacion_procesada->informacion_puntos_adicionales.numero_puntos = 0;
  informacion_procesada->informacion_puntos_adicionales.puntos = NULL;
}